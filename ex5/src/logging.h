#ifndef LOGGING_H
#define LOGGING_H


#include <stdio.h>
#include "point.h"
#include "range.h"
#include "vector.h"

#define log(format, ...)     (fprintf(stderr, format, ## __VA_ARGS__))
#define logLine(format, ...) (log(format "\n", ## __VA_ARGS__))
#define logClearLine()       log("\r                                                                                \r")
#define logFlush()           (fflush(stderr))


void logRangeInt(RangeInt range);
void logRangeDouble(RangeDouble range);
void logRangeV2(RangeV2 range);
void logPoint(Point p);
void logV2(V2 v);


#endif /* LOGGING_H */
