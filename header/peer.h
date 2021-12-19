#ifndef PEER_H
#define PEER_H

#include "../header/network.h"
#include "../header/receiver.h"
#include "../header/sender.h"

namespace LoadBalancing::Network::Peer{

    // port range from which the receiver peer port will be randomly chosen
    constexpr int MIN_PORT = 1024;
    constexpr int MAX_PORT = 65535;

    class Peer{
        private:
            char* mediator_IP;      // mediator IP address
            int mediator_port;      // mediator port
            int mediator_socket;    // socket used for connection with mediator

            int peer_type;          // SENDER / RECEIVER

            std::unique_ptr<Receiver::Receiver> receiver; // instantiated when the peer is a RECEIVER
            std::unique_ptr<Sender::Sender> sender;       // instantiated when the peer is a SENDER

        public:
            Peer( char*, int );
            virtual ~Peer();

            int ConnectToMediator();                    // peer connects to mediator
            int CreateConnection();                     // establishes TCP connection with mediator
            
            int GetLoad( );                             // a RECEIVER type peer gets a load
            int SendLoad( char* );                      // a SENDER type peer sends a load

            int SendMessage( int, void*, int, int );    // sends an arbitrary message   

            int GetPeerType();      
            int SetPeerType( int );


            
            //void AskForJob(char * fname);
            //void ReceiveJobe(char * fname);
    };

}

#endif // PEER_H