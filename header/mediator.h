#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "network.h"

#include <queue>

namespace LoadBalancing::Network::Mediator{
    
    class Mediator{
        private:
            int port;
            int mediator_socket;

            std::queue<PeerID> receivers;
            std::queue<PeerID> senders;

        public:
            Mediator( int );
            virtual ~Mediator();

            int CreateTCPConnection();          // Open TCP connection     
            int AcceptPeer();                   // Connect to peer 
            int DistributeLoads();              // Pairs senders and receivers   

    };

}







#endif // MEDIATOR_H[]