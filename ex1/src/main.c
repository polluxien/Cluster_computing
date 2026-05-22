#include "worker.h"
#include "matrix.h"
#include "workload.h"

#include <mpi.h>
#include <stdio.h>

// Some size calculation helpers
#define KB(x) (1024ULL * x)
#define MB(x) (1024ULL * KB(x))
#define GB(x) (1024ULL * MB(x))

// Used to determine m x p and p x n matrix dimensions.
#define SPACE_COMPLEXITY MB(96)

// Only coefficents 0 or 1
#define MATRIX_RAND_LOW (0)
#define MATRIX_RAND_HIGH (2)

#define M (1024ULL)
#define N M
#define P M

void memory_print(int byte_count)
{
    double number = byte_count;
    char *unit = "bytes";

    if (byte_count >= GB(1))
    {
        number /= (double)GB(1);
        unit = "Gb";
    }
    else if (byte_count >= MB(1))
    {
        number /= (double)MB(1);
        unit = "Mb";
    }
    else if (byte_count >= KB(1))
    {
        number /= (double)KB(1);
        unit = "Kb";
    }

    printf("%.2f %s", number, unit);
}

int main(int argc, char **argv)
{
    worker_init(&argc, &argv);

    // Initialize data
    Matrix a = {0};
    Matrix b = {0};

    if (WORKER_IS_ROOT)
    {
        /*
        for benchmarking purposes we create a matrix to be
        defined in such a way to take maximal possible time to compute
        in given space complexity
        MatrixParams work = workload_max_time(SPACE_COMPLEXITY);*/

        // enough just to run a program using macros from above
        // as default there are 1024 colums and exact same number of raws for each matrix
        MatrixParams work = create_default_workload(M, N, P);
        printf("M, P, N :=: %d, %d, %d\n", work.m, work.p, work.n);

        worker_printf("Workers = %d.\n", worker_count);
        worker_printf("TOTAL   space = ");
        memory_print(workload_size(work));
        printf(".\n");
        worker_printf("TOTAL   time  = %llu.\n", workload_time(work));

        //  allocate matrix and fill it with random values
        a = mat_alloc(work.m, work.p);
        mat_random(a, MATRIX_RAND_LOW, MATRIX_RAND_HIGH);

        b = mat_alloc(work.p, work.n);
        mat_random(b, MATRIX_RAND_LOW, MATRIX_RAND_HIGH);
    }

    // Communicate required data between workers
    //  each process recieve only relevant rows from matrix a
    //  and matrix b entierly
    Matrix a_slice = mat_mpi_scatter_rows(a); //  internal usage of MPI_Scatterv function
    mat_mpi_broadcast(&b);                    //  internal usage of MPI_Bcast function

    // Perform subtask
    worker_printf("Subtask space = ");
    memory_print((a_slice.rowCount + b.columnCount) * (unsigned long long)a_slice.columnCount * sizeof(int));
    printf(".\n");
    worker_printf("Subtask time  = %llu.\n", (a_slice.rowCount * a_slice.columnCount * (unsigned long long)b.columnCount));

    //  each process compute the multiplication results for
    //  its part of matrix a multiplied with b
    Matrix c_slice = mat_alloc(a_slice.rowCount, b.columnCount);
    mat_multiply(c_slice, a_slice, b);

    // Collect subresults
    Matrix c = mat_mpi_gather_rows(c_slice, a.rowCount); //  internal usage of MPI_Gatherv function

    if (WORKER_IS_ROOT)
    {
        // mat_print(c);
    }

    // Cleanup
    mat_free(a_slice);
    mat_free(c_slice);

    if (WORKER_IS_ROOT)
    {
        mat_free(a);
        mat_free(b);
        mat_free(c);
    }

    worker_finalize();
}
