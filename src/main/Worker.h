#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

//Ref: https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
class Worker{
public:
    //Explicitly using the default constructor to
    //underline the fact that it does get called 
    Worker() : the_thread() {}
    virtual ~Worker(){
        /*
        stop_thread = true;
        if(the_thread.joinable()) the_thread.join();
        */
    }
    virtual void Start(){
        /*
        the_thread = std::thread(&MyClass::mainLoop,this);
        */
    }
    virtual void mainLoop(){
        /*
        while(stop_thread){
            // Do something useful, e.g:
            /*std::cout << "while loop "<< "\n";
            std::this_thread::sleep_for( std::chrono::seconds(1) );
            stop_thread=true;
        }*/
    }

protected:
    std::thread the_thread;
    bool stop_thread = false;    
};

#endif //WORKER_H
