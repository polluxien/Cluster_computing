#ifndef OMP_THREAD_H
#define OMP_THREAD_H


extern int ompThreadId;
extern int ompThreadCount;

#pragma omp threadprivate(ompThreadId)
#pragma omp threadprivate(ompThreadCount)

void ompThreadSetup(int threadCount);
void ompThreadInit();
void ompThreadFinalize();


#endif /* OMP_THREAD_H */
