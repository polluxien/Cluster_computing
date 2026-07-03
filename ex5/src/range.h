#ifndef RANGE_H
#define RANGE_H


#include "vector.h"

typedef struct RangeDouble RangeDouble;
struct RangeDouble {
    double offset;
    double size;
};

typedef struct RangeInt RangeInt;
struct RangeInt {
    int offset;
    int size;
};

typedef struct RangeV2 RangeV2;
struct RangeV2 {
    V2 offset;
    V2 size;
};

RangeDouble rangeDouble(double offset, double size);
RangeInt    rangeInt(int offset, int size);
RangeV2     rangeV2(V2 offset, V2 size);

double rangeDoubleSample(RangeDouble range);
int    rangeIntSample(RangeInt range);
V2     rangeV2Sample(RangeV2 range);

RangeInt rangeIntPartitionAtIndex(RangeInt range, int n, int index);
RangeInt rangeIntPartitionWeightedAtIndex(RangeInt range, int n, int index, int *weights);


#endif /* RANGE_H */
