#include "range.h"


#include <stdlib.h>

RangeInt rangeIntSplit(int blockIndex, int blockCount, int length) {
    RangeInt result = {0};
    
    // Only split if there is something to split.
    if (blockCount) {
        // Base sizes
        int size     = length / blockCount;
        int offset   = size * blockIndex;
        int leftover = length % blockCount;
        
        // Account for leftovers by distributing them one by one starting at the first block.
        if (blockIndex < leftover) {
            size++;
            offset += blockIndex;
        } else {
            offset += leftover;
        }

        result.low  = offset;
        result.high = offset + size;
    }

    return result;
}

double doubleSample(double low, double high) {
    double t = (double)rand() / (double)RAND_MAX; // [0, 1]
    return low + (high - low) * t;
}
