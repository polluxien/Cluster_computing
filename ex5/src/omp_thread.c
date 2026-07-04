#include "omp_thread.h"


#include <omp.h>

int ompThreadId    = 0;
int ompThreadCount = 0;

void ompThreadSetup(int threadCount) {
    omp_set_num_threads(threadCount);
}

void ompThreadInit() {
    ompThreadId    = omp_get_thread_num();
    ompThreadCount = omp_get_num_threads();
}

void ompThreadFinalize() {
    // Nothing yet.
}

