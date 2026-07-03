#ifndef MPI_PROCESS_H
#define MPI_PROCESS_H



#include "point.h"


extern int mpiProcessRank;
extern int mpiProcessCount;

#define MPI_PROCESS_CONTROLLER if (mpiProcessRank == 0)
#define MPI_PROCESS_WORKER     if (mpiProcessRank != 0)

void mpiProcessInit(int *argc, char ***argv);
void mpiProcessFinalize(void);

void mpiBroadcastInt(int *value);
void mpiBroadcastPoints(Point *points, int pointCount);
void mpiGatherPoints(Point *points, RangeInt range, int *offsets, int *sizes);


#endif /* MPI_PROCESS_H */
