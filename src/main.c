
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MATRIX_SIZE_M 100
#define MATRIX_SIZE_P 90
#define MATRIX_SIZE_N 80 

typedef struct Matrix Matrix;

struct Matrix {
    int height;
    int width;
    int *values;
};


void   getWorkRange(int work_index, int work_count, int data_size, int *offsetOut, int *sizeOut);
Matrix matrixAlloc(int height, int width);
void   matrixSum(Matrix a, Matrix b, Matrix out);
void   matrixPrint(Matrix m);
void   matrixZero(Matrix m);
void   matrixFillRandom(Matrix m);
int    getMatrixIndex(Matrix m, int row, int column);
void   matrixMultiply(Matrix a, Matrix b, Matrix out, int rowStart, int rowEnd);


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    int count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    int centerSize = 70;
    Matrix a = matrixAlloc(8, centerSize);
    Matrix b = matrixAlloc(centerSize, 4);
    Matrix c = matrixAlloc(a.height, b.width);

    if (rank == 0) {
        matrixFillRandom(a);
        matrixFillRandom(b);
    }

    matrixZero(c);

    MPI_Bcast(&a.height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a.width,  1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a.values, a.height * a.width, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&b.height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b.width,  1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.values, b.height * b.width, MPI_INT, 0, MPI_COMM_WORLD);
    
    int offset;
    int size;
    getWorkRange(rank, count, c.height, &offset, &size);

    matrixMultiply(a, b, c, offset, offset+size);

    printf("[worker:%d] c = ", rank);
    matrixPrint(c);

    /*
    int *output = 0;

    if (rank == 0)
        output = (int *)malloc(sizeof(int) * c.height * c.width);

    MPI_Reduce(c.values, &output, c.height * c.width, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    */

    MPI_Finalize();

    return 0;
}

Matrix matrixAlloc(int height, int width) {
    Matrix m = {0};
    m.height = height;
    m.width = width;
    m.values = (int *)malloc(height * width * sizeof(int));
    return m;
}

void matrixFillRandom(Matrix m) {
    for (int index = 0; index < m.height * m.width; index++) {
        m.values[index] = rand() % 10;
    }
}

void matrixPrint(Matrix m)
{
    printf("{\n");
    
    for (int row = 0; row < m.height; row += 1) {
        printf("  ");
        for (int column = 0; column < m.width; column += 1) {
            printf("%d, ", m.values[row * m.width + column]);
        }
        printf("\n");
    }

    printf("}\n");
}

void getWorkRange(int work_index, int work_count, int data_size, int *offsetOut, int *sizeOut) {
    int baseSize = data_size / work_count;
    int baseOffset = work_index * baseSize;
    int leftOut = data_size % work_count;

    int offset = baseOffset;
    int size = baseSize;

    if (work_index < leftOut)
    {
        size++;
        offset += work_index;
    }
    else
    {
        offset += leftOut;
    }

    *offsetOut = offset;
    *sizeOut = size;
}

int getMatrixIndex(Matrix m, int row, int column) {
    if (row < 0 || row >= m.height || column < 0 || column >= m.width) {
        printf("ERROR: Invalid coordinates (%d, %d) for matrix of size (%d, %d).\n", row, column, m.height, m.width);
        return 0;
    }

    return row * m.width + column;
}

void matrixZero(Matrix m) {
    memset(m.values, 0, m.height * m.width * sizeof(int));
}

void matrixSum(Matrix a, Matrix b, Matrix out) {
    if (a.height != b.height || b.height != out.height || a.width != b.width || b.width != out.width) {
        printf("ERROR: Unable to add matrices of size (%d, %d) and (%d, %d) into (%d, %d).\n", a.height, a.width, b.height, b.width, out.height, out.width);
        return;
    }

    for (int i = 0; i < a.height * a.width; i++) {
        out.values[i] = a.values[i] + b.values[i];
    }
}

void matrixMultiply(Matrix a, Matrix b, Matrix out, int rowStart, int rowEnd) {
    if (a.height != out.height || a.width != b.height || b.width != out.width) {
        printf("ERROR: Unable to multiply matrices of size (%d, %d) and (%d, %d) into (%d, %d).\n", a.height, a.width, b.height, b.width, out.height, out.width);
        return;
    }

    for (int row = rowStart; row < rowEnd; row++) {
        for (int column = 0; column < out.width; column++) {
            int outIndex = getMatrixIndex(out, row, column);
            out.values[outIndex] = 0;

            for (int index = 0; index < a.width; index++) {
                int aIndex = getMatrixIndex(a, row,   index);
                int bIndex = getMatrixIndex(b, index, column);
                out.values[outIndex] += a.values[aIndex] * b.values[bIndex];
            }
        }
    }
}
