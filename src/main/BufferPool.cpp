#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

#include <iostream>
#include "BufferPool.h"
/*
class BufferPool{
	public:
	BufferPool(uint64_t id);
    bool allocBuffer(uint64_t num_buffer);
    BaseBuffer* getPoolBuffer();
    bool returnPoolBuffer();
    uint64_t getPoolSize() const;
    bool isEmpty() const;

	private:
    std::mutex pool_mutex;
    std::condition_variable pool_cond;
    uint64_t pool_id;
    std::deque<BaseBuffer*> buffersList;
    const uint64_t num_buffer;
};;
*/

BufferPool::BufferPool(uint64_t id)
	:pool_id(id){
}

bool BufferPool::allocBuffer(uint64_t num_buf, uint64_t buf_size){
	if(num_buf > 0){
		uint8_t* bufferPtrList = (uint8_t*)malloc(num_buf*sizeof(uint8_t));
		//printf("0:%p\n", &bufferPtrList[0]);
		for(int i = 0; i < num_buf; i++){
			BaseBuffer* buf_ptr=new BaseBuffer(&bufferPtrList[i], buf_size);
			//printf("1:%p\n", &bufferPtrList[i]);
			//printf("2:%p\n", buf_ptr->getBuffer());
			bufferList.push_back(buf_ptr);
		}
		//std::cout <<"allocated bufferList size "<<bufferList.size()<<"\n";
		num_buffer=num_buf;
		buffer_size=buf_size;
		return true;
	}
	else
		return false;
}

BaseBuffer* BufferPool::getPoolBuffer(){
	std::unique_lock<std::mutex> locker(pool_mutex);
	//std::cout <<bufferList.size()<<"\n";
	pool_cond.wait(locker, [this](){return bufferList.size() > 0;});
	BaseBuffer* front = bufferList.front();
	bufferList.pop_front();
	locker.unlock();
	pool_cond.notify_all();
	return front;
}

bool BufferPool::releasePoolBuffer(BaseBuffer* buf_){
	if( buf_ !=  NULL ){
		std::unique_lock<std::mutex> locker(pool_mutex);
		pool_cond.wait(locker, [this](){return bufferList.size() < num_buffer;});   
		bufferList.push_back(buf_);
		locker.unlock();
		pool_cond.notify_all();
		return true;
	}
}

uint64_t BufferPool::getPoolSize() const{
	return bufferList.size();
}

uint64_t BufferPool::getBufferSize() const{
	return buffer_size;
}

bool BufferPool::isEmpty() const{
	return bufferList.empty();
}



