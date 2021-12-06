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
    //confiamos em você, código :)
    return 0;
}
#endif

#ifdef CLIENT
int main(){
    //confiamos em você, cliente, você pode rodar também :)
    std::cout << "CLIENTE" << std::endl;
    Network::Client::Client client;
    client.SendFile();
    //client.CreateTCPConnection();
    //client.SendMessage();

    return 0;
}
#endif











