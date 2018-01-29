#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>

#include <iostream>
#include <memory>
#include <thread>

//#include "mythread.h"
#include "MemWorker.h"
#include "NetWorker.h"

void print_usage() {
    printf("Usage: ./memthread -d duration in seconds -bs buffer size  -nb the number of buffers\n");
    printf("                   -qs buffer size  -nq the number of buffers\n");
}


int main(int argc, char *argv[]) {
    char *p;
    uint64_t duration;
    uint64_t buf_size;
    uint64_t num_buf;
    uint64_t qsize;
    int option = 0;

    //Specifying the expected options
    while ((option = getopt(argc, argv,"d:b:n:q:")) != -1) {
        switch (option) {
            case 'd' : 
                duration = strtol(optarg,&p, 10);//atoi(optarg);
                std::cout << "duration: " << duration<<"\n";
                break;
            case 'b': 
                buf_size = strtol(optarg,&p, 10);
                std::cout << "buffer size: " << buf_size<<"\n";
                break;
            case 'n' : 
                num_buf = strtol(optarg,&p, 10); 
                std::cout << "number of buffers: " << num_buf<<"\n";
                break;
            case 'q' : 
                qsize = strtol(optarg,&p, 10); 
                std::cout << "queue size: " << qsize<<"\n";
                break;
            default: 
                print_usage(); 
                exit(EXIT_FAILURE);
        }
    }
    /*if (duration < 0 || buf_size < 0 || num_buf < 0 || qsize < 0) {
        print_usage();
        exit(EXIT_FAILURE);
    }*/

    uint64_t qid=1000;
    uint64_t plid=100;

    //uint64_t qsize=128;
    //uint64_t num_buf=32;
    //uint64_t buf_size=1024;

    BufferPool* recv_pl =new BufferPool(plid+1);
    recv_pl->allocBuffer(num_buf , buf_size);
    SafeQueue* recv_sq =new SafeQueue(qid+1, qsize);
    Socket* recv_sock=new Socket();

    MemWorker* sinker=new MemWorker(1,"sinker-01");
    NetWorker* recver=new NetWorker(3,"recver-01");

    sinker->setQueue(recv_sq);
    sinker->setBufferPool(recv_pl);
    recver->setQueue(recv_sq);
    recver->setBufferPool(recv_pl);
    recver->setSocket(recv_sock);

    recver->allocCycleStats(10000000, "left"); //  9196035 in 10 sec->10000000
    sinker->allocCycleStats( 10000000, "right");

    std::cout << "----------------------------"<<"\n";
    // TODO: catch the received signal SIGINT when ctrl-c
    sinker->threadStart("sink");
    recver->threadStart("recv");
    sleep(duration);

    sinker->threadStop();
    recver->threadStop();
    
    sinker->threadJoin();
    recver->threadJoin();

    system_stats* stats1=recver->getStats();
    system_stats* stats2=sinker->getStats();

    std::cout << "recv bytes:" << stats1->bytesProduced/(1024*1024) <<" MB \n";
    std::cout << "recv packets:" << stats1->packetsProduced<<"\n";
    std::cout << "recv overall time" << stats1->thread_time/1000 <<" micro sec\n";
    std::cout << "recv writing time:"<< stats1->op_time/1000 <<" micro sec\n";

    uint64_t source_bits= 8*stats1->bytesProduced;
    double thread_rate=(double)source_bits/(double)stats1->thread_time;
    double op_rate=(double)source_bits/(double)stats1->op_time;
    std::cout << "recv overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "recv effective rate:" << op_rate <<" Gbps \n";

    std::cout << "sink bytes:" << stats2->bytesConsumed/(1024*1024*1024) <<" GB \n";
    std::cout << "sink packets:" << stats2->packetsConsumed <<"\n";
    std::cout << "sink overall time" << stats2->thread_time/1000 <<" micro sec\n";
    std::cout << "sink writing time:"<< stats2->op_time/1000 <<" micro sec\n";

    uint64_t sink_bits= 8*stats2->bytesConsumed;
    thread_rate=(double)sink_bits/(double)stats1->thread_time;
    op_rate=(double)sink_bits/(double)stats1->op_time;
    std::cout << "sink overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "sink effective rate:" << op_rate <<" Gbps \n";

    uint64_t len;
    std::vector<uint64_t> left_list=recver->getLeftStats();
    std::vector<uint64_t> right_list=sinker->getRightStats();
    if(left_list.size() < right_list.size()){
        len = left_list.size();
    }
    else{
        len= right_list.size();
    }
    //std::cout << left_list.size() <<"\n";
    //std::cout << right_list.size() <<"\n";
    
    uint64_t recv_time=0;
    uint64_t queue_time=0;
    uint64_t sink_time=0;
    uint64_t pool_time=0;
    //TODO !!!! calculate the time of each step.
    for(uint64_t i = 0; i < len - 10 ; i++){
        uint64_t t_pool2worker  = left_list.at(i);
        uint64_t t_worker2queue = left_list.at(i+1);
        uint64_t t_queue2worker  = right_list.at(i);
        uint64_t t_worker2pool = right_list.at(i+1);
        uint64_t next_t_pool2worker = left_list.at(i+2);

        recv_time  = recv_time + t_worker2queue - t_pool2worker;
        queue_time = queue_time + t_queue2worker - t_worker2queue;
        sink_time  = sink_time + t_worker2pool - t_queue2worker;
        pool_time  = pool_time + next_t_pool2worker - t_worker2pool;
    }

    uint64_t iter=len/2;
    double mean_recv_time = (double) recv_time/(double) iter;
    double mean_queue_time= (double) queue_time/(double) iter;
    double mean_sink_time = (double) sink_time/(double) iter;
    double mean_pool_time = (double) pool_time/(double) iter;

    std::cout << "mean recv " << mean_recv_time/1000 <<" micro sec\n";
    std::cout << "mean queue " << mean_queue_time/1000 <<" micro sec\n";
    std::cout << "mean sink " << mean_sink_time/1000 <<" micro sec\n";
    std::cout << "mean pool " << mean_pool_time/1000 <<" micro sec\n";
    

	return 0;
}
