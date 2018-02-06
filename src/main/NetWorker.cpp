#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "NetWorker.h"
//#include "rdtsc.h"

/*
typedef struct{
    uint64_t pool2worker_time;
    uint64_t worker2queue_time;
    uint64_t queue2worker_time;
    uint64_t worker2pool_time;
} cycle_stats
*/

//http://thispointer.com/c11-how-to-use-stdthread-as-a-member-variable-in-class/
//https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
NetWorker::NetWorker(uint64_t wid, const std::string& name_)
:Worker(wid, name_)
{
	//stop_thread = false;
	//stats = (system_stats*)malloc(sizeof(system_stats));
}


NetWorker::~NetWorker(){
	stop_thread = true;
}

/*void NetWorker::threadStop(){
	if(the_thread.joinable()){
		stop_thread = true;
	}
}

void NetWorker::threadJoin(){
	if(the_thread.joinable())
		the_thread.join();	
}*/

void NetWorker::threadStart(const std::string& stage){
	// This will start the thread. Notice move semantics!
	std::string str1 ("send");
	std::string str2 ("recv");
	worker_stage=stage;
	struct timespec ts;
	start_time = NetWorker::getNanoSecond(ts);
	/*
	std::promise<uint64_t> p_source;
	std::promise<uint64_t> p_sink;
    auto f_source = p_source.get_future();
    auto f_sink = p_sink.get_future();*/

	if( worker_stage == "send"){
		the_thread = std::thread(&NetWorker::sendLoop, this);
	}
	else if (worker_stage == "recv"){
		the_thread = std::thread(&NetWorker::receiveLoop, this);
	}
	else{
		 std::cout << "invalid string\n";
	}
}

void NetWorker::sendLoop(){
	std::cout << "sendLoop "<< "\n";
	std::string ipaddr ("10.1.100.31"); // server
	std::string port ("8000");
	uint64_t Retry=5;
	uint64_t retryDelayInMicros=1000;

	struct timespec ts1,ts2,ts3,ts4, ts5, ts6;
    uint64_t thread_start, thread_end, send_start, send_end;
    uint64_t pool_end, queue_start, write_interval=0; 
    thread_start = NetWorker::getNanoSecond(ts1);
    tcp_socket->tcpConnect(ipaddr, port, bufpl->getBufferSize(), retryDelayInMicros, Retry);
    while(!stop_thread){
    		queue_start = NetWorker::getNanoSecond(ts2);
            NetWorker::getBufferFromQueue();
    		//NetWorker::getBufferFromPool();
			if(WorkerBuffer->getSize() > 0){
				send_start = NetWorker::getNanoSecond(ts3);
            	//TODO put it in a method
 				uint64_t sent_bytes=tcp_socket->tcpSend(WorkerBuffer->getBuffer(), WorkerBuffer->getSize());	
				stats->bytesConsumed+=sent_bytes;//*sizeof(uint8_t);
				stats->bytes_stats.push_back(sent_bytes);
				stats->packetsConsumed+=1;
            	send_end = NetWorker::getNanoSecond(ts4);
            	NetWorker::pushBufferToPool();
            	pool_end = NetWorker::getNanoSecond(ts5);
            	step_stats.push_back(queue_start);	
				step_stats.push_back(send_start);
            	step_stats.push_back(send_end);
            	step_stats.push_back(pool_end);
				write_interval = write_interval+ (send_end - send_start);
            }

            //std::cout << "source, Pool  size:" << bufpl->getPoolSize()<<"\n";
            //std::cout << "source, Queue size:" << queue->getSize()<<"\n";
	}
	tcp_socket->tcpClose();
    thread_end = NetWorker::getNanoSecond(ts6);
    stats->thread_time = thread_end - thread_start;
    stats->op_time = write_interval;

    /*std::cout << "send  bytes:" << stats->bytesProduced/(1024*1024*1024) <<" GB \n";
    std::cout << "send  packets:" << stats->packetsProduced<<"\n";
    std::cout << "send  overall time" << stats->thread_time/1000 <<" micro sec\n";
    std::cout << "send  writing time:"<< stats->op_time/1000 <<" micro sec\n";

    uint64_t source_bits= 8*stats->bytesProduced;
    double thread_rate=(double)source_bits/(double)stats->thread_time;
    double op_rate=(double)source_bits/(double)stats->op_time;
    std::cout << "send  overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "send  effective rate:" << op_rate <<" Gbps \n";
	*/

}

