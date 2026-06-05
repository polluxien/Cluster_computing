#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

#include "v2.h"
#include "range.h"
#include "point.h"
#include "bitmap.h"
#include "timing.h"
#include "quadtree.h"

// Flag to enable image generation for debug purposes.
const int imageGenerationOn = 0;
const int imageSize         = 1024;

// Parameter to easly calculate duration and timestep size of the simulation.
const int videoFramesPerSecond = 60;

// Simulation parameters.
const int    simBodyCount = 10000;
const double simStep      = 1.0 / (double)videoFramesPerSecond;
const V2     simPosLow    = (V2){-200, -200};
const V2     simPosHigh   = (V2){ 200,  200};
const double simMassLow   = { 0.1 };
const double simMassHigh  = { 10.0 };
const int    simIterCount = 60 * videoFramesPerSecond;
const double simTheta     = 1.2;

// Globals for MPI values of the worker.
int workerRank  = 0;
int workerCount = 1;

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
    MPI_Init(&argc, &argv);

    // Collection of performance timers.
    Timestamp totalTime         = {0};
    Timestamp commTime          = {0};
    Timestamp imageTime         = {0};
    Timestamp multiComputeTime  = {0};
    Timestamp singleComputeTime = {0};

    Timestamp startTime = timestampNow();

    MPI_Comm_rank(MPI_COMM_WORLD, &workerRank);
    MPI_Comm_size(MPI_COMM_WORLD, &workerCount);

    // Generate points.
    int    pointCount = 0;
    Point *points     = 0;
    
    if (workerRank == 0) {
        PROFILE(singleComputeTime) {
            pointCount = simBodyCount;
            points     = pointsAlloc(pointCount);
            pointsGenerate(points, pointCount, simPosLow, simPosHigh, simMassLow, simMassHigh);
        }
    }

    // Broadcast points.
    PROFILE(commTime) {
        MPI_Bcast(&pointCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Allocate array for points of workers, except rank 0.
    RangeInt workerRange = rangeIntSplit(workerRank, workerCount, pointCount);

    if (workerRank != 0) {
        points = pointsAlloc(pointCount);
    }

    // Allocate quadtree for all
    Quadtree tree = quadtreeAlloc(pointCount * 64);
    
    // Calculate displacements and sizes for `Gatherv` calls.
    int *gatherDisplacements = 0;
    int *gatherSizes         = 0;

    if (workerRank == 0) {
        PROFILE(singleComputeTime) {
            gatherDisplacements = (int *)malloc(sizeof(int) * workerCount);
            gatherSizes         = (int *)malloc(sizeof(int) * workerCount);

            for (int rank = 0; rank < workerCount; rank++) {
                RangeInt range = rangeIntSplit(rank, workerCount, pointCount);
                gatherDisplacements[rank] = range.low * sizeof(Point);
                gatherSizes[rank]         = (range.high - range.low) * sizeof(Point);
            }
        }
    }
    
    // Allocate bitmap for image drawing.
    Bitmap bitmap = {0};

    if (workerRank == 0 && imageGenerationOn) {
        bitmap = bitmapAlloc(imageSize, imageSize);
    }

    // Perform simulation iteratively.
    for (int iteration = 0; iteration < simIterCount; iteration++) {
        // Broadcast current snapshot of points.
        PROFILE(commTime) {
            MPI_Bcast(points, pointCount * sizeof(Point), MPI_CHAR, 0, MPI_COMM_WORLD);
        }

        // Update own slice of points.
        PROFILE(multiComputeTime) {
            // Build Quadtree.
            Rect bounds = rectPointBounds(points, pointCount);
            quadtreeBuild(&tree, bounds, points, pointCount);
            
            // Update each point.
            for (int i = workerRange.low; i < workerRange.high; i++) {
                Point *point = &points[i];
                V2 force = quadtreeGetForce(&tree, *points, simTheta);
                point->velocity = v2Add(point->velocity, v2Scale(force, simStep));
                point->position = v2Add(point->position, v2Scale(point->velocity, simStep));
            }
        }

        // Gather updated snapshot of points.
        PROFILE(commTime) {
            MPI_Gatherv(points + workerRange.low, (workerRange.high - workerRange.low) * sizeof(Point), MPI_CHAR, points, gatherSizes, gatherDisplacements, MPI_CHAR, 0, MPI_COMM_WORLD);
        }
        
        // Generate image for iteration.
        if (workerRank == 0 && imageGenerationOn) {
            PROFILE(imageTime) {
                char filepath[256] = {0};
                // The file name pattern gets used by FFMPEG for video generation.
                sprintf(filepath, "dist/points-%05d.pgm", iteration);
                bitmapDrawPoints(bitmap, points, pointCount, simPosLow, simPosHigh);
                bitmapSavePgm(bitmap, filepath);
            }

        }
    }

    free(points);
    quadtreeFree(&tree);

    // Free allocated resources.
    if (workerRank == 0) {

        if (imageGenerationOn) {
            bitmapFree(bitmap);
        }
    }

    totalTime = timestampSince(startTime);

    // Print performance report.
    if (workerRank == 0) {
        unsigned long long totalNs = nanosecondsFromTimestamp(totalTime);
        unsigned long long totalMs = millisecondsFromTimestamp(totalTime);

        unsigned long long commNs = nanosecondsFromTimestamp(commTime);
        unsigned long long commMs = millisecondsFromTimestamp(commTime);

        unsigned long long imageNs = nanosecondsFromTimestamp(imageTime);
        unsigned long long imageMs = millisecondsFromTimestamp(imageTime);

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
        printf("image_time          = %12llu ns (%7.3f sec) [%6.2f%%]\n", imageNs, imageMs * 1e-3, imageNs * 100.0 / (double)totalNs);
        printf("multi_compute_time  = %12llu ns (%7.3f sec) [%6.2f%%]\n", multiComputeNs, multiComputeMs * 1e-3, multiComputeNs * 100.0 / (double)totalNs);
        printf("single_compute_time = %12llu ns (%7.3f sec) [%6.2f%%]\n", singleComputeNs, singleComputeMs * 1e-3, singleComputeNs * 100.0 / (double)totalNs);
        printf("comm_time           = %12llu ns (%7.3f sec) [%6.2f%%]\n", commNs, commMs * 1e-3, commNs * 100.0 / (double)totalNs);
        // printf("================================================================================\n");
    }

    MPI_Finalize();
}
