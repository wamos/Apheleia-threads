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
#include "Worker.h"

/*typedef struct {
	uint64_t packetsConsumed;
    uint64_t bytesConsumed;
    uint64_t packetsProduced;
	uint64_t bytesProduced;
	uint64_t thread_time;
	uint64_t op_time;
} system_stats;*/


class NetWorker: public Worker{

public:
	NetWorker(uint64_t id, const std::string& name);
	~NetWorker();

	void threadStart(const std::string& stage);
	void sendLoop();
	void receiveLoop();

	void setSocket(Socket* sock_ptr);
	uint64_t getNanoSecond(struct timespec tp);
	void allocStepStats(uint64_t size);
	std::vector<uint64_t> getStepStats();
		
private:
	Socket* tcp_socket;
	std::string server_IP;
	std::string client_IP;
	/*---------------------*/
    std::vector<uint64_t> step_stats;
    uint64_t start_time;
    /*--------------------*/
};

#endif //NET_WORKER_H
