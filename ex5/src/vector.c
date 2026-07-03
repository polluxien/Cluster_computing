#include "vector.h"


#include <math.h>


V2 v2(double x, double y) {
    return (V2){x, y};
}

V2 v2Add(V2 a, V2 b) {
    return v2(a.x + b.x, a.y + b.y);
}

V2 v2Subtract(V2 a, V2 b) {
    return v2(a.x - b.x, a.y - b.y);
}

double v2DotProduct(V2 a, V2 b) {
    return a.x * b.x + a.y * b.y;
}

double v2Length(V2 v) {
    return sqrt(v2DotProduct(v, v));
}

V2 v2Normalize(V2 v) {
    V2 result = v;

    double length = v2Length(v);

    if (length > 0.0) {
        result = v2Scale(v, 1.0 / length);
    }

    return result;
}

V2 v2Scale(V2 v, double factor) {
    return v2(v.x * factor, v.y * factor);
}

int v2Equals(V2 a, V2 b) {
    return (a.x == b.x) && (a.y == b.y);
}
