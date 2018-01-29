#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "SafeQueue.h"

// for const method
// https://stackoverflow.com/questions/751681/meaning-of-const-last-in-a-c-method-declaration

// std::thread not pthread, std:mutex not pthread_mutex_t
// https://stackoverflow.com/questions/13134186/c11-stdthreads-vs-posix-threads

// TODO: C++ Allocator
//https://stackoverflow.com/questions/31358804/whats-the-advantage-of-using-stdallocator-instead-of-new-in-c

SafeQueue::SafeQueue(const uint64_t id_, uint64_t qsize)
    :qid(id_),
    queueSize(qsize),
    totalBytes(0){
}

bool SafeQueue::push(BaseBuffer* buf_){
    if( buf_ !=  NULL ){
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, [this](){return bufferQueue.size() < queueSize;});   
        /*actual operation starts*/
        bufferQueue.push_back(buf_);
        totalBytes += buf_->getSize();
        /*actual operation ends*/
        locker.unlock();
        cond.notify_all();
        return true;
    }
}

BaseBuffer* SafeQueue::pop(){
    std::unique_lock<std::mutex> locker(mu);
    cond.wait(locker, [this](){return bufferQueue.size() > 0;});
    BaseBuffer* front = bufferQueue.front();
    bufferQueue.pop_front();
    locker.unlock();
    cond.notify_all();
    return front;
}

BaseBuffer* SafeQueue::getFront() const{
    return bufferQueue.front();  
}

BaseBuffer* SafeQueue::getBack() const{
    return bufferQueue.back();  
}

uint64_t SafeQueue::getSize() const{
    return bufferQueue.size();  
}

bool SafeQueue::isEmpty() const{
    return bufferQueue.empty();
}

uint64_t SafeQueue::totalProcessedBytes() const{
    return totalBytes;
}



