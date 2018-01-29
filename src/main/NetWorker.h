#ifndef NET_WORKER_H
#define NET_WORKER_H

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
#include "BufferPool.h"
#include "Socket.h"
#include "Utils.h"

/*typedef struct {
	uint64_t packetsConsumed;
    uint64_t bytesConsumed;
    uint64_t packetsProduced;
	uint64_t bytesProduced;
	uint64_t thread_time;
	uint64_t op_time;
} system_stats;*/


class NetWorker{

	public:
	NetWorker(uint64_t id, const std::string& name);
	virtual ~NetWorker();

	system_stats* threadStart(const std::string& stage);
	void sendLoop();
	void receiveLoop();

	void setQueue(SafeQueue* queue_ptr);
	void setBufferPool(BufferPool* bufpl_ptr);
	void setSocket(Socket* sock_ptr);
	void getBufferFromPool(); // in terms of buffers
	bool pushBufferToQueue(); // in terms of buffers
	void getBufferFromQueue(); // in terms of buffers
	bool pushBufferToPool();
	uint64_t getBytesConsumed();
	uint64_t getBytesProduced();
	system_stats* getStats();
	void threadStop();
	void threadJoin();
	uint64_t getNanoSecond(struct timespec tp);
	void allocCycleStats(uint64_t size, const std::string& name);
	std::vector<uint64_t> getLeftStats();
	std::vector<uint64_t> getRightStats();

	//virtual Logger* startIntervalLogger();		

	private:
	Socket* tcp_socket;
	std::string server_IP;
	std::string client_IP;

	const uint64_t worker_id;
	std::thread the_thread;
	std::string worker_name;
	std::string worker_stage;
	system_stats* stats;
	/*---------------------*/
	std::vector<uint64_t> left_stats;
	std::vector<uint64_t> right_stats;
	uint64_t start_time;
	/*--------------------*/
	bool stop_thread;
	SafeQueue* queue;
	BufferPool* bufpl;
	BaseBuffer* WorkerBuffer;
	void writeBuffer();
	void cleanBuffer();

    std::mutex sock_mutex;
    std::condition_variable sock_cond;
};

#endif //NET_WORKER_H
