#ifndef WORKLOAD_H
#define WORKLOAD_H


/*
 *  Workload parameters for the matrix multiplication type. The programm will create
 *  two matrices of size m x p and p x n and multiply them into a matrix of size
 *  m x n. 
 */
typedef struct MatrixWorload MatrixWorkload;
struct MatrixWorload {
    int n;
    int p;
    int m; 
};

// Workload creation using constraints
MatrixWorkload workload_max_time(unsigned long long max_size_bytes);

// Workload complexity
unsigned long long workload_size(MatrixWorkload w);
unsigned long long workload_time(MatrixWorkload w);


#endif // WORKLOAD_H
