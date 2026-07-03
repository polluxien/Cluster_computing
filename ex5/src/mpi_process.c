#include "mpi_process.h"


#include <mpi.h>
#include "logging.h"

int mpiProcessRank  = 0;
int mpiProcessCount = 0;

void mpiProcessInit(int *argc, char ***argv) {
    MPI_Init(argc, argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &mpiProcessRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiProcessCount);
}

void mpiProcessFinalize(void) {
    MPI_Finalize();
}

void mpiBroadcastInt(int *value) {
    MPI_Bcast(value, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void mpiBroadcastPoints(Point *points, int pointCount) {
    MPI_Bcast(points, sizeof(Point) * pointCount, MPI_CHAR, 0, MPI_COMM_WORLD);
}

void mpiGatherPoints(Point *points, RangeInt range, int *offsets, int *sizes) {
    MPI_Gatherv(
        (char *)points + range.offset,
        range.size,
        MPI_CHAR,
        points,
        sizes,
        offsets,
        MPI_CHAR,
        0,
        MPI_COMM_WORLD
    );
}
