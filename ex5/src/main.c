#include <stdlib.h>
#include "config.h"
#include "logging.h"
#include "mpi_process.h"
#include "omp_thread.h"
#include "point.h"
#include "rect.h"
#include "quadtree.h"
#include "timing.h"

int main(int argc, char **argv) {
    // Per process exeution.
    mpiProcessInit(&argc, &argv);
    Timestamp programStartTime = timestampNow();

    // Timers
    Timestamp communicationTime = {0};
    Timestamp parallelTime      = {0};
    

    // Per process state.
    Point *points     = 0;
    int    pointCount = 0;
    Quadtree quadtree = {0};
    

    // Generate points.
    MPI_PROCESS_CONTROLLER
    {
        pointCount = simBodyCount;
        points     = pointsAllocate(pointCount);
        pointsSample(points, pointCount, simPosition, simVelocity, simMass);
    }


    // Per process memory allocation
    PROFILE(communicationTime)
    {
        mpiBroadcastInt(&pointCount);
    }

    PROFILE(parallelTime)
    {
        MPI_PROCESS_WORKER { points = pointsAllocate(pointCount); }
        quadtree = quadtreeAlloc(pointCount * 64);
    }

    // Setup for MPI gathering.
    int *mpiOffsets = 0;
    int *mpiSizes   = 0;

    MPI_PROCESS_CONTROLLER
    {
        mpiOffsets = (int *)malloc(sizeof(int) * mpiProcessCount);
        mpiSizes   = (int *)malloc(sizeof(int) * mpiProcessCount);

        for (int i = 0; i < mpiProcessCount; i++) {
            RangeInt range = rangeIntPartitionWeightedAtIndex(rangeInt(0, pointCount), mpiProcessCount, i, simThreadCountPerProcess);
            mpiOffsets[i] = range.offset * sizeof(Point);
            mpiSizes[i]   = range.size   * sizeof(Point);
        }
    }
    
    RangeInt processRange = rangeIntPartitionWeightedAtIndex(rangeInt(0, pointCount), mpiProcessCount, mpiProcessRank, simThreadCountPerProcess);
    ompThreadSetup(simThreadCountPerProcess[mpiProcessRank]);
    
    // Per thread execution.
    #pragma omp parallel
    {
        ompThreadInit();
        RangeInt threadRange = rangeIntPartitionAtIndex(processRange, ompThreadCount, ompThreadId);

        // Start iterative simulation.
        for (int iteration = 0; iteration < simIterCount; iteration++) {
            // Log progress to Terminal.
            #pragma omp master
            {
                MPI_PROCESS_CONTROLLER
                {
                    double percentage = (iteration)*100.0/(double)simIterCount;

                    logClearLine();
                    log("Iteration %5d/%5d (%6.2f%%)", iteration+1, simIterCount, percentage);
                    logFlush();
                }
            }

            #pragma omp barrier

            // Per process distribution of current state. 
            #pragma omp master
            {
                PROFILE(communicationTime)
                {
                    mpiBroadcastPoints(points, pointCount);
                }

                PROFILE(parallelTime)
                {
                    Rect bounds = rectPointBounds(points, pointCount);
                    quadtreeBuild(&quadtree, bounds, points, pointCount);
                }
            }

            #pragma omp barrier

            for (int i = threadRange.offset; i < threadRange.offset + threadRange.size; i++) {
                V2 force = quadtreeGetForce(&quadtree, points[i], simTheta);
                pointUpdate(&points[i], force, simStep);
            }

            #pragma omp barrier

            #pragma omp master
            {
                PROFILE(communicationTime)
                {
                    mpiGatherPoints(points, processRange, mpiOffsets, mpiSizes);
                }
            }

            #pragma omp barrier
        }

        ompThreadFinalize();
    }

    MPI_PROCESS_CONTROLLER
    {
        logClearLine();
        logLine("Finished.");
    }

    mpiProcessFinalize();
    return 0;
}
