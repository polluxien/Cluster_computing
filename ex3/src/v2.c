#include "v2.h"

#include <math.h>

#include "range.h"


double clamp(double value, double min, double max) {
    double result = value;
    if (result < min) result = min;
    if (result > max) result = max;
    return result;
}

int v2Equals(V2 a, V2 b) {
    return (a.x == b.x) && (a.y == b.y);
}

// Utility for easy construction.
V2 v2(double x, double y) {
     return (V2){x, y};
}

// Rnadom sampling from "Box" low to high.
V2 v2Sample(V2 low, V2 high) {
    return v2(
        doubleSample(low.x, high.x),
        doubleSample(low.y, high.y)
    );
}

// Basic arithmetic operations
V2 v2Add(V2 a, V2 b) {
    return v2(a.x + b.x, a.y + b.y);
}

V2 v2Subtract(V2 a, V2 b) {
    return v2(a.x - b.x, a.y - b.y);
}

V2 v2Scale(V2 v, double s) {
    return v2(v.x * s, v.y * s);
}

V2 v2Hadamard(V2 a, V2 b) {
    return v2(a.x * b.x, a.y * b.y);
}

double v2DotProduct(V2 a, V2 b) {
    return a.x * b.x + a.y * b.y;
}

double v2Length(V2 v) {
    return sqrt(v2DotProduct(v, v));
}

// Scale to size 1. If length is zero, return the input.
V2 v2Normalize(V2 v) {
    V2 result = v;
    double length = v2Length(v);

    if (length) {
        result = v2Scale(v, 1.0 / length);
    }

    return result;
}
