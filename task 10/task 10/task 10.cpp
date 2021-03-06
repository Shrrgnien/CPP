// task 10.cpp: определяет точку входа для консольного приложения.
//


/*Постановка задачи (Задание 10)

Цель данной работы состоит в разработке программы для блочного
перемножения матриц, а также в сравнении времени выполнения в
зависимости от размера блока.
Согласно условиям задачи, матрицы хранятся в линеаризованном
виде. Матрица А верхне-треугольная. Хранится в виде одномерного
массива по блочным строкам. Матрица В нижне-треугольная. Хранится в
виде одномерного массива по блочным столбцам.
В процессе выполнения работы были реализованы следующие
способы выполнения программы:
1) Последовательное выполнение программы
2) Перемножение каждых двух блоков параллельно
3) Параллельное перемножение разных пар блоков в разных
вычислительных ядрах одновременно.
Параллельные варианты программы были реализованы при помощи
технологии OpenMP.
Проверка корректности перемножения матриц осуществляется путем
сравнения результатов вычислений реализованного метода и стандартного
перемножения матриц.
*/

#include "stdafx.h"
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <chrono>  
#define max -1000.0
#define min 1000.0
#define matrixWidth 2880
#define matrixSize matrixWidth*matrixWidth
#define blockWidth 96
#define blockSize blockWidth*blockWidth
#define blocksInARow matrixWidth/blockWidth
#define blockNumber matrixWidth*matrixWidth/blockWidth/blockWidth
using namespace std;
std::chrono::steady_clock::time_point time1, time2;
std::chrono::steady_clock::time_point time_begin;
std::chrono::duration<double> elapsedTime;

double createMatrix[matrixWidth][matrixWidth];
double* matrixB = new double[matrixSize];
double* matrixA = new double[matrixSize];
double* matrixC = new double[matrixSize];

float getRandomNumber()
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<float>(rand() * fraction * (max - min + 1) + min);
}

template <typename T>
void FillZeroMatrix(T *matrix) {
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = 0;
	}
}

template <typename T>
void ReadMatrix(string name, T* matrix) {
	ifstream file(name);
	int i = 0;
	while (!file.eof()) {
		file >> matrix[i];
		i++;
	}
	file.close();
}

void CreateMatrix(char c) {
	string name = "matrix ";
	name = name + c + ".txt";
	ofstream file(name, ios_base::trunc);
	if (c == 'A') {
		for (int i = 0; i < matrixWidth; i++)
			for (int j = 0; j < matrixWidth; j++) {
				if (i > j) createMatrix[i][j] = 0.0;
				else createMatrix[i][j] = getRandomNumber();
				file << createMatrix[i][j] << " ";
			}
	}
	else {
		for (int i = 0; i < matrixWidth; i++)
			for (int j = 0; j < matrixWidth; j++) {
				if (i < j) createMatrix[i][j] = 0;
				else createMatrix[i][j] = getRandomNumber();
				file << createMatrix[i][j] << " ";
			}
	}
	file.close();
}

template <typename T>
void PrintMatrix(T* a) {
	int k = 0;
	for (int i = 0; i < matrixSize; i++) {
		if (k != matrixWidth - 1) {
			k = k + 1;
			cout << a[i] << ", ";
		}
		else {
			k = 0;
			cout << a[i] << endl;
		}
	}
	cout << endl << endl;
}

