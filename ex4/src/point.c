#include "point.h"

#include <stdlib.h>
#include <math.h>


// The gravitational constant
const double G = 1.0;

// The softening parameter to mitigate division by zero and explosions of force vectors.
const double softeningParameter = 0.01;

Point *pointsAlloc(int count) {
    return (Point *)malloc(count * sizeof(Point));
}

void pointsGenerate(Point *points, int pointCount, V2 posLow, V2 posHigh, double massLow, double massHigh) {
    // Generate points with respect to the ranges from the parameters.
    for (int i = 0; i < pointCount; i++) {
        points[i].mass         = doubleSample(massLow, massHigh);
        points[i].position     = v2Sample(posLow, posHigh);
        points[i].velocity     = v2(0, 0);
    }
}

void pointsUpdateVelocity(RangeInt targetRange, Point *points, int pointCount, double simulationStep) {
    // Iterate over each point in the target range.
    for (int targetIndex = targetRange.low; targetIndex < targetRange.high; targetIndex++) {
        Point *target = &points[targetIndex];
        V2 F_total = {0};
    
        // Determine the accumulated force <F_total> on the target point.
        for (int pointIndex = 0; pointIndex < pointCount; pointIndex++) {
            if (targetIndex == pointIndex) {
                continue;
            }

            // Newton's Law of Universal Graviation.
            Point point = points[pointIndex];
            V2 r = v2Subtract(point.position, target->position);
            V2 F = v2Scale(r, (G * target->mass * point.mass) / (v2DotProduct(r, r) + softeningParameter));
            F_total = v2Add(F_total, F);
        }
    
        // Update the acceleration and velocity of the point.
        target->velocity = v2Add(target->velocity, v2Scale(F_total, simulationStep));
    }
}

void pointsMove(Point *points, int count, double simStep) {
    // Update all point positions based on their velocity for a certain timestep.
    for (int i = 0; i < count; i++) {
        Point *point = &points[i];
        point->position = v2Add(point->position, v2Scale(point->velocity, simStep));
    }
}

Point pointsCombine(Point *points, int pointCount) {
    Point result = {0};

    for (int i = 0; i < pointCount; i++) {
        Point point = points[i];
        double massTotal = result.mass + point.mass;

        if (massTotal) {
            double weightResult = result.mass / massTotal;
            double weightPoint  = point.mass  / massTotal;

            result.mass = massTotal;
            result.position = v2Add(
                v2Scale(result.position, weightResult),
                v2Scale(point.position,  weightPoint)
            );
        }
    }

    return result;
}

Rect rectPointBounds(Point *points, int pointCount) {
    Rect r = {0};

    if (pointCount) {
        r = rect(points[0].position, points[0].position);

        for (int i = 0; i < pointCount; i++) {
            V2 p = points[i].position;

            r.low.x = fmin(r.low.x, p.x);
            r.low.y = fmin(r.low.y, p.y);
            r.high.x = fmax(r.high.x, p.x);
            r.high.y = fmax(r.high.y, p.y);
        }
    }

    return r;
}

