#ifndef RECT_H
#define RECT_H


#include "v2.h"

typedef struct Rect Rect;
struct Rect {
    V2 low;
    V2 high;
};

Rect rect(V2 low, V2 high);
V2   rectCenter(Rect r);


#endif // RECT_H
