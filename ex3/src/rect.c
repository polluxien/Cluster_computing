#include "rect.h"

Rect rect(V2 low, V2 high) {
    return (Rect){low, high};
}

V2 rectCenter(Rect r) {
    return v2Add(r.low, v2Scale(v2Subtract(r.high, r.low), 0.5));
}
