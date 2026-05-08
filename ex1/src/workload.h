#ifndef WORKLOAD_H
#define WORKLOAD_H

/*
 *  Workload parameters for the matrix multiplication type. The programm will create
 *  two matrices of size m x p and p x n and multiply them into a matrix of size
 *  m x n.
 */
typedef struct Params MatrixParams;
struct Params
{
    int n;
    int p;
    int m;
};

// Workload creation using constraints
MatrixParams workload_max_time(unsigned long long max_size_bytes);
MatrixParams create_default_workload(unsigned long long m, unsigned long long n, unsigned long long p);

// Workload complexity
unsigned long long workload_size(MatrixParams w);
unsigned long long workload_time(MatrixParams w);

#endif // WORKLOAD_H
