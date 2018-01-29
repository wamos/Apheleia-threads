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
        stop_thread = true;
        if(the_thread.joinable()) the_thread.join();
    }
    virtual void Start(){
        // This will start the thread. Notice move semantics!
        the_thread = std::thread(&Worker::mainLoop,this);
    }
    virtual void mainLoop(){
        //auto start = std::chrono::system_clock::now();
        while(stop_thread){
            // Do something useful, e.g:
            std::cout << "while loop "<< "\n";
            std::this_thread::sleep_for( std::chrono::seconds(1) );
            stop_thread=true;
        }
        /*auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "elapsed time: " << elapsed_seconds.count() << "\n";*/
    }

protected:
    std::thread the_thread;
    bool stop_thread = false;    
};

int main( int argc, char** argv )
{
    Worker instance;
    instance.Start();
    return 0;
}
