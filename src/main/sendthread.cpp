#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>
#include <signal.h>

#include <iostream>
#include <memory>
#include <thread>
#include <fstream>

#include "MemWorker.h"
#include "NetWorker.h"


MemWorker* sourcer;
NetWorker* sender;

void print_usage() {
    printf("Usage: ./memthread -d duration in seconds -bs buffer size  -nb the number of buffers\n");
    printf("                   -qs buffer size  -nq the number of buffers\n");
}

void handle_sigpipe(int signum){
    sourcer->threadStop();
    sender->threadStop();

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
    BufferPool* send_pl =new BufferPool(plid);
    send_pl->allocBuffer(num_buf , buf_size);
    SafeQueue* send_sq =new SafeQueue(qid, qsize);
    Socket* send_sock=new Socket();

    sourcer=new MemWorker(0,"sourcer-01");
    sender=new NetWorker(2,"sender-01");
    sourcer->setQueue(send_sq);
    sourcer->setBufferPool(send_pl);
    sender->setQueue(send_sq);
    sender->setBufferPool(send_pl);
    sender->setSocket(send_sock);

    sourcer->allocStepStats(150000); //  9196035 in 10 sec->10000000
    sender->allocStepStats(150000);

    signal(SIGPIPE, handle_sigpipe);

    std::cout << "----------------------------"<<"\n";
    // TODO: catch the received signal SIGINT when ctrl-c
    sourcer->threadStart("source");
    sender->threadStart("send");
    sleep(duration);

    sourcer->threadStop();
    sender->threadStop();
    
    sourcer->threadJoin();
    sender->threadJoin();
    std::cout << "----------------------------"<<"\n";

    system_stats* stats1=sourcer->getStats();
    system_stats* stats2=sender->getStats();
    
    std::cout << "source bytes:" << stats1->bytesProduced/(1024*1024*1024) <<" GB \n";
    std::cout << "source packets:" << stats1->packetsProduced<<"\n";
    std::cout << "source overall time" << stats1->thread_time/1000 <<" micro sec\n";
    std::cout << "source writing time:"<< stats1->op_time/1000 <<" micro sec\n";

    uint64_t source_bits= 8*stats1->bytesProduced;
    double thread_rate=(double)source_bits/(double)stats1->thread_time;
    double op_rate=(double)source_bits/(double)stats1->op_time;
    std::cout << "source overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "source effective rate:" << op_rate <<" Gbps \n";
    

    std::cout << "send bytes:" << stats2->bytesConsumed/(1024*1024*1024) <<" GB \n";
    std::cout << "send packets:" << stats2->packetsConsumed <<"\n";
    std::cout << "send overall time" << stats2->thread_time/1000 <<" micro sec\n";
    std::cout << "send writing time:"<< stats2->op_time/1000 <<" micro sec\n";

    uint64_t sink_bits= 8*stats2->bytesConsumed;
    thread_rate=(double)sink_bits/(double)stats2->thread_time;
    op_rate=(double)sink_bits/(double)stats2->op_time;
    std::cout << "send overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "send effective rate:" << op_rate <<" Gbps \n";

    std::ofstream source_file, enqueue_file, dequeue_file, send_file; 
    std::ofstream  enpool_file, depool_file, bytes_file;

    bytes_file.open("send_bytes.txt");
    std::vector<uint64_t> send_list =stats2->bytes_stats;
    for(const uint64_t& e : send_list){
        bytes_file << e <<"\n";    
    }
    bytes_file.close();

    source_file.open("CDF_source.txt");
    enqueue_file.open("CDF_enqueue_s.txt");
    dequeue_file.open("CDF_dequeue_s.txt");
    send_file.open("CDF_send.txt");
    enpool_file.open("CDF_enpool_s.txt");
    depool_file.open("CDF_depool_s.txt");

    uint64_t len=0;
    std::vector<uint64_t> left_list=sourcer->getStepStats();
    std::vector<uint64_t> right_list=sender->getStepStats();
    if(left_list.size() < right_list.size()){
        len = left_list.size();
    }
    else{
        len= right_list.size();
    }
    std::cout << left_list.size() <<"\n";
    std::cout << right_list.size() <<"\n";
    
    uint64_t source_time=0;
    uint64_t enqueue_time=0;
    uint64_t dequeue_time=0;
    uint64_t send_time=0;
    uint64_t enpool_time=0;
    uint64_t depool_time=0;
    //TODO !!!! calculate the time of each step.
    for(uint64_t i = 0; i < len - 8 ; i=i+4){
        uint64_t t_pool2worker  = left_list.at(i);
        uint64_t t_worker_source = left_list.at(i+1);
        uint64_t t_worker_donesource = left_list.at(i+2);
        uint64_t t_worker2queue     = left_list.at(i+3);

        //uint64_t t_next_pool2worker  = left_list.at(i+4);    

        uint64_t t_queue2worker  = right_list.at(i);
        uint64_t t_worker_send = right_list.at(i+1);
        uint64_t t_worker_donesend = right_list.at(i+2);
        uint64_t t_worker2pool = right_list.at(i+3);

        /*uint64_t t_source  = t_worker2queue - t_pool2worker;
        uint64_t t_queue   = t_queue2worker - t_worker2queue;
        uint64_t t_send    = t_worker2pool  - t_queue2worker;
        uint64_t t_pool    = t_next_pool2worker - t_worker2pool;*/

        uint64_t op_depool  = t_worker_source     - t_pool2worker;
        uint64_t op_source    = t_worker_donesource - t_worker_source;
        uint64_t op_enqueue = t_worker2queue    - t_worker_donesource;

        uint64_t op_dequeue = t_worker_send     - t_queue2worker;
        uint64_t op_send    = t_worker_donesend - t_worker_send;
        uint64_t op_enpool  = t_worker2pool     - t_worker_donesend;

        source_file << op_source<< "\n";
        source_time  = source_time + op_source;

        enqueue_file << op_enqueue << "\n";
        enqueue_time = enqueue_time + op_enqueue;

        dequeue_file << op_dequeue<< "\n";
        dequeue_time = dequeue_time + op_dequeue;

        send_file << op_send << "\n";
        send_time  = send_time + op_send;

        enpool_file << op_enpool <<"\n";
        enpool_time  = enpool_time + op_enpool;

        depool_file << op_depool <<"\n";
        depool_time  = depool_time + op_depool;
    }

    uint64_t iter=len/4;
    double mean_source_time = (double) source_time/(double) iter;
    double mean_enqueue_time= (double) enqueue_time/(double) iter;
    double mean_dequeue_time= (double) dequeue_time/(double) iter;
    double mean_send_time = (double) send_time/(double) iter;
    double mean_enpool_time = (double) enpool_time/(double) iter;
    double mean_depool_time = (double) depool_time/(double) iter;

    std::cout << "mean source " << mean_source_time/1000 <<" micro sec\n";
    std::cout << "mean enqueue " << mean_enqueue_time/1000 <<" micro sec\n";
    std::cout << "mean dequeue " << mean_dequeue_time/1000 <<" micro sec\n";
    std::cout << "mean send " << mean_send_time/1000 <<" micro sec\n";
    std::cout << "mean enpool " << mean_enpool_time/1000 <<" micro sec\n";
    std::cout << "mean depool " << mean_depool_time/1000 <<" micro sec\n";
    
    source_file.close();
    enqueue_file.close();
    dequeue_file.close();
    send_file.close();
    enpool_file.close();
    depool_file.close();

	return 0;
}
