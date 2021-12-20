#include <iostream>
#include <chrono>
#include <thread>

using namespace std::this_thread; 
using namespace std::chrono;

/**
 * Program designed to test load balancing over P2P networks.
 */
int main(){

    // Prints "Hello world" in intervals of 1 second.
    for( int i=0; i<10; i++ ){
        std::cout << "Hello world!" << std::endl;
        sleep_for( milliseconds(1000) );
    }

}