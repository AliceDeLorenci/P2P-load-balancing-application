#include "../header/peer.h" 
#include "../header/load_balancing.h"
#include <iostream>

namespace LoadBalancing::Network::Peer{

    Peer::Peer(){}

    Peer::~Peer(){}

    void Peer::AskForJob(char * fname){

        std::cout << "Asking for a job" << std::endl;
        LoadBalancing::Network::Client::Client client;
        client.CreateTCPConnection();
        client.SendFile( fname );
        client.ReceiveOutput();

    };

    void Peer::ReceiveJobe(char * fname){

        servidor.CreateTCPConnection();
        servidor.AcceptClient();
        servidor.ReceiveFile( fname );

    }

}