#ifndef POINT_H
#define POINT_H


#include "v2.h"
#include "rect.h"
#include "range.h"


/*
 *  Points are data structures to describe point masses. A point mass has no area, but
 *  acceleration, velocity and position.
 */
typedef struct Point Point;
struct Point {
    V2 position;
    V2 velocity;
    double mass;
};

// Memory management.
Point *pointsAlloc(int count);

// Random generation
void pointsGenerate(Point *points, int pointCount, V2 posLow, V2 posHigh, double massLow, double massHigh);

// Simulation of the pairwise forces.
void  pointsUpdateVelocity(RangeInt targetRange, Point *points, int pointCount, double simStep);
void  pointsMove(Point *points, int count, double simulationStep);
Point pointsCombine(Point *points, int count);

Rect rectPointBounds(Point *points, int count);


#endif // POINT_H
