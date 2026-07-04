#include "point.h"


#include <stdlib.h>


double gravitationalConstant = 1.0;
double softeningParameter    = 1e-5;


Point point(V2 position, V2 velocity, double mass) {
    return (Point){position, velocity, mass};
}

Point *pointsAllocate(int count) {
    return (Point *)malloc(sizeof(Point) * count);
}

Point pointSample(RangeV2 position, RangeV2 velocity, RangeDouble mass) {
    return point(
        rangeV2Sample(position),
        rangeV2Sample(velocity),
        rangeDoubleSample(mass)
    );
}

void pointsSample(Point *points, int count, RangeV2 position, RangeV2 velocity, RangeDouble mass) {
    for (int i = 0; i < count; i++) {
        points[i] = pointSample(position, velocity, mass);
    }
};

V2 pointCalculateForce(Point target, Point *points, int count) {
    V2 force = v2(0, 0);
    
    for (int i = 0; i < count; i++) {
        Point point = points[i];

        V2 r = v2Subtract(point.position, target.position);
        double squaredDistance = v2DotProduct(r, r);

        double numerator   = gravitationalConstant * target.mass * point.mass;
        double denominator = squaredDistance + softeningParameter;
        double magnitude   = numerator / denominator;

        V2 forcePair = v2Scale(v2Normalize(r), magnitude);
        force = v2Add(force, forcePair);
    }

    return force;
}

void pointUpdate(Point *target, V2 force, double timestep) {
    V2 acceleration = v2Scale(force, target->mass);
    target->velocity = v2Add(target->velocity, v2Scale(acceleration, timestep));
    target->position = v2Add(target->position, v2Scale(target->velocity, timestep));
}

Point pointsCombine(Point *points, int count) {
    Point result = {0};

    for (int i = 0; i < count; i++) {
        double cumulativeMass = result.mass + points[i].mass;

        V2 weightedPosition = v2Add(
            v2Scale(result.position,    result.mass    / cumulativeMass),
            v2Scale(points[i].position, points[i].mass / cumulativeMass)
        );

        result = point(weightedPosition, v2(0, 0), cumulativeMass);
    }

    return result;
}