void NetWorker::receiveLoop(){
	std::cout << "receiveLoop "<< "\n";
	std::string port ("8000");
	uint64_t timeoutInMicros= 1000*1000*60;
	int backlogSize=10;

	struct timespec ts1,ts2,ts3,ts4,ts5,ts6;
    uint64_t thread_start, thread_end, recv_start, recv_end;
    uint64_t pool_start, queue_end, recv_interval=0;
    thread_start = NetWorker::getNanoSecond(ts1);
    tcp_socket->tcpListen(port, backlogSize);
    Socket* new_socket=tcp_socket->tcpAccept(timeoutInMicros, bufpl->getBufferSize());

    //std::unique_lock<std::mutex> locker(sock_mutex);
    //sock_cond.wait(locker, [this](){return new_socket != NULL;});

    while(!stop_thread){
    		pool_start = NetWorker::getNanoSecond(ts2);
            NetWorker::getBufferFromPool();
			if(WorkerBuffer->getSize() > 0){
				recv_start = NetWorker::getNanoSecond(ts3);
				//TODO
 				uint64_t recv_bytes=new_socket->tcpReceive( WorkerBuffer->getBuffer(), WorkerBuffer->getSize());
 				if(recv_bytes >0){
 					stats->bytesProduced+=recv_bytes;
 					stats->bytes_stats.push_back(recv_bytes);
					stats->packetsProduced+=1;
					recv_end = NetWorker::getNanoSecond(ts4);
					NetWorker::pushBufferToQueue();
					queue_end = NetWorker::getNanoSecond(ts5);
					step_stats.push_back(pool_start);
					step_stats.push_back(recv_start);
            		step_stats.push_back(recv_end);
            		step_stats.push_back(queue_end);
					recv_interval = recv_interval+ (recv_end - recv_start);
				}
            }

            //std::cout << "sink, Pool  size:" << bufpl->getPoolSize()<<"\n";
            //std::cout << "sink, Queue size:" << queue->getSize()<<"\n";
	}
	tcp_socket->tcpClose();
	//locker.unlock();
	//sock_cond.notify_all();

	thread_end = NetWorker::getNanoSecond(ts6);
    stats->thread_time = thread_end - thread_start;
    stats->op_time = recv_interval;
}

void NetWorker::setSocket(Socket* sock_ptr){
	tcp_socket=sock_ptr;
}

/*
void NetWorker::setQueue(SafeQueue* queue_ptr){
	queue=queue_ptr;
}

void NetWorker::setBufferPool(BufferPool* bufpl_ptr){
	bufpl=bufpl_ptr;
}

void NetWorker::getBufferFromPool(){
	WorkerBuffer = bufpl->getPoolBuffer();
	struct timespec ts;
	uint64_t t_pool2worker= NetWorker::getNanoSecond(ts);
	left_stats.push_back(t_pool2worker-start_time);
}

bool NetWorker::pushBufferToQueue(){
	queue->push(WorkerBuffer);
	struct timespec ts;
	uint64_t t_worker2queue= NetWorker::getNanoSecond(ts);
	left_stats.push_back(t_worker2queue-start_time);
}

void NetWorker::getBufferFromQueue(){
	WorkerBuffer = queue->pop();
	struct timespec ts;
	uint64_t t_queue2worker= NetWorker::getNanoSecond(ts);
	right_stats.push_back(t_queue2worker-start_time);
}

bool NetWorker::pushBufferToPool(){
	bufpl->releasePoolBuffer(WorkerBuffer);
	struct timespec ts;
	uint64_t t_worker2pool= NetWorker::getNanoSecond(ts);
	right_stats.push_back(t_worker2pool-start_time);
}

uint64_t NetWorker::getBytesConsumed(){
    return stats->bytesConsumed;
}

uint64_t NetWorker::getBytesProduced(){
    return stats->bytesProduced;
}

system_stats* NetWorker::getStats(){
    return stats;
}
*/


uint64_t NetWorker::getNanoSecond(struct timespec tp){
	clock_gettime(CLOCK_MONOTONIC, &tp);
    return (1000000000) * (uint64_t)tp.tv_sec + tp.tv_nsec;
}


void NetWorker::allocStepStats(uint64_t size){
	step_stats.reserve(size*4);
}

std::vector<uint64_t> NetWorker::getStepStats(){
	return step_stats;
}


