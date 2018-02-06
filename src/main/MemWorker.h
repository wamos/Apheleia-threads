#ifndef MEM_WORKER_H
#define MEM_WORKER_H

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <future>
#include <vector>

#include "BaseBuffer.h"
//#include "SafeQueue.h"
#include "BufferPool.h"
#include "Utils.h"
#include "Worker.h"

/*typedef struct {
	uint64_t packetsConsumed;
    uint64_t bytesConsumed;
    uint64_t packetsProduced;
	uint64_t bytesProduced;
	uint64_t thread_time;
	uint64_t op_time;
} system_stats;*/


class MemWorker: public Worker{

public:
	MemWorker(uint64_t id, const std::string& name);
	~MemWorker();

	/* Start and mainLoop are for threading*/
    void threadStart(const std::string& stage);
	void sourceLoop();
	void sinkLoop();

	/* operations inside the thread*/
	uint64_t getNanoSecond(struct timespec tp);
	void allocStepStats(uint64_t size);
	std::vector<uint64_t> getStepStats();		

private:
	void writeBuffer();
	void cleanBuffer();
	 /*---------------------*/
    std::vector<uint64_t> step_stats;
    uint64_t start_time;
    /*--------------------*/
};


#endif //MEM_WORKER_H
