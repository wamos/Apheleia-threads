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

/*typedef struct {
	uint64_t packetsConsumed;
    uint64_t bytesConsumed;
    uint64_t packetsProduced;
	uint64_t bytesProduced;
	uint64_t thread_time;
	uint64_t op_time;
} system_stats;*/


class MemWorker{//: public Worker{

	public:
	MemWorker(uint64_t id, const std::string& name);
	~MemWorker();

	/* Start and mainLoop are for threading*/
    system_stats* threadStart(const std::string& stage);
    //void mainLoop(std::promise<uint64_t> p);
	void sourceLoop();
	void sinkLoop();

	/* operations inside the thread*/
	void setQueue(SafeQueue* queue_ptr);
	void setBufferPool(BufferPool* bufpl_ptr);
	void getBufferFromPool(); // in terms of buffers
	bool pushBufferToQueue(); // in terms of buffers
	void getBufferFromQueue(); // in terms of buffers
	bool pushBufferToPool();
	uint64_t getBytesConsumed();
	uint64_t getBytesProduced();
	system_stats* getStats();
	void threadStop();
	void threadJoin();
	uint64_t getNanoSecond(struct timespec tp);
	void allocCycleStats(uint64_t size, const std::string& name);
	std::vector<uint64_t> getLeftStats();
	std::vector<uint64_t> getRightStats();		

	private:
	const uint64_t worker_id;
	std::thread the_thread;
	std::string worker_name;
	std::string worker_stage;
	system_stats* stats;
	/*--------------------*/
	std::vector<uint64_t> left_stats;
	std::vector<uint64_t> right_stats;
	uint64_t start_time;
	/*--------------------*/
	bool stop_thread;
	SafeQueue* queue;
	BufferPool* bufpl;
	BaseBuffer* WorkerBuffer;
	void writeBuffer();
	void cleanBuffer();
	uint64_t bytesConsumed;
    uint64_t bytesProduced;
};


#endif //MEM_WORKER_H
