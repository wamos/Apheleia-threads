#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H


#include <deque>
#include <mutex>
#include <condition_variable>

#include "BaseBuffer.h"

class BufferPool{
	public:
	BufferPool(uint64_t id);
    bool allocBuffer(uint64_t numbuf, uint64_t buf_size);
    BaseBuffer* getPoolBuffer();
	bool releasePoolBuffer(BaseBuffer* buf_);
    uint64_t getPoolSize() const;
    uint64_t getBufferSize() const;
    bool isEmpty() const;

	private:
    std::mutex pool_mutex;
    std::condition_variable pool_cond;
    uint64_t pool_id;
    std::deque<BaseBuffer*> bufferList;
    uint64_t num_buffer;
    uint64_t buffer_size;
};

#endif //BUFFER_POOL_H
