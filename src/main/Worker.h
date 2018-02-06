#ifndef WORKER_H
#define WORKER_H

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

//Ref: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class Worker{
public:
    Worker(uint64_t id, const std::string& name);
    virtual ~Worker();
    virtual void threadStart(const std::string& stage);
    void threadStop();
    void threadJoin();
    void setQueue(SafeQueue* queue_ptr);
    void setBufferPool(BufferPool* bufpl_ptr);
    void getBufferFromPool(); // in terms of buffers
    bool pushBufferToQueue(); // in terms of buffers
    void getBufferFromQueue(); // in terms of buffers
    bool pushBufferToPool();
    uint64_t getBytesConsumed();
    uint64_t getBytesProduced();
    system_stats* getStats();
    //uint64_t getNanoSecond(struct timespec tp);

protected:
    const uint64_t worker_id;
    std::thread the_thread;
    std::string worker_name;
    std::string worker_stage;
    system_stats* stats;
    bool stop_thread; 
    SafeQueue* queue;
    BufferPool* bufpl;
    BaseBuffer* WorkerBuffer;  
    /*---------------------*/
    //std::vector<uint64_t> step_stats;
    //uint64_t start_time;
    /*--------------------*/


};

#endif //WORKER_H
