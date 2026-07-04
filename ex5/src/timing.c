#include "timing.h"


#include <time.h>


#define NS_IN_SEC (1000000000ULL)


// Gets the current timestamp. Absolute timestep values have no meaning, only their differences.
Timestamp timestampNow() {
    struct timespec t = {0};
    clock_gettime(CLOCK_MONOTONIC, &t);

    Timestamp result = {0};
    result.seconds = t.tv_sec;
    result.nanoseconds = t.tv_nsec;
    return result;
}

// Wrapper for ease of use.
Timestamp timestampSince(Timestamp start) {
    return timestampBetween(timestampNow(), start);
}

Timestamp timestampAdd(Timestamp a, Timestamp b) {
    unsigned long long seconds = a.seconds + (unsigned long long)b.seconds;
    unsigned long long nanoseconds = a.nanoseconds + (unsigned long long)b.nanoseconds;

    // Account for overflows by adjusting the seconds.
    if (nanoseconds > NS_IN_SEC) {
        seconds     += 1;
        nanoseconds -= NS_IN_SEC;
    }
    
    Timestamp result = {0};
    result.seconds     = seconds;
    result.nanoseconds = nanoseconds;
    return result;
}

void timestampAddSince(Timestamp *out, Timestamp a) {
    *out = timestampAdd(*out, timestampSince(a));
}

Timestamp timestampBetween(Timestamp end, Timestamp start) {
    // Swap the timestamps, if start is later than end.
    if (start.seconds > end.seconds || (start.seconds == end.seconds && start.nanoseconds > end.nanoseconds)) {
        Timestamp temp = end;
        end = start;
        start = temp;
    }

    Timestamp result = {0};
    result.seconds = end.seconds - start.seconds;
    result.nanoseconds = end.nanoseconds - start.nanoseconds;
    
    // Account for overflows in nanoseconds by adjusting the seconds.
    if (result.nanoseconds < 0) {
        result.seconds     -= 1;
        result.nanoseconds += NS_IN_SEC;
    }
    
    return result;
}

// Basic calculations of human-readable time by combining nanoseconds and seconds from the timestamp.
unsigned long long secondsFromTimestamp(Timestamp t) {
    unsigned long long factorSec  = 1ULL;
    unsigned long long quotientNs = NS_IN_SEC;
    return (t.seconds * factorSec) + (t.nanoseconds / quotientNs);
}

unsigned long long millisecondsFromTimestamp(Timestamp t) {
    unsigned long long factorSec  = 1000ULL;
    unsigned long long quotientNs = 1000000ULL;
    return (t.seconds * factorSec) + (t.nanoseconds / quotientNs);
}

unsigned long long microsecondsFromTimestamp(Timestamp t) {
    unsigned long long factorSec  = 1000000ULL;
    unsigned long long quotientNs = 1000ULL;
    return (t.seconds * factorSec) + (t.nanoseconds / quotientNs);
}

unsigned long long nanosecondsFromTimestamp(Timestamp t) {
    unsigned long long factorSec  = NS_IN_SEC;
    unsigned long long quotientNs = 1ULL;
    return (t.seconds * factorSec) + (t.nanoseconds / quotientNs);
}
