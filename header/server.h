#ifndef SERVER_H
#define SERVER_H

#include "network.h"

namespace Network::Server{

    class Server{
        private:
            int port = 1234;
            int server_socket;
            int connection_socket;

        public:
            Server();
            int CreateTCPConnection();
            int AcceptClient();
    };

}

#endif // SERVER_H