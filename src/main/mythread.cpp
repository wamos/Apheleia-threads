#ifndef WORKERCPP_H
#define WORKERCPP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <iostream>
#include <chrono>

#include "mythread.h"
//Ref: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
Worker::Worker(uint64_t id, const std::string& name): 
worker_id(id),
worker_name(name),
bytesConsumed(0),
bytesProduced(0),
the_thread(){
stop_thread = false;
stats = (system_stats*)malloc(sizeof(system_stats));
//stats->bytesProduced=0;
//stats->packetsProduced=0;
}

Worker::~Worker(){
    stop_thread = true;
    if(the_thread.joinable()){
        std::cout << "the_thread is joinable" << the_thread.joinable() <<"\n";   
        the_thread.join();
    }
}

system_stats* Worker::Start(std::promise<int> promise){
    std::cout << "Start"<< "\n";
    std::promise<uint64_t> p;
    auto f = p.get_future();
    //for(int i=0;i<100;i++){
    promise.set_value(99);
    //p.set_value(100);
    //p.set_value(101);
    //}
    the_thread = std::thread(&Worker::mainLoop, this, std::move(p));//std::move(p));
    the_thread.join();

    int i = f.get();
    std::cout << "start,count:" << i << "\n";
    return Worker::getStats();
}

void Worker::mainLoop(std::promise<uint64_t> p){//std::promise<int> && p){
    std::cout << "mainloop" << "\n";
    for(int i=0;i<10;i++){ //while
        if(!stop_thread){
            Worker::getBufferFromPool();
            Worker::writeBuffer();
            Worker::pushBufferToPool();
            std::cout << "in loop, Pool size" << bufpl->getPoolSize()<<"\n";
            //std::this_thread::sleep_for( std::chrono::seconds(1) );
        }
    }
    p.set_value(999);
    uint64_t my_bytes=Worker::getBytesProduced();
    std::cout << "Produced Bytes" << my_bytes <<"\n";
}

void Worker::getBufferFromPool(){
    WorkerBuffer = bufpl->getPoolBuffer();
}

bool Worker::pushBufferToPool(){
    bufpl->releasePoolBuffer(WorkerBuffer);
}

void Worker::setQueue(SafeQueue* queue_ptr){
    queue=queue_ptr;
}

void Worker::setBufferPool(BufferPool* bufpl_ptr){
    bufpl=bufpl_ptr;
}

void Worker::writeBuffer(){
    uint64_t buf_size = WorkerBuffer->getSize();
    std::cout << "writeBuffer"<< "\n";

    std::unique_lock<std::mutex> locker(thread_mutex);
    thread_cond.wait(locker, [this](){ return WorkerBuffer->getSize() > 0;}); 

    memset(WorkerBuffer->getBuffer(), 1 ,(size_t) buf_size);
    stats->bytesProduced+=buf_size*sizeof(uint8_t);
    stats->packetsProduced+=1;
    WorkerBuffer->setWritten(true);

    locker.unlock();
    thread_cond.notify_all();
}

uint64_t Worker::getBytesConsumed(){
    return 0; //stats->bytesConsumed;
}

uint64_t Worker::getBytesProduced(){
    return stats->bytesProduced;
}

system_stats* Worker::getStats(){
    return stats;
}
 


#endif //WORKERCPP_H
