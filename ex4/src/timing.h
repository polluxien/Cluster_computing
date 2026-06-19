#ifndef TIMING_h
#define TIMING_h


/*
 *  Timing functions to measure durations. A timestamp is a counter from an arbitrary
 *  point in time. Only differences of them are meaningfull.
 */
typedef struct Timestamp Timestamp;
struct Timestamp {
    int seconds;
    int nanoseconds;
};

// Getting timestamps, differences and accumulation.
Timestamp timestampNow();
Timestamp timestampSince(Timestamp start);
Timestamp timestampAdd(Timestamp a, Timestamp b);
void      timestampAddSince(Timestamp *out, Timestamp a);
Timestamp timestampBetween(Timestamp end, Timestamp start);

// Getting durations in human-readable time.
unsigned long long secondsFromTimestamp(Timestamp t);
unsigned long long millisecondsFromTimestamp(Timestamp t);
unsigned long long microsecondsFromTimestamp(Timestamp t);
unsigned long long nanosecondsFromTimestamp(Timestamp t);


#endif // TIMING_h