template <typename T>
T *MatrixToBlocks(T *matrix, char type) {
	T *converted = new T[matrixSize];
	int width, height;
	width = 0;
	height = 0;
	int j = 0, i = 0, k = 0, elemCount = 0, rowCount = 1, columnCount = 1, blockCount = 0;
	if (type=='A')	{
		while (elemCount < matrixSize) {
			if ((k + 1) % matrixWidth != 0 && k != 0) {
				k = k - matrixWidth * (blockWidth - 1) + 1;
			else if (k != 0) k = k + 1;
			for (int blockElemCount = 0; blockElemCount < blockSize; blockElemCount++)
			{
				converted[elemCount] = matrix[k];
				if ((blockElemCount + 1) % blockWidth == 0 && blockElemCount != 0 && blockElemCount + 1 < blockSize) {
					k = k + matrixWidth - blockWidth + 1;
				}
				else if (blockElemCount + 1 < blockSize)
					k = k + 1;
				elemCount++;
			}
		}
	}
	else if (type=='B') {
		int columnNumber = 1;
		while (elemCount < matrixSize) {
			if (k + matrixWidth < matrixSize && k != 0) {
				k = k + matrixWidth - (blockWidth-1);
				blockCount++;
			}
				else if (k != 0) {
					k = blockWidth * columnNumber;
					columnNumber++;
				}
				for (int blockElemCount = 0; blockElemCount < blockSize; blockElemCount++) {
					converted[elemCount] = matrix[k];
					if ((blockElemCount + 1) % blockWidth == 0 && blockElemCount != 0 && blockElemCount + 1 < blockSize) {
						k = k + matrixWidth - blockWidth + 1;
					}
					else if (blockElemCount + 1 < blockSize) {
						k = k + 1;
					}
					elemCount++;
				}
			}
	}
	else cout << "Wrong matrix type" << endl;
	return converted;
	delete[] converted;
}

template <typename T>
void PrintBlocks(T* matrix) {
	int i = 0;
	while(i<matrixSize){
		for (int j = 0; j < blockSize; j++) {
			cout << matrix[i] << " ";
			if (j!=0 && (j+1)%blockWidth == 0 )
				cout << endl;
			i++;
		}
		cout << endl;
	}
	cout << endl << endl;
}

template <typename Elem_Type>
void BlockMult(Elem_Type* a, Elem_Type* b, Elem_Type* c, int block_width) {
	int block_size = block_width * block_width;
//#pragma omp parallel for
	for (int i = 0; i < block_size; i++)
		for (int j = 0; j < block_width; j++) {
			*(c + i) += *(a + (i / block_width)*block_width + j) * *(b + i % block_width + block_width * j);
		}
}

template <typename Elem_Type>
void PrintBlock(Elem_Type* matrix) {
	for (int i = 0; i < blockSize; i++) {
		cout << matrix[i] << " ";
		if ((i+1) % blockWidth == 0)
			cout << endl;
	}
	cout << endl;
}

template <typename Elem_Type>
void MultBlockMatrix(Elem_Type* a, Elem_Type* b, Elem_Type* c, int block_width){
	int block_size = block_width * block_width;
	int blocks_in_a_row = matrixWidth / block_width;
	int block_number = matrixWidth * matrixWidth / block_width / block_width;
//#pragma omp parallel for
	for (int i = 0; i < block_number; i++)
		for (int j = 0; j < blocks_in_a_row; j++) {
			if(!CheckZeroBlock((a + (i / (blocks_in_a_row))*matrixWidth*block_width + (block_size)* j), block_size) && !CheckZeroBlock((b + (i % (blocks_in_a_row))*matrixWidth*block_width + (block_size)* j), block_size))
			BlockMult((a + (i / (blocks_in_a_row))*matrixWidth*block_width + (block_size) * j), (b + (i % (blocks_in_a_row))*matrixWidth*block_width + (block_size) * j), (c + i * block_size), block_width);
		}
}

template <typename Elem_Type>
bool CheckZeroBlock(Elem_Type* a, int block_size) {
	for (int i = 0; i < block_size; i++) {
		if (*(a + i) != 0)
			return false;
	}
	return true;
}

int main()
{
	cout << "Reading..." << endl;
	ReadMatrix("matrix A.txt", matrixA);
	ReadMatrix("matrix B.txt", matrixB);
	cout << "Converting..." << endl;
	auto t1=MatrixToBlocks(matrixA,'A');
	auto t2 = MatrixToBlocks(matrixB, 'B');
	FillZeroMatrix(matrixC);
	cout << "BlockMatrix multiplication try..." << endl;
	ofstream file("results.txt", ios_base::trunc);
	file << "new muliplication " << endl;
	double average[10];
	double ave = 0;
	int block_width[20] = { 6, 10, 15, 20, 24, 30, 36, 40, 60, 72, 80, 96, 120, 144, 160, 180, 240, 360, 480, 720};
	for (int k = 0; k < 20; k++) {
		double ave = 0;
		int i = 4;
		while(i<5) {
			FillZeroMatrix(matrixC);
			time1 = std::chrono::steady_clock::now();
			MultBlockMatrix(t1, t2, matrixC, block_width[k]);
			time2 = std::chrono::steady_clock::now();
			std::chrono::duration<double, std::milli> elapsedTime = time2 - time1;
			std::cout << "Time: " << elapsedTime.count() / 1000.0 << std::endl;
			file << "Time: " << elapsedTime.count() / 1000.0 << endl;
			average[i] = elapsedTime.count() / 1000.0;
			cout << average[i] << endl;
			ave += average[i];
			if (elapsedTime.count() / 1000.0 > 100)
				i = 5;
			i++;
		}
		cout << endl << "block_width=" << block_width[k] << "; average time for 5 runs = " << ave / 5.000 << endl << endl;
		file << endl << "block_width=" << block_width[k] << "; average time for 5 runs = " << ave / 5.000 << endl << endl;
	}
	file.close();
	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixC;
	system("Pause");
    return 0;
}

