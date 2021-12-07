#include "../header/server.h"
#include "../header/client.h"
#include <iostream>
#include <string.h>

// OBS: ler e enviar o executavel por blocos para evitar estouro de heap

// OBS: https://stackoverflow.com/questions/11189490/how-to-redirect-the-output-of-system-to-a-file
//      https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po

#ifdef SERVER
int main (){
    std::cout << "SERVER" << std::endl;

    char fname[] = "copia";

    LoadBalancing::Network::Server::Server servidor;
    servidor.CreateTCPConnection();
    servidor.AcceptClient();
    servidor.ReceiveFile( fname );


    return 0;
}
#endif

#ifdef CLIENT
int main(){
    std::cout << "CLIENTE" << std::endl;
    LoadBalancing::Network::Client::Client client;
    client.CreateTCPConnection();
    char fname[] = "teste";
    client.SendFile( fname );
    client.ReceiveOutput();
    //client.SendMessage();

    return 0;
}
#endif











