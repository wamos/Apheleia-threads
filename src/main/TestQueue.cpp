//#ifndef TEST_H
//#define TEST_H
//#endif //TEST_H

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <iostream>
#include <memory>
#include <thread>

//#include "SafeQueue.h"
//#include "BaseBuffer.h"
#include "MemWorker.h"
#include "rdtsc.h"
#define CYCLE_TIME 0.33 // cycle time for b09 machine

//#include "BufferPool.h"

/*class Stringclass{
	public:
	Stringclass(uint64_t wid, const std::string& name_)
	:workerid(wid),
	name(name_){
	}

	std::string getName(){
		return name;
	}

	private:
	const uint64_t workerid;
	std::string name;
};*/

static inline uint64_t get_time_ns(void)
{
    struct timespec tp;

    if (clock_gettime(CLOCK_REALTIME, &tp) != 0)
        return -1;

    return (1000000000) * (uint64_t)tp.tv_sec + tp.tv_nsec;
}

int main( int argc, char** argv )
{
  
	/*uint64_t qid=100;
	std::string str = "abc";
	Stringclass* sc =new Stringclass(qid, str);
	std::cout <<"Name: "<< sc->getName()<<"\n";*/
	
	/*std::thread mThread1( playQueue, 3 );
	std::thread mThread2( playQueue, 4 );
	mThread1.join();
	mThread2.join();*/

	//TODO: 1. Timer for the prcoessing loop
	//TODO: 2. Threading, at least 2 threads for source and sink

	uint64_t qid=1001;
	uint64_t plid=101;

	uint64_t qsize=128;
	uint64_t num_buf=32;
	uint64_t buf_size=1024;

	uint64_t num_iter=1000*1000;
	uint64_t start,end, total_time=0;


	// TODO: maybe try C++11 Smart Pointer
	BufferPool* pl =new BufferPool(plid);
	pl->allocBuffer(num_buf , buf_size);
	SafeQueue* sq =new SafeQueue(qid, qsize);
	
	MemWorker* sourcer=new MemWorker(0,"sourcer-test");
	MemWorker* sinker=new MemWorker(1,"sinker-test");

	sourcer->setQueue(sq);
	sourcer->setBufferPool(pl);
	sinker->setQueue(sq);
	sinker->setBufferPool(pl);

	//std::cout <<"Queue is Empty? "<<sq->isEmpty()<<"\n";
	//std::cout <<"Pool is Empty? "<<pl->isEmpty()<<"\n";

	for(int i=0; i<num_iter; i++){
		//std::cout <<"Loop:" << i <<"\n";
		start=get_time_ns();
		sourcer->getBufferFromPool();
		//std::cout <<"getBufferFromPool"<<"\n";
		sourcer->Start();
		//sourcer->mainLoop("source");
		//std::cout <<"mainLoop"<<"\n";
		sourcer->pushBufferToQueue();
		//std::cout <<"pushBufferToQueue"<<"\n";
		sinker->getBufferFromQueue();
		//std::cout <<"getBufferFromQueue"<<"\n";
		sourcer->Start();
		//sinker->mainLoop("sink");
		//std::cout <<"mainLoop"<<"\n";
		sinker->pushBufferToPool();
		end=get_time_ns();
		total_time=total_time+(end-start);
	}

	std::cout <<"Consumed Bytes: "<< sourcer->getBytesProduced()<<" bytes\n";
	std::cout <<"Produced Bytes: "<< sinker->getBytesConsumed()<<" byes\n";
	std::cout <<"Time per loop: "<< (total_time)/num_iter <<" ns\n";


	// we need a test that a single buffer pass through a loop
	// bufferpool->sourcer->queue->sinker, check the ptr addr
	// if the addr is the same, then the loop is right.

  	return 0;
  	
}

