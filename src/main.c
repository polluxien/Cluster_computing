
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define OUTPUT_ENABLED 1

#define MATRIX_SIZE_M 3
#define MATRIX_SIZE_P 4
#define MATRIX_SIZE_N 5

typedef struct Matrix Matrix;

struct Matrix
{
    int height;
    int width;
    int *values;
};

// MPI helpers.
void mpiBroadcastMatrix(Matrix *m, int rank);
Matrix *mpiGatherMatrices(Matrix m, int rank, int count);
void mpiGetWorkRange(int work_index, int work_count, int data_size, int *offsetOut, int *sizeOut);

// Matrix
Matrix matrixAlloc(int height, int width);
void matrixPrint(Matrix m);
void matrixZero(Matrix m);
void matrixFillRandom(Matrix m);
int getMatrixIndex(Matrix m, int row, int column);
void matrixMultiply(Matrix a, Matrix b, Matrix out, int rowStart, int rowEnd);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    // Get process rank and total process count.
    int rank;
    int count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    Matrix a, b, c;

    if (rank == 0)
    {
        // Generate matrices.
        a = matrixAlloc(MATRIX_SIZE_M, MATRIX_SIZE_P);
        b = matrixAlloc(MATRIX_SIZE_P, MATRIX_SIZE_N);
        matrixFillRandom(a);
        matrixFillRandom(b);
    }

    // Broadcast data.
    mpiBroadcastMatrix(&a, rank);
    mpiBroadcastMatrix(&b, rank);

    c = matrixAlloc(a.height, b.width);
    matrixZero(c);

    // Execute workload.
    {
        int offset;
        int size;
        mpiGetWorkRange(rank, count, c.height, &offset, &size);
        matrixMultiply(a, b, c, offset, offset + size);
    }

    // Gather all matrices
    Matrix *matrices = mpiGatherMatrices(c, rank, count);

    // Combine values.
    if (rank == 0)
    {
        for (int worker_rank = 0; worker_rank < count; worker_rank++)
        {
            for (int i = 1; i < c.height * c.width; i++)
            {
                c.values[i] += matrices[worker_rank].values[i];
            }
        }

        if (OUTPUT_ENABLED)
        {
            printf("[worker:%d] c = ", rank);
            matrixPrint(c);
        }
    }

    MPI_Finalize();

    return 0;
}

Matrix *mpiGatherMatrices(Matrix m, int rank, int count)
{
    int *received = 0;

    if (rank == 0)
    {
        received = (int *)malloc(count * m.height * m.width * sizeof(int));
    }

    MPI_Gather(m.values, m.height * m.width, MPI_INT, received, m.height * m.width, MPI_INT, 0, MPI_COMM_WORLD);

    Matrix *matrices = (Matrix *)malloc(count * sizeof(Matrix));

    for (int i = 0; i < count; i++)
    {
        matrices[i].height = m.height;
        matrices[i].width = m.width;
        matrices[i].values = &received[m.height * m.width * i];
    }

    return matrices;
}

void mpiBroadcastMatrix(Matrix *m, int rank)
{
    MPI_Bcast(&m->height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m->width, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        m->values = (int *)malloc(m->height * m->width * sizeof(int));
    }

    MPI_Bcast(m->values, m->height * m->width, MPI_INT, 0, MPI_COMM_WORLD);
}

void mpiGetWorkRange(int work_index, int work_count, int data_size, int *offsetOut, int *sizeOut)
{
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

Matrix matrixAlloc(int height, int width)
{
    Matrix m = {0};
    m.height = height;
    m.width = width;
    m.values = (int *)malloc(height * width * sizeof(int));
    return m;
}

void matrixFillRandom(Matrix m)
{
    for (int index = 0; index < m.height * m.width; index++)
    {
        m.values[index] = rand() % 10;
    }
}

void matrixPrint(Matrix m)
{
    printf("{\n");

    for (int row = 0; row < m.height; row += 1)
    {
        printf("  ");

        for (int column = 0; column < m.width; column += 1)
        {
            printf("%d, ", m.values[row * m.width + column]);
        }

        printf("\n");
    }

    printf("}\n");
}

int getMatrixIndex(Matrix m, int row, int column)
{
    if (row < 0 || row >= m.height || column < 0 || column >= m.width)
    {
        printf("ERROR: Invalid coordinates (%d, %d) for matrix of size (%d, %d).\n", row, column, m.height, m.width);
        return 0;
    }

    return row * m.width + column;
}

void matrixZero(Matrix m)
{
    memset(m.values, 0, m.height * m.width * sizeof(int));
}

void matrixMultiply(Matrix a, Matrix b, Matrix out, int rowStart, int rowEnd)
{
    if (a.height != out.height || a.width != b.height || b.width != out.width)
    {
        printf("ERROR: Unable to multiply matrices of size (%d, %d) and (%d, %d) into (%d, %d).\n", a.height, a.width, b.height, b.width, out.height, out.width);
        return;
    }

    for (int row = rowStart; row < rowEnd; row++)
    {
        for (int column = 0; column < out.width; column++)
        {
            int outIndex = getMatrixIndex(out, row, column);
            out.values[outIndex] = 0;

            for (int index = 0; index < a.width; index++)
            {
                int aIndex = getMatrixIndex(a, row, index);
                int bIndex = getMatrixIndex(b, index, column);
                out.values[outIndex] += a.values[aIndex] * b.values[bIndex];
            }
        }
    }
}
