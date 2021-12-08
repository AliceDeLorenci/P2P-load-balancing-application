#ifndef PEER_H
#define PEER_H

#include "../header/network.h"
#include "../header/server.h"
#include "../header/client.h"

namespace LoadBalancing::Network::Peer{

    class Peer{
        private:
            Server::Server servidor;
            Client::Client cliente;

        public:
            Peer();
            virtual ~Peer();
            void AskForJob(char * fname);
            void ReceiveJobe(char * fname);
    };

}

#endif // PEER_H