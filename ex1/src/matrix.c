#include "worker.h"
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

Matrix mat_alloc(int rowCount, int columnCount)
{
    Matrix m = {0};

    if (rowCount && columnCount)
    {
        m.rowCount = rowCount;
        m.columnCount = columnCount;
        m.values = (int *)malloc(rowCount * columnCount * sizeof(int));
    }

    return m;
}

Matrix mat_copy(Matrix m)
{
    Matrix copy = mat_alloc(m.rowCount, m.columnCount);

    for (int row = 0; row < m.rowCount; row++)
    {
        for (int column = 0; column < m.columnCount; column++)
        {
            M_AT(copy, row, column) = M_AT(m, row, column);
        }
    }

    return copy;
}

void mat_free(Matrix m)
{
    if (m.values)
    {
        free(m.values);
    }
}

void mat_fill(Matrix m, int value)
{
    for (int row = 0; row < m.rowCount; row++)
    {
        for (int column = 0; column < m.columnCount; column++)
        {
            M_AT(m, row, column) = value;
        }
    }
}

void mat_diagonal(Matrix m, int value)
{
    for (int row = 0; row < m.rowCount; row++)
    {
        for (int column = 0; column < m.columnCount; column++)
        {
            M_AT(m, row, column) = (row == column) ? value : 0;
        }
    }
}

void mat_random(Matrix m, int lowBound, int highBound)
{
    int offset = lowBound;
    int length = highBound - lowBound;

    for (int row = 0; row < m.rowCount; row++)
    {
        for (int column = 0; column < m.columnCount; column++)
        {
            M_AT(m, row, column) = offset + (rand() % length);
        }
    }
}

void mat_add(Matrix out, Matrix a, Matrix b)
{
    if (
        out.rowCount != a.rowCount ||
        out.columnCount != a.columnCount ||
        out.rowCount != b.rowCount ||
        out.columnCount != b.columnCount)
    {
        // Warn about it.
        return;
    }

    for (int row = 0; row < out.rowCount; row++)
    {
        for (int column = 0; column < out.columnCount; column++)
        {
            M_AT(out, row, column) = M_AT(a, row, column) + M_AT(b, row, column);
        }
    }
}

void mat_multiply(Matrix out, Matrix a, Matrix b)
{
    if (out.rowCount != a.rowCount || out.columnCount != b.columnCount || a.columnCount != b.rowCount)
    {
        // Warn about it.
        return;
    }

    int n = a.columnCount;

    for (int row = 0; row < a.rowCount; row++)
    {
        for (int column = 0; column < b.columnCount; column++)
        {
            int coefficient = 0;

            for (int index = 0; index < n; index++)
            {
                coefficient += M_AT(a, row, index) * M_AT(b, index, column);
            }

            M_AT(out, row, column) = coefficient;
        }
    }
}

void mat_scale(Matrix out, int factor)
{
    for (int row = 0; row < out.rowCount; row++)
    {
        for (int column = 0; column < out.columnCount; column++)
        {
            M_AT(out, row, column) *= factor;
        }
    }
}

void mat_print_(const char *name, Matrix m)
{
    // Header
    printf("%s = Matrix[%d x %d]{\n", name, m.rowCount, m.columnCount);

    for (int row = 0; row < m.rowCount; row++)
    {
        // Indentation
        printf("  ");

        // Row vector
        for (int column = 0; column < m.columnCount; column++)
        {
            printf("%4d, ", M_AT(m, row, column));
        }

        printf("\n");
    }

    printf("}\n");
}

void mat_mpi_broadcast(Matrix *m)
{
    // Broadcast dimensions
    MPI_Bcast(&m->rowCount, 1, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);
    MPI_Bcast(&m->columnCount, 1, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);

    // Create buffer
    if (!WORKER_IS_ROOT)
    {
        *m = mat_alloc(m->rowCount, m->columnCount);
    }

    // Broadcast coefficients.
    MPI_Bcast(m->values, m->rowCount * m->columnCount, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);
}

Matrix mat_mpi_scatter_rows(Matrix m)
{
    // Broadcast dimensions
    int rowCount = 0;
    int columnCount = 0;

    if (WORKER_IS_ROOT)
    {
        rowCount = m.rowCount;
        columnCount = m.columnCount;
    }

    MPI_Bcast(&rowCount, 1, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);
    MPI_Bcast(&columnCount, 1, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);

    // Setup scatter ranges
    int *displacements = 0;
    int *sendCounts = 0;

    if (WORKER_IS_ROOT)
    {
        displacements = (int *)malloc(worker_count * sizeof(int));
        sendCounts = (int *)malloc(worker_count * sizeof(int));

        for (int rank = 0; rank < worker_count; rank++)
        {
            WorkerRange subrange = worker_get_range(rank, rowCount);
            displacements[rank] = subrange.offset * columnCount;
            sendCounts[rank] = subrange.length * columnCount;
        }
    }

    // Setup receive buffer
    WorkerRange range = worker_get_my_range(rowCount);
    Matrix submatrix = mat_alloc(range.length, columnCount);

    // Scatter data
    MPI_Scatterv(m.values, sendCounts, displacements, MPI_INT, submatrix.values, submatrix.rowCount * submatrix.columnCount, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);

    // Clean up
    if (WORKER_IS_ROOT)
    {
        free(displacements);
        free(sendCounts);
    }

    return submatrix;
}

Matrix mat_mpi_gather_rows(Matrix submatrix, int rowCount)
{
    int columnCount = submatrix.columnCount;

    // Setup receive buffer
    Matrix m = {0};

    if (WORKER_IS_ROOT)
    {
        m = mat_alloc(rowCount, columnCount);
    }

    // Setup gather ranges
    int *displacements = 0;
    int *receiveCounts = 0;

    if (WORKER_IS_ROOT)
    {
        displacements = (int *)malloc(worker_count * sizeof(int));
        receiveCounts = (int *)malloc(worker_count * sizeof(int));

        for (int rank = 0; rank < worker_count; rank++)
        {
            WorkerRange subrange = worker_get_range(rank, rowCount);
            displacements[rank] = subrange.offset * columnCount;
            receiveCounts[rank] = subrange.length * columnCount;
        }
    }

    // Gather data
    MPI_Gatherv(submatrix.values, submatrix.rowCount * submatrix.columnCount, MPI_INT, m.values, receiveCounts, displacements, MPI_INT, WORKER_ROOT_RANK, MPI_COMM_WORLD);

    // Clean up
    if (WORKER_IS_ROOT)
    {
        free(displacements);
        free(receiveCounts);
    }

    return m;
}
