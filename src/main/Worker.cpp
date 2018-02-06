#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Worker.h"

Worker::Worker(uint64_t wid, const std::string& name_)
:worker_id(wid),
worker_name(name_),
the_thread()
{
	stop_thread = false;
	stats = (system_stats*)malloc(sizeof(system_stats));
}

Worker::~Worker(){
	stop_thread = true;
}

void Worker::threadStop(){
	if(the_thread.joinable()){
		stop_thread = true;
	}
}

void Worker::threadJoin(){
	if(the_thread.joinable())
		the_thread.join();	
}

void Worker::threadStart(const std::string& stage){
	std::cout << "Any type of Workers need its own threadStart method \n";
}

void Worker::setQueue(SafeQueue* queue_ptr){
	queue=queue_ptr;
}

void Worker::setBufferPool(BufferPool* bufpl_ptr){
	bufpl=bufpl_ptr;
}

void Worker::getBufferFromPool(){
	WorkerBuffer = bufpl->getPoolBuffer();
	/*struct timespec ts;
	uint64_t t_pool2worker= Worker::getNanoSecond(ts);
	left_stats.push_back(t_pool2worker-start_time);*/
}

bool Worker::pushBufferToQueue(){
	queue->push(WorkerBuffer);
	/*struct timespec ts;
	uint64_t t_worker2queue= Worker::getNanoSecond(ts);
	left_stats.push_back(t_worker2queue-start_time);*/
}

void Worker::getBufferFromQueue(){
	WorkerBuffer = queue->pop();
	/*struct timespec ts;
	uint64_t t_queue2worker= Worker::getNanoSecond(ts);
	right_stats.push_back(t_queue2worker-start_time);*/
}

bool Worker::pushBufferToPool(){
	bufpl->releasePoolBuffer(WorkerBuffer);
	/*struct timespec ts;
	uint64_t t_worker2pool= Worker::getNanoSecond(ts);
	right_stats.push_back(t_worker2pool-start_time);*/
}

uint64_t Worker::getBytesConsumed(){
    return stats->bytesConsumed;
}

uint64_t Worker::getBytesProduced(){
    return stats->bytesProduced;
}

system_stats* Worker::getStats(){
    return stats;
}

/*
uint64_t Worker::getNanoSecond(struct timespec tp){
	clock_gettime(CLOCK_MONOTONIC, &tp);
	//clock_gettime(CLOCK_REALTIME, &tp);
    return (1000000000) * (uint64_t)tp.tv_sec + tp.tv_nsec;
}*/

