#ifndef CLIENT_H
#define CLIENT_H

#include "network.h"

namespace Network::Client{
    class Client{
        private:
            int port = 1234;
            int client_socket;

        public:
            Client();
            int CreateTCPConnection();
            int SendMessage();
            void SendFile( char* fname );
    };
}

#endif // CLIENT_H