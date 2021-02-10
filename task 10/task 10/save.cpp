/*template <typename T>
T *Multiply(T* a, T* b) {
T *result = new T[matrixSize];
int elemCount = 0, rowCount=0, columnCount=0;
while (elemCount < matrixSize) {
for (int blockInARowCount = 0; blockInARowCount < blocksInARow; blockInARowCount++) {

for (int smth = 0; smth < matrixWidth; smth++) {
for (int elemInARow = 0; blockElemCount < matrixWidth; blockElemCount++) {
c[elemCount] += matrixA[rowCount] * matrixB[columnCount];
if ((elemInARow + 1) % blockWidth == 0 && elemInARow != 0 blockElemCount + 1 < blockSize) {
rowCount += blockSize - blockWidth + 1;
columnCount += blockWidth;
}
if (columnCount)
rowCount++;
columnCount += blockWidth;
elemCount++;
}
}
c[0] = a[0] * b[0] + a[1] * b[2] + a[4] * b[4] + a[5] * b[7];
}
}
int j = 0;
int k = 0;
for (int i = 0; i < matrixSize; i++) {
int count = 0;
if (i != 0 && i % (matrixWidth / blockWidth) == 0)
k = k + matrixWidth / blockWidth;
if (i % (matrixWidth / blockWidth) != 0)
j = j + matrixWidth / blockWidth;
else j = 0;
for (int count = 0; count<blocksInARow; count++) {
//cout << "k+count=" << k + count << "  j+count=" << j + count << endl;
if (i >= matrixSize && k + count >= matrixSize && j + count >= matrixSize)
cout << "ERRRRRRRRRRRRRRRRRRRR" << endl;
c[i] +=a.blockMatrix[k + count] * b.blockMatrix[j + count];
}
}

}*/

/*template<typename ELEMENT_TYPE>
void mult(ELEMENT_TYPE * A, ELEMENT_TYPE * B,
ELEMENT_TYPE * C, int n, int bSize)
{
ELEMENT_TYPE s, err;
int i, j, k, ik, jk, kk;
assert(n % bSize == 0);

for (j = 0; j < n; j++)
{
for (i = 0; i < n; i++)
{
C[j * n + i] = 0;
}
}


for (jk = 0; jk < n; jk += bSize)
20 Лабораторная работа №5
for (kk = 0; kk < n; kk += bSize)
for (ik = 0; ik < n; ik += bSize)
for (j = 0; j < bSize; j++)
for (k = 0; k < bSize; k++)
//#pragma simd
for (i = 0; i < bSize; i++)
C[(jk + j) * n + (ik + i)] +=
A[(jk + j) * n + (kk + k)] *
B[(kk + k) * n + (ik + i)];
}*/

/*if ((k%blockSize == 0) && (k != 0)) {
l += blockSize;
}
int j = k % blockSize;
for (int i = 0; i < blockSize; i++) {
if (k >= blockSize * blockSize && i + l >= blockSize * blockSize && j >= blockSize * blockSize)
cout << "ERRRRRRRRRRR" << endl;
c.block[k] = c.block[k] + a.block[i + l] * b.block[j];
j += blockSize;
}*/

//cout << "MULY END" << endl;

/*
if (rowCount < blockWidth) {
rowCount++;
i++;
}
else if (columnCount < blockWidth) {
columnCount++;
rowCount = 0;
i = i + matrixWidth - blockCount * blockWidth + 1;
}
else if (blockCount < blocksInARow) {
i = i - matrixWidth * (blockWidth - 1) + 1;
columnCount = 0;
rowCount = 0;
blockCount++;
}
else if (blockCount + 1 == blocksInARow) {
rowCount = 0;
columnCount = 0;
blockCount = 0;
i++;
}*/



/*BlockMatrix c;
//cout << "matrixSize=" << matrixSize << "  blockMatrixSize=" << blockMatrixSize << endl;
int j = 0;
int k = 0;
#pragma omp parallel for
for (int i = 0; i < blockMatrixSize; i++) {
int count = 0;
if (i!=0 && i % (matrixSize / blockSize) == 0)
k = k + matrixSize / blockSize;
if (i % (matrixSize / blockSize) != 0)
j = j + matrixSize / blockSize;
else j = 0;
//cout << "i=" << i << " k=" << k << " j=" << j << endl;
for(int count=0; count<matrixSize/blockSize; count++){
//cout << "k+count=" << k + count << "  j+count=" << j + count << endl;
if (i >= blockMatrixSize && k + count >= blockMatrixSize && j + count >= blockMatrixSize)
cout << "ERRRRRRRRRRRRRRRRRRRR" << endl;
if(!a.blockMatrix[k + count].CheckZeroBlock() && !b.blockMatrix[j + count].CheckZeroBlock())
c.blockMatrix[i] = c.blockMatrix[i] + a.blockMatrix[k + count] * b.blockMatrix[j + count];
}
}
return c;		*/
