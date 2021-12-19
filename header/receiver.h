#ifndef RECEIVER_H
#define RECEIVER_H

#include "network.h"

namespace LoadBalancing::Network::Receiver{

    class Receiver{
        private:
            int port;                   // receiver port
            int socketfd;               // receiver socket (connection entry point)
            int connection_socket;      // connection with sender
            char* efname;               // executable file name
            char* ofname;               // file where the executable output will be saved

        public:
            Receiver( int, char*, char* );
            virtual ~Receiver();

            int CreateTCPConnection();  // Open TCP connection      
            int AcceptSender();         // Establish connection with sender

            int ReceiveFile( );        // Receives executable from sender
            void SendOutput( int );     // Sends executable output to sender
    };

}

#endif // RECEIVER_H