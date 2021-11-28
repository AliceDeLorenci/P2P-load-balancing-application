#include "../header/server.h"
#include "../header/client.h"
#include <iostream>
#include <string.h>


int main ( int argc, char** argv ){

    if( argc > 1 ){
        if( strcmp( argv[1], "S" ) == 0 ){
            std::cout << "SERVER" << std::endl;
            Network::Server::Server servidor;
            servidor.CreateTCPConnection();
            servidor.AcceptClient();           
        }
        else{
            std::cout << "CLIENTE" << std::endl;
            Network::Client::Client client;
            client.CreateTCPConnection();
            client.SendMessage();
        }
    }
    return 0;
}


/*
#ifdef SERVER
int main (){
    std::cout << "SERVER" << std::endl;
    Network::Server::Server servidor;
    servidor.CreateTCPConnection();
    servidor.AcceptClient();
    //confiamos em você, código :)
    return 0;
}
#endif

#ifdef CLIENT
int main(){
    //confiamos em você, cliente, você pode rodar também :)
    std::cout << "CLIENTE" << std::endl;
    Network::Client::Client client;
    client.CreateTCPConnection();
    client.SendMessage();

    return 0;
}
#endif
*/










