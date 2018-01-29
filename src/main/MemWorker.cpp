#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MemWorker.h"
//#include "rdtsc.h"

//http://thispointer.com/c11-how-to-use-stdthread-as-a-member-variable-in-class/
//https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
MemWorker::MemWorker(uint64_t wid, const std::string& name_)
:worker_id(wid),
worker_name(name_),
bytesConsumed(0),
bytesProduced(0),
the_thread()
{
	stop_thread = false;
	stats = (system_stats*)malloc(sizeof(system_stats));
}
//source_thread(),
//sink_thread()


MemWorker::~MemWorker(){
	stop_thread = true;
}

void MemWorker::threadStop(){
	if(the_thread.joinable()){
		stop_thread = true;
	}
}

void MemWorker::threadJoin(){
	if(the_thread.joinable())
		the_thread.join();	
}

system_stats* MemWorker::threadStart(const std::string& stage){
	// This will start the thread. Notice move semantics!
	std::string str1 ("source");
	std::string str2 ("sink");
	worker_stage=stage;
	/*
	std::promise<uint64_t> p_source;
	std::promise<uint64_t> p_sink;
    auto f_source = p_source.get_future();
    auto f_sink = p_sink.get_future();*/

	if( worker_stage == "source"){
		the_thread = std::thread(&MemWorker::sourceLoop, this);
	}
	else if (worker_stage == "sink"){
		the_thread = std::thread(&MemWorker::sinkLoop, this);
	}
	else{
		 std::cout << "invalid string\n";
	}
}

void MemWorker::sourceLoop(){
	std::cout << "sourceloop "<< "\n";
	struct timespec ts1,ts2,ts3,ts4;
    uint64_t thread_start, thread_end, write_start, write_end, write_interval=0; 
    thread_start = MemWorker::getNanoSecond(ts1);
    while(!stop_thread){
            MemWorker::getBufferFromPool();
			if(WorkerBuffer->getSize() > 0){
				write_start = MemWorker::getNanoSecond(ts2);
            	MemWorker::writeBuffer();
            	write_end = MemWorker::getNanoSecond(ts3);
            }
            MemWorker::pushBufferToQueue();
			write_interval = write_interval+ (write_end - write_start);
            //std::cout << "source, Pool  size:" << bufpl->getPoolSize()<<"\n";
            //std::cout << "source, Queue size:" << queue->getSize()<<"\n";
	}
    thread_end = MemWorker::getNanoSecond(ts4);
    stats->thread_time = thread_end - thread_start;
    stats->op_time = write_interval;

    std::cout << "source  bytes:" << stats->bytesProduced/(1024*1024*1024) <<" GB \n";
    std::cout << "source  packets:" << stats->packetsProduced<<"\n";
    std::cout << "source  overall time" << stats->thread_time/1000 <<" micro sec\n";
    std::cout << "source  writing time:"<< stats->op_time/1000 <<" micro sec\n";

    uint64_t source_bits= 8*stats->bytesProduced;
    double thread_rate=(double)source_bits/(double)stats->thread_time;
    double op_rate=(double)source_bits/(double)stats->op_time;
    std::cout << "source  overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "source  effective rate:" << op_rate <<" Gbps \n";
}

void MemWorker::sinkLoop(){
	std::cout << "sinkloop "<< "\n";
	struct timespec ts1,ts2,ts3,ts4;
    uint64_t thread_start, thread_end, clean_start, clean_end, clean_interval=0;
    thread_start = MemWorker::getNanoSecond(ts1);
    while(!stop_thread){
            MemWorker::getBufferFromQueue();
			if(WorkerBuffer->getSize() > 0){
				clean_start = MemWorker::getNanoSecond(ts2);
            	MemWorker::cleanBuffer();
            	clean_end = MemWorker::getNanoSecond(ts3);
            }
            MemWorker::pushBufferToPool();
			clean_interval = clean_interval+ (clean_end - clean_start);
            //std::cout << "sink, Pool  size:" << bufpl->getPoolSize()<<"\n";
            //std::cout << "sink, Queue size:" << queue->getSize()<<"\n";
	}
	thread_end = MemWorker::getNanoSecond(ts4);
    stats->thread_time = thread_end - thread_start;
    stats->op_time = clean_interval;
}

void MemWorker::setQueue(SafeQueue* queue_ptr){
	queue=queue_ptr;
}

void MemWorker::setBufferPool(BufferPool* bufpl_ptr){
	bufpl=bufpl_ptr;
}


void MemWorker::getBufferFromPool(){
	WorkerBuffer = bufpl->getPoolBuffer();
	struct timespec ts;
	uint64_t t_pool2worker= MemWorker::getNanoSecond(ts);
	left_stats.push_back(t_pool2worker-start_time);
	/*if(WorkerBuffer != NULL){
		std::cout <<"Good Buffer!"<<"\n";
	}*/
}

bool MemWorker::pushBufferToQueue(){
	queue->push(WorkerBuffer);
	struct timespec ts;
	uint64_t t_worker2queue= MemWorker::getNanoSecond(ts);
	left_stats.push_back(t_worker2queue-start_time);
}

void MemWorker::getBufferFromQueue(){
	WorkerBuffer = queue->pop();
	struct timespec ts;
	uint64_t t_queue2worker= MemWorker::getNanoSecond(ts);
	right_stats.push_back(t_queue2worker-start_time);
}

bool MemWorker::pushBufferToPool(){
	bufpl->releasePoolBuffer(WorkerBuffer);
	struct timespec ts;
	uint64_t t_worker2pool= MemWorker::getNanoSecond(ts);
	right_stats.push_back(t_worker2pool-start_time);
}

void MemWorker::writeBuffer(){
	uint64_t buf_size = WorkerBuffer->getSize();
	memset(WorkerBuffer->getBuffer(), 1 ,(size_t) buf_size);
	stats->bytesProduced+=buf_size*sizeof(uint8_t);
	stats->packetsProduced+=1;
	//WorkerBuffer->setWritten(true);
}

void MemWorker::cleanBuffer(){
	uint64_t buf_size = WorkerBuffer->getSize();
	memset(WorkerBuffer->getBuffer(), 0 , (size_t) buf_size);
	stats->bytesConsumed+=buf_size*sizeof(uint8_t);
	stats->packetsConsumed+=1;
	//WorkerBuffer->setWritten(false);
}

uint64_t MemWorker::getBytesConsumed(){
    return stats->bytesConsumed;
}

uint64_t MemWorker::getBytesProduced(){
    return stats->bytesProduced;
}

system_stats* MemWorker::getStats(){
    return stats;
}

uint64_t MemWorker::getNanoSecond(struct timespec tp){
	clock_gettime(CLOCK_REALTIME, &tp);
    return (1000000000) * (uint64_t)tp.tv_sec + tp.tv_nsec;
}

void MemWorker::allocCycleStats(uint64_t size, const std::string& side){
	std::string str1 ("left");
	std::string str2 ("right");

	if(side == "left"){
		left_stats.reserve(size*2);
	}
	else if (side == "right"){
		right_stats.reserve(size*2);
	}
	else{
		std::cout << "sth wrong" <<"\n";
	}

}

std::vector<uint64_t> MemWorker::getLeftStats(){
	return left_stats;
}
std::vector<uint64_t> MemWorker::getRightStats(){
	return right_stats;
}

