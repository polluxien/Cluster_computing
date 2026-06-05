#ifndef RANGE_H 
#define RANGE_H 


// Range of integers. For example used as index ranges for workers based on their rank and count.
typedef struct RangeInt RangeInt;
struct RangeInt
{
    int low;
    int high;
};

// Splits a range of <length> into <blockCount> blocks that are as equal as possible in size.
// Then selects the <blockIndex>'th block and returns its index range.
RangeInt rangeIntSplit(int blockIndex, int blockCount, int length);

// Samples a double from an interval.
double doubleSample(double low, double high);


#endif // RANGE_H 
