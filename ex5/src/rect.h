#ifndef RECT_H
#define RECT_H


#include "vector.h"
#include "point.h"

typedef struct Rect Rect;
struct Rect {
    V2 low;
    V2 high;
};

Rect rect(V2 low, V2 high);
V2   rectCenter(Rect r);
Rect rectPointBounds(Point *points, int count);


#endif // RECT_H
