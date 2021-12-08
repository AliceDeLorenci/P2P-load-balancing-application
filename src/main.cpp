#include "../header/load_balancing.h"

#include <iostream>
#include <string>

#include <unistd.h>
#include "spdlog/spdlog.h"

static void init() {

    #ifndef _DEBUG
        spdlog::set_level(spdlog::level::info);
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    #else
        // exhibits a greater number of runtime messages, including messages received throught the UDP and TCP connections
        spdlog::set_level(spdlog::level::trace);
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [thread %t] %v");
    #endif

}

#ifdef SERVER

int main ( int argc, char* argv[] ){

    init();

    LoadBalancing::LoadBalancing app( 1234 );
    app.RunApplication();

    return 0;
}

#elif CLIENT
int main(){

    init();

    LoadBalancing::LoadBalancing app( "127.0.0.1", 1234 );
    app.RunApplication();
    
    return 0;
}

#elif PEER
int main(){
    
    std::cout << "PEER" << std::endl;

    do{
        std::string s;
        std::cin >> s;
        
        if(s == "c"){
            LoadBalancing::Network::Peer::Peer peer;
            char fname[] = "teste";
            peer.AskForJob( fname );
        }
        else{
            LoadBalancing::Network::Peer::Peer peer;
            char fname[] = "copia";
            peer.ReceiveJobe( fname );
        }

    }while(s != "c" && s != "s");
    
    return 0;
}
#endif











