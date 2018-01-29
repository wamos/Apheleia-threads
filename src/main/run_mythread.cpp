#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>

#include <iostream>
#include <memory>
#include <thread>

//#include "mythread.h"
//#include "MemWorker.h"
#include "NetWorker.h"

void print_usage() {
    printf("Usage: ./memthread -d duration in seconds -bs buffer size  -nb the number of buffers\n");
    printf("                   -qs buffer size  -nq the number of buffers\n");
}

/*MemWorker* sourcer, MemWorker* sinker,*/
void mainfunction(NetWorker* sender, NetWorker* recver, uint64_t duration){

    sender->threadStart("send");
    recver->threadStart("recv");

    std::cout << "----------------------------"<<"\n";
    // TODO: catch the received signal SIGINT when ctrl-c
    sleep(duration);
    sender->threadStop();
    recver->threadStop();
    sender->threadJoin();
    recver->threadJoin();


    /*
    std::cout << "----------------------------"<<"\n";
    sourcer->threadStart("source");
    sinker->threadStart("sink");
    // TODO: catch the received signal SIGINT when ctrl-c
    sleep(duration);
    sourcer->threadStop();
    sinker->threadStop();
    sourcer->threadJoin();
    sinker->threadJoin();
    system_stats* stats1=sourcer->getStats();
    system_stats* stats2=sinker->getStats();

    std::cout << "source bytes:" << stats1->bytesProduced/(1024*1024*1024) <<" GB \n";
    std::cout << "source packets:" << stats1->packetsProduced<<"\n";
    std::cout << "source overall time" << stats1->thread_time/1000 <<" micro sec\n";
    std::cout << "source writing time:"<< stats1->op_time/1000 <<" micro sec\n";

    uint64_t source_bits= 8*stats1->bytesProduced;
    double thread_rate=(double)source_bits/(double)stats1->thread_time;
    double op_rate=(double)source_bits/(double)stats1->op_time;
    std::cout << "source overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "source effective rate:" << op_rate <<" Gbps \n";

    std::cout << "sink bytes:" << stats2->bytesConsumed/(1024*1024*1024) <<" GB \n";
    std::cout << "sink packets:" << stats2->packetsConsumed <<"\n";
    std::cout << "sink overall time" << stats2->thread_time/1000 <<" micro sec\n";
    std::cout << "sink writing time:"<< stats2->op_time/1000 <<" micro sec\n";

    uint64_t sink_bits= 8*stats2->bytesConsumed;
    thread_rate=(double)sink_bits/(double)stats1->thread_time;
    op_rate=(double)sink_bits/(double)stats1->op_time;
    std::cout << "sink overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "sink effective rate:" << op_rate <<" Gbps \n";
    */

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

    BufferPool* recv_pl =new BufferPool(plid+1);
    recv_pl->allocBuffer(num_buf , buf_size);
    SafeQueue* recv_sq =new SafeQueue(qid+1, qsize);

    Socket* send_sock=new Socket();
    Socket* recv_sock=new Socket();

    //MemWorker* sourcer=new MemWorker(0,"sourcer-01");
    NetWorker* sender=new NetWorker(2,"sender-01");
    //sourcer->setQueue(send_sq);
    //sourcer->setBufferPool(send_pl);
    sender->setQueue(send_sq);
    sender->setBufferPool(send_pl);
    sender->setSocket(send_sock);

    
    //MemWorker* sinker=new MemWorker(1,"sinker-01");
    NetWorker* recver=new NetWorker(3,"recver-01");
    //sinker->setQueue(recv_sq);
    //sinker->setBufferPool(recv_pl);
    recver->setQueue(recv_sq);
    recver->setBufferPool(recv_pl);
    recver->setSocket(recv_sock);

    //sender->threadStart("send");
    recver->threadStart("recv");

    std::cout << "----------------------------"<<"\n";
    // TODO: catch the received signal SIGINT when ctrl-c
    sender->threadStart("send");
    sleep(duration);

    sender->threadStop();
    recver->threadStop();
    
    sender->threadJoin();
    recver->threadJoin();

    system_stats* stats1=sender->getStats();
    system_stats* stats2=recver->getStats();


    std::cout << "source bytes:" << stats1->bytesProduced/(1024*1024*1024) <<" GB \n";
    std::cout << "source packets:" << stats1->packetsProduced<<"\n";
    std::cout << "source overall time" << stats1->thread_time/1000 <<" micro sec\n";
    std::cout << "source writing time:"<< stats1->op_time/1000 <<" micro sec\n";

    uint64_t source_bits= 8*stats1->bytesProduced;
    double thread_rate=(double)source_bits/(double)stats1->thread_time;
    double op_rate=(double)source_bits/(double)stats1->op_time;
    std::cout << "source overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "source effective rate:" << op_rate <<" Gbps \n";

    std::cout << "sink bytes:" << stats2->bytesConsumed/(1024*1024*1024) <<" GB \n";
    std::cout << "sink packets:" << stats2->packetsConsumed <<"\n";
    std::cout << "sink overall time" << stats2->thread_time/1000 <<" micro sec\n";
    std::cout << "sink writing time:"<< stats2->op_time/1000 <<" micro sec\n";

    uint64_t sink_bits= 8*stats2->bytesConsumed;
    thread_rate=(double)sink_bits/(double)stats1->thread_time;
    op_rate=(double)sink_bits/(double)stats1->op_time;
    std::cout << "sink overall   rate:" << thread_rate <<" Gbps \n";
    std::cout << "sink effective rate:" << op_rate <<" Gbps \n";



    /*sourcer, sinker,*/ 
    //mainfunction(sender, recver, duration);

    /*
	system_stats* source_stats=sourcer>Start("source");

    system_stats* sink_stats=sinker>Start("sink");*/

	return 0;
}
