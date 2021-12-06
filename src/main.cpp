#include "../header/server.h"
#include "../header/client.h"
#include <iostream>
#include <string.h>


#ifdef SERVER
int main (){
    std::cout << "SERVER" << std::endl;
    Network::Server::Server servidor;
    servidor.CreateTCPConnection();
    servidor.AcceptClient();
    char* fname = "copia";
    servidor.ReceiveFile( fname );
    return 0;
}
#endif

#ifdef CLIENT
int main(){
    std::cout << "CLIENTE" << std::endl;
    Network::Client::Client client;
    client.CreateTCPConnection();
    char* fname = "teste";
    client.SendFile( fname );
    //client.SendMessage();

    return 0;
}
#endif











