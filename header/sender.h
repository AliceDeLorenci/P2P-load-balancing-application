#ifndef SENDER_H
#define SENDER_H

#include "network.h"
#include <iostream>

namespace LoadBalancing::Network::Sender{
    class Sender{
        private:
            char* IP;           // receiver IP address
            int port;           // receiver port
            int socketfd;       // connection to receiver

            char* efname;   // executable file name (sent to receiver)

        public:
            Sender( char*, int );
            virtual ~Sender();

            int CreateTCPConnection();      // establish connection with receiver
            void SendFile( char* );         // sends executable to receiver
            void ReceiveOutput();           // get executable output from receiver
            int CheckReceiveOutput();       // check whether there is new output
    };
}

#endif // SENDER_H