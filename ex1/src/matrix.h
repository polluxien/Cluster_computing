#ifndef MATRIX_H
#define MATRIX_H

/*
 *  A matrix is a view into a continuous array of integers. The dimensions of the
 *  data are <rowCount> and <columnCount>. The index of the position (R, C) in a
 *  matrix <M> can be calculated like this:
 *
 *      index := R * M.columnCount + C
 *      value := M.values[index]
 *
 *  The position can be read using the helper macro M_INDEX and M_AT.
 */
typedef struct Matrix Matrix;
struct Matrix
{
    int rowCount;
    int columnCount;
    int *values;
};

#define M_INDEX(m, r, c) ((m).columnCount * (r) + (c))
#define M_AT(m, r, c) ((m).values[M_INDEX(m, r, c)])

// Memory operations
Matrix mat_alloc(int rowCount, int columnCount);
Matrix mat_copy(Matrix m);
void mat_free(Matrix m);

// Filling in data
void mat_diagonal(Matrix m, int value);
void mat_fill(Matrix m, int value);
void mat_random(Matrix m, int lowBound, int highBound);

// Operations on matrices
void mat_add(Matrix out, Matrix a, Matrix b);
void mat_multiply(Matrix out, Matrix a, Matrix b);
void mat_scale(Matrix out, int factor);

// Helpers for debugging
void mat_print_(const char *name, Matrix m);
#define mat_print(m) (mat_print_(#m, m))

// Matrix specific MPI operations
void mat_mpi_broadcast(Matrix *m);
Matrix mat_mpi_scatter_rows(Matrix m);
Matrix mat_mpi_gather_rows(Matrix m, int rowCount);

#endif // MATRIX_H
