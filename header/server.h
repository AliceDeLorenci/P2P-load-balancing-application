#ifndef SERVER_H
#define SERVER_H

#include "network.h"

namespace LoadBalancing::Network::Server{

    class Server{
        private:
            int port;                   // server port
            int server_socket;          // server socket (connection entry point)
            int connection_socket;      // connection with client
            char* efname;               // executable file name
            char* ofname;               // file where the executable output will be saved

        public:
            Server( int, char*, char* );
            virtual ~Server();

            int CreateTCPConnection();  // Open TCP connection      
            int AcceptClient();         // Establish connection with client

            int ReceiveFile( );        // Receives executable from client
            void SendOutput( int );     // Sends executable output to client
    };

}

#endif // SERVER_H