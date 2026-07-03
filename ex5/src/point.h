#ifndef POINT_H
#define POINT_H



#include "vector.h"
#include "range.h"

typedef struct Point Point;
struct Point {
    V2 position;
    V2 velocity;
    double mass;
};


Point *pointsAllocate(int count);
Point  point(V2 position, V2 velocity, double mass);
Point  pointSample(RangeV2 position, RangeV2 velocity, RangeDouble mass);
void   pointsSample(Point *points, int count, RangeV2 position, RangeV2 velocity, RangeDouble mass);
V2     pointCalculateForce(Point target, Point *points, int count);
void   pointUpdate(Point *target, V2 force, double timestep);
Point  pointsCombine(Point *points, int count);


#endif /* POINT_H */
