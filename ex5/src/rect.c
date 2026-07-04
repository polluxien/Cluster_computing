#include "rect.h"


#include <math.h>

Rect rect(V2 low, V2 high) {
    return (Rect){low, high};
}

V2 rectCenter(Rect r) {
    return v2Add(r.low, v2Scale(v2Subtract(r.high, r.low), 0.5));
}

Rect rectPointBounds(Point *points, int pointCount) {
    Rect r = {0};

    if (pointCount) {
        r = rect(points[0].position, points[0].position);

        for (int i = 0; i < pointCount; i++) {
            V2 p = points[i].position;

            r.low.x  = fmin(r.low.x, p.x);
            r.low.y  = fmin(r.low.y, p.y);
            r.high.x = fmax(r.high.x, p.x);
            r.high.y = fmax(r.high.y, p.y);
        }
    }

    return r;
}
