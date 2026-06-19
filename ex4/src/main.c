#include <stdio.h>

#include <omp.h>

#include "v2.h"
#include "range.h"
#include "point.h"
#include "timing.h"
#include "quadtree.h"

// Flag to enable image generation for debug purposes.
const int imageGenerationOn = 0;
const int imageSize         = 1024;

// Parameter to easly calculate duration and timestep size of the simulation.
const int videoFramesPerSecond = 60;

// Simulation parameters.
const int    simBodyCount = 6000;
const double simStep      = 1.0 / (double)videoFramesPerSecond;
const V2     simPosLow    = (V2){-200, -200};
const V2     simPosHigh   = (V2){ 200,  200};
const double simMassLow   = { 0.1 };
const double simMassHigh  = { 10.0 };
const int    simIterCount = 60 * videoFramesPerSecond;
const double simTheta     = 1.2;

// Enable/Disable profiling.
#if 1
#define PROFILE(accumulator) for ( \
        Timestamp blockStart = timestampNow(); \
        blockStart.seconds != 0 || blockStart.nanoseconds != 0; \
        timestampAddSince(&(accumulator), blockStart), blockStart = (Timestamp){0, 0})
#else
#define PROFILE(accumulator)
#endif


int main(int argc, char **argv) {

    // Shared variables
    int      pointCount = 0;
    Point   *points     = 0;
    Quadtree tree       = {0};

    #pragma omp parallel
    {
        Timestamp startTime = timestampNow();

        // Collection of performance timers.
        Timestamp totalTime         = {0};
        Timestamp commTime          = {0};
        Timestamp imageTime         = {0};
        Timestamp multiComputeTime  = {0};
        Timestamp singleComputeTime = {0};

        int workerRank  = omp_get_thread_num();
        int workerCount = omp_get_num_threads();
        
        #pragma omp master
        {
            fprintf(stderr, "SETUP.\n"); fflush(stderr);
            PROFILE(singleComputeTime)
            {
                pointCount = simBodyCount;
                points     = pointsAlloc(pointCount);
                pointsGenerate(points, pointCount, simPosLow, simPosHigh, simMassLow, simMassHigh);

                tree = quadtreeAlloc(pointCount * 64);
            }
        }

        #pragma omp barrier

        // Allocate array for points of workers, except rank 0.
        RangeInt workerRange = rangeIntSplit(workerRank, workerCount, pointCount);

        // Perform simulation iteratively.
        for (int iteration = 0; iteration < simIterCount; iteration++) {

            // Build Quadtree.
            #pragma omp master
            {
                fprintf(stderr, "\r                         ");
                fprintf(stderr, "\rITERATION %5d.", iteration);
                fflush(stderr);

                PROFILE(singleComputeTime)
                {
                    Rect bounds = rectPointBounds(points, pointCount);
                    quadtreeBuild(&tree, bounds, points, pointCount);
                }
            }
            #pragma omp barrier

            // Update own slice of points.
            PROFILE(multiComputeTime) {
                // Update each point.
                for (int i = workerRange.low; i < workerRange.high; i++) {
                    Point *point = &points[i];
                    V2 force = quadtreeGetForce(&tree, *points, simTheta);
                    point->velocity = v2Add(point->velocity, v2Scale(force, simStep));
                    point->position = v2Add(point->position, v2Scale(point->velocity, simStep));
                }
            }
            #pragma omp barrier
        }


        // Print performance report.
        #pragma omp master
        {
            fprintf(stderr, "\n"); fflush(stderr);

            totalTime = timestampSince(startTime);

            unsigned long long totalNs = nanosecondsFromTimestamp(totalTime);
            unsigned long long totalMs = millisecondsFromTimestamp(totalTime);

            unsigned long long commNs = nanosecondsFromTimestamp(commTime);
            unsigned long long commMs = millisecondsFromTimestamp(commTime);

            unsigned long long singleComputeNs = nanosecondsFromTimestamp(singleComputeTime);
            unsigned long long singleComputeMs = millisecondsFromTimestamp(singleComputeTime);

            unsigned long long multiComputeNs = nanosecondsFromTimestamp(multiComputeTime);
            unsigned long long multiComputeMs = millisecondsFromTimestamp(multiComputeTime);

            printf("================================================================================\n");
            printf("worker_count        = %d\n", workerCount);
            printf("iteration_count     = %d\n", simIterCount);
            printf("body_count          = %d\n", simBodyCount);
            printf("theta               = %g\n", simTheta);
            printf("total_time          = %12llu ns (%7.3f sec) [%6.2f%%]\n", totalNs, totalMs * 1e-3, 100.0);
            printf("multi_compute_time  = %12llu ns (%7.3f sec) [%6.2f%%]\n", multiComputeNs, multiComputeMs * 1e-3, multiComputeNs * 100.0 / (double)totalNs);
            printf("single_compute_time = %12llu ns (%7.3f sec) [%6.2f%%]\n", singleComputeNs, singleComputeMs * 1e-3, singleComputeNs * 100.0 / (double)totalNs);
            printf("comm_time           = %12llu ns (%7.3f sec) [%6.2f%%]\n", commNs, commMs * 1e-3, commNs * 100.0 / (double)totalNs);
            // printf("================================================================================\n");
        }
    }
}
