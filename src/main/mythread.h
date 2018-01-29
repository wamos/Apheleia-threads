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

#include "SafeQueue.h"
#include "BufferPool.h"

typedef struct {
    //uint64_t packetsConsumed;
    //uint64_t bytesConsumed;
    uint64_t packetsProduced;
    uint64_t bytesProduced;
} system_stats;


//Ref: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class Worker{
public:
    //Explicitly using the default constructor to
    //underline the fact that it does get called 
    //Worker();
    Worker(uint64_t id, const std::string& name);
    ~Worker();
    system_stats* Start(std::promise<int> promise);
    void mainLoop(std::promise<uint64_t> p);
    void setQueue(SafeQueue* queue_ptr);
    void setBufferPool(BufferPool* bufpl_ptr);
    void getBufferFromPool(); // in terms of buffers
    //bool pushBufferToQueue(); // in terms of buffers
    //void getBufferFromQueue(); // in terms of buffers
    bool pushBufferToPool();
    uint64_t getBytesConsumed();
    uint64_t getBytesProduced();
    system_stats* getStats();

protected:
    const uint64_t worker_id;
    std::string worker_name;
    uint64_t bytesConsumed;
    uint64_t bytesProduced;
    std::thread the_thread;
    system_stats* stats;
    bool stop_thread;// = false;
    SafeQueue* queue;
    BufferPool* bufpl; 
    BaseBuffer* WorkerBuffer;
    std::mutex thread_mutex;
    std::condition_variable thread_cond;
    void writeBuffer();

};


#endif //WORKER_H
