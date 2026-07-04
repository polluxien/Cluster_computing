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

// Convenient helper for scoped timing.
//
// It would be sufficient to have the initalization (1st statement) and step (3rd statement)
// without zeroing scopeStart, but the compiler optimizes the whole for loop away because
// the condition (2nd statement) is always 0.
//
// Zeroing scopeStart and using it as the loop condition is probably sufficient to
// prevent the optimization.
#define PROFILE(time)                                                           \
    for(                                                                        \
        Timestamp scopeStart = timestampNow();                                  \
        scopeStart.seconds != 0 && scopeStart.nanoseconds != 0;                 \
        timestampAddSince(&(time), scopeStart), scopeStart = (Timestamp){0, 0}  \
    )


#endif // TIMING_h
