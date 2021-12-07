#ifndef CLIENT_H
#define CLIENT_H

#include "network.h"
#include <iostream>

namespace LoadBalancing::Network::Client{
    class Client{
        private:
            int port = 1234;
            int client_socket;

        public:
            Client();
            int CreateTCPConnection();
            int SendMessage();
            void SendFile( char* fname );
            void ReceiveOutput();
            int CheckReceiveOutput();
    };
}

#endif // CLIENT_H