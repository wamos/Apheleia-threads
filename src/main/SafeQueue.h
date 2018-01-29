#ifndef SAFF_QUEUE_H
#define SAFE_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

#include "BaseBuffer.h"
//class BaseBuffer;

class SafeQueue{
    public:
    SafeQueue(uint64_t id_, uint64_t qsize);
    bool push(BaseBuffer* buf_);
    BaseBuffer* pop();
    BaseBuffer* getFront() const;
    BaseBuffer* getBack() const;
    bool isEmpty() const;
    uint64_t getSize() const;
    uint64_t totalProcessedBytes() const;

    private:
    std::mutex mu;
    std::condition_variable cond;
    std::deque<BaseBuffer*> bufferQueue;

    const uint64_t qid;
    uint64_t queueSize; //predeclared queue size
    uint64_t totalBytes;
    //const uint64_t size_ = 64; // fixed size or not?
};

#endif
