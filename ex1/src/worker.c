#include "worker.h"

#include <stdio.h>

#include <mpi.h>


// Global values for MPI rank and count
int worker_rank = 0;
int worker_count = 0;

void worker_init(int *argc, char ***argv) {
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &worker_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worker_count);
}

void worker_finalize() {
    MPI_Finalize();
}

/*
 *  Splits the range [0, length) idealy into same size chunks. If the total length is
 *  not divisible by the worker count the remainder gets distributed evenly starting 
 *  from the beginning.
 */
WorkerRange worker_get_range(int rank, int length) {
    WorkerRange range = {0};
    range.totalLength = length;

    // Split the length into uniform chunks of the same size
    range.offset = rank * (length / worker_count);
    range.length = length / worker_count;

    // If there are leftovers, distribute them and adjust the offsets
    int leftOver = length % worker_count;
    
    if (rank < leftOver) {
        range.length += 1;    // take a leftover
        range.offset += rank; // adjust offset
    } else {
        range.offset += leftOver; // adjust offset
    }

    return range;
}

void worker_range_print(WorkerRange range) {
    worker_printf("WorkerRange: [%d, %d)", range.offset, range.offset + range.length);

    if (range.length) {
        printf(" <#%d>", range.length);
    }

    printf("\n");
}
