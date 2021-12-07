#include <iostream>
#include <chrono>
#include <thread>

using namespace std::this_thread; 
using namespace std::chrono;

int main(){

    for( int i=0; i<10; i++ ){
        std::cout << "Hello world!" << std::endl;
        sleep_for( nanoseconds(10) );
    }

}