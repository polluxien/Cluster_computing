#ifndef CONFIG_H
#define CONFIG_H


/*
 *  A worker range describes the subset of an integer range dedicated to the worker
 *  based on its rank, the total process count and the overall workload length. The
 *  range in mathematical notation is equal to [offset, offset+length).
 */
typedef struct WorkerRange WorkerRange;
struct WorkerRange
{
    int offset;
    int length;
    int totalLength;
};

// Abstraction for single process work
#define WORKER_ROOT_RANK (0)
#define WORKER_IS_ROOT   (worker_rank == WORKER_ROOT_RANK)

// Global values for the MPI rank and count
extern int worker_rank;
extern int worker_count;

// MPI setup and teardown
void worker_init(int *argc, char ***argv);
void worker_finalize(void);

// Worker ranges
WorkerRange worker_get_range(int rank, int length);
#define     worker_get_my_range(length) (worker_get_range(worker_rank, length))

// Helpers for debugging
void    worker_range_print(WorkerRange range);
#define worker_printf(format, ...) (printf("[worker:%d] " format, worker_rank, ## __VA_ARGS__))


#endif // CONFIG_H
