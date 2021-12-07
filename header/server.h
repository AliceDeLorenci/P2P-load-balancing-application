#ifndef SERVER_H
#define SERVER_H

#include "network.h"

namespace LoadBalancing::Network::Server{

    class Server{
        private:
            int port = 1234;
            int server_socket;
            int connection_socket;
            char* fname;            // received executable name
            char* ofname = "output.txt";

        public:
            Server();
            int CreateTCPConnection();
            int AcceptClient();
            void ReceiveFile( char* fname );
            // void StartSendingOutput();
            void SendOutput( int );
    };

}

#endif // SERVER_H