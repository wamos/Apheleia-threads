#ifndef STAT_H
#define STAT_H

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "SafeQueue.h"

typedef struct {
	uint64_t packetsConsumed;
    uint64_t bytesConsumed;
    uint64_t packetsProduced;
	uint64_t bytesProduced;
	uint64_t thread_time;
	uint64_t op_time;
} system_stats;

/*
typedef struct{
	uint64_t pool2worker_time;
	uint64_t worker2queue_time;
} leftcycle_stats;


typedef struct{
    uint64_t queue2worker_time;
    uint64_t worker2pool_time;
} rightcycle_stats;
*/

#endif
