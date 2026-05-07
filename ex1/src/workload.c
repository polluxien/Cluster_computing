#include "workload.h"

#include <math.h>

MatrixWorkload workload_max_time(unsigned long long max_size_bytes)
{
    // Find the maximal amount of coefficients that fit into the size constrain
    // Xenia: explaining how many integers kann fit into given bytes
    unsigned long long max_coefficients = max_size_bytes / sizeof(int);

    // Create two matrices that are the most squarish, because that maximizes the time
    // complexity m * p * n for a given size Xenia: maximizing no. of multiplication.
    int m = (int)pow(max_coefficients / 2.0, 1.0 / 2.0);
    int p = (int)(max_coefficients / (2.0 * m));
    int n = m;

    MatrixWorkload result = {0};
    result.m = m;
    result.p = p;
    result.n = n;
    return result;
}

// Calculates the space complexity of a workload
unsigned long long workload_size(MatrixWorkload w)
{
    unsigned long long m = w.m;
    unsigned long long p = w.p;
    unsigned long long n = w.n;
    return (n + m) * p * sizeof(int);
}

// Calculates the time complexity of a workload
unsigned long long workload_time(MatrixWorkload w)
{
    unsigned long long m = w.m;
    unsigned long long p = w.p;
    unsigned long long n = w.n;
    return n * m * p;
}
