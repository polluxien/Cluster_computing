#include "range.h"


#include <stdlib.h>


double sample01() {
    return ((double)rand() / (double)RAND_MAX);
}

RangeDouble rangeDouble(double offset, double size) {
    return (RangeDouble){offset, size};
}

RangeInt rangeInt(int offset, int size) {
    return (RangeInt){offset, size};
}

RangeV2 rangeV2(V2 offset, V2 size) {
    return (RangeV2){offset, size};
}

double rangeDoubleSample(RangeDouble range) {
    return range.offset + range.size * sample01();
}

int rangeIntSample(RangeInt range) {
    return range.offset + (int)(range.size * sample01() + 0.5);
}

V2 rangeV2Sample(RangeV2 range) {
    return v2(
        rangeDoubleSample((RangeDouble){ range.offset.x, range.size.x }),
        rangeDoubleSample((RangeDouble){ range.offset.y, range.size.y })
    );
}

void rangeIntPartition(RangeInt range, int n, RangeInt *parition) {
    for (int i = 0; i < n; i++) {
        parition[i] = rangeIntPartitionAtIndex(range, n, i);
    }
}

RangeInt rangeIntPartitionAtIndex(RangeInt range, int n, int index) {
    int size     = range.size / n;
    int offset   = range.offset + size * index;
    int leftover = range.size % range.size;

    if (index < leftover) {
        offset += index;
        size++;
    } else {
        offset += leftover;
    }

    return rangeInt(offset, size);
}

RangeInt rangeIntPartitionWeightedAtIndex(RangeInt range, int n, int index, int *weights) {
    int blockCount   = 0;
    int blocksBefore = 0;

    for (int i = 0; i < n; i++) {
        blockCount += weights[i];

        if (i < index) blocksBefore += weights[i];
    }

    int size = 0;

    for (int i = blocksBefore; i < blocksBefore + weights[index]; i++) {
        size += rangeIntPartitionAtIndex(range, blockCount, i).size;
    }
    
    int offset = rangeIntPartitionAtIndex(range, blockCount, blocksBefore).offset;
    return rangeInt(offset, size);
}
