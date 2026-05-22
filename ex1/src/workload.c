#include "workload.h"

#include <math.h>

MatrixParams workload_max_time(unsigned long long max_size_bytes)
{
    // Find the maximal amount of coefficients that fit into the size constrain
    unsigned long long max_coefficients = max_size_bytes / sizeof(int);

    // Create two matrices that are the most squarish, because that maximizes the time
    // complexity m * p * n for a given size.
    int m = (int)pow(max_coefficients / 2.0, 1.0 / 2.0);
    int p = (int)(max_coefficients / (2.0 * m));
    int n = m;

    MatrixParams result = {0};
    result.m = m;
    result.p = p;
    result.n = n;
    return result;
}

MatrixParams create_default_workload(unsigned long long m, unsigned long long n, unsigned long long p)
{
    MatrixParams result = {0};
    result.m = m;
    result.p = p;
    result.n = n;
    return result;
}

// Calculates the space complexity of a workload
unsigned long long workload_size(MatrixParams w)
{
    unsigned long long m = w.m;
    unsigned long long p = w.p;
    unsigned long long n = w.n;
    return (n + m) * p * sizeof(int);
}

// Calculates the time complexity of a workload
unsigned long long workload_time(MatrixParams w)
{
    unsigned long long m = w.m;
    unsigned long long p = w.p;
    unsigned long long n = w.n;
    return n * m * p;
}