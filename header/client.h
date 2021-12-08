#ifndef CLIENT_H
#define CLIENT_H

#include "network.h"
#include <iostream>

namespace LoadBalancing::Network::Client{
    class Client{
        private:
            char* IP;       // server IP address
            int port;       // server port
            int client_socket;

            char* efname;   // executable file name (sent to server)

        public:
            Client( char*, int );
            virtual ~Client();

            int CreateTCPConnection();      // establish connection with server
            void SendFile( char* fname );   // sends executable to server
            void ReceiveOutput();           // receives executable output from server
            int CheckReceiveOutput();       // check whether there is new output
    };
}

#endif // CLIENT_H