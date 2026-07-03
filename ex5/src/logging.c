#include "logging.h"

void logRangeInt(RangeInt range) {
    log("[%d, %d)", range.offset, range.offset + range.size);
}

void logRangeDouble(RangeDouble range) {
    log("[%g, %g)", range.offset, range.offset + range.size);
}

void logRangeV2(RangeV2 range) {
    logRangeDouble(rangeDouble(range.offset.x, range.size.x));
    log(" x ");
    logRangeDouble(rangeDouble(range.offset.y, range.size.y));
}

void logPoint(Point p) {
    log("Point(position = ");
    logV2(p.position);
    log(", velocity = ");
    logV2(p.velocity);
    log(", mass = %g)", p.mass);
}

void logV2(V2 v) {
    log("V2(%g, %g)", v.x, v.y);
}
