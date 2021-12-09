#ifndef PEER_H
#define PEER_H

#include "../header/network.h"
#include "../header/server.h"
#include "../header/client.h"

namespace LoadBalancing::Network::Peer{

    constexpr int MIN_PORT = 1000;
    constexpr int MAX_PORT = 2000;

    class Peer{
        private:
            char* mediator_IP;      // mediator IP address
            int mediator_port;      // mediator port
            int mediator_socket;    // socket used for connection with mediator

            int peer_type;          // SENDER / RECEIVER

            std::unique_ptr<Server::Server> server;
            std::unique_ptr<Client::Client> client;

        public:
            Peer( char*, int );
            virtual ~Peer();

            int ConnectToMediator();                    // peer connects to mediator
            int CreateConnection();                     // establishes connection with mediator
            int SendMessage( int, void*, int, int );    // sends an arbitrary message   

            int GetPeerType();      
            int SetPeerType( int );


            
            //void AskForJob(char * fname);
            //void ReceiveJobe(char * fname);
    };

}

#endif // PEER_H