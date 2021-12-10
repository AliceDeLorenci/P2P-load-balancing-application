#include "../header/mediator.h" 
#include "../header/load_balancing.h" 
#include "../header/network.h"
#include <iostream>

namespace LoadBalancing::Network::Mediator{

    /**
     * Constructor 
     */
    Mediator::Mediator( int port ){
        this->port = port;
    }

    /**
     * Destructor 
     */
    Mediator::~Mediator(){}

    /**
     * Open TCP connection
     */
    int Mediator::CreateTCPConnection(){

        // allocate a socket for the server
        if ( (mediator_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
            ExitWithMessage( "Failed to create TCP server socket." );

        // attempt to stop port from blocking after repeated executions
        int allow = 1;
        if ( setsockopt(mediator_socket, SOL_SOCKET, SO_REUSEADDR, &allow, sizeof(int)) < 0 )
            ExitWithMessage( "Failed to configure TCP server socket." );

        // assign a port and IP address to the socket
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons( port );
        addr.sin_addr.s_addr = INADDR_ANY;          // local host
        memset( &addr.sin_zero, 0, sizeof(addr.sin_zero) );

        if ( bind( mediator_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0 )
            ExitWithMessage( "Failed to bind server socket." );

        // start listening to clients
        if ( listen( mediator_socket, 1 ) == -1) 
            ExitWithMessage( "Failed to listen for clients." );

        return EXIT_SUCCESS;

    }

    /**
     * Accept client
     */
    int Mediator::AcceptClient(){

        spdlog::info( "Waiting for peer..." );

        struct PeerID peer;

        socklen_t len = sizeof( peer.addr );

        // accept client
        if( ( peer.connection_socket = accept( mediator_socket, (struct sockaddr*)&peer.addr, &len ) ) < 0 )
            ExitWithMessage("Failed to accept client.");

        // deal with both IPv4 and IPv6:
        if (peer.addr.ss_family == AF_INET) {

            struct sockaddr_in *s = (struct sockaddr_in *)&peer.addr;
            peer.port = ntohs( s->sin_port );
            inet_ntop( AF_INET, &s->sin_addr, peer.ipstr, sizeof( peer.ipstr ) );

        } else { // AF_INET6

            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&peer.addr;
            peer.port = ntohs( s->sin6_port );
            inet_ntop( AF_INET6, &s->sin6_addr, peer.ipstr, sizeof( peer.ipstr ) );

        }

        spdlog::info( "Peer IP address: {}", peer.ipstr );

        // receive peer type
        if( recv( peer.connection_socket, &peer.type, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Error receiving peer type." );

        spdlog::info( "Peer type: {}", peer.type == RECEIVER ? "receiver" : "sender" );

        if( peer.type == RECEIVER ){

            // receive peer's server socket port
            if( recv( peer.connection_socket, &peer.server_port, sizeof( int ), 0 ) < 0 )
                ExitWithMessage( "Error receiving peer server port." );
            
            spdlog::info( "Peer server port: {}", peer.server_port );

            receivers.push( peer );

            // the mediator won't need to communicate further with the receiver
            close( peer.connection_socket );

        }else if( peer.type == SENDER ){

            senders.push( peer );

        }else{
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    /**
     * Pair senders and receivers
     */
    int Mediator::DistributeLoads(){

        if( (!receivers.empty()) && (!senders.empty()) ){

            struct PeerID receiver, sender;

            receiver = receivers.front();
            sender = senders.front();

            struct ServerID receiver_addr;
            receiver_addr.port = receiver.server_port;
            strcpy( receiver_addr.ipstr, receiver.ipstr );

            if( send( sender.connection_socket, &receiver_addr, sizeof( struct ServerID ), 0 ) < 0 )
                ExitWithMessage( "Coundn't pair sender and receiver." );

            // close connection with sender peer
            close( sender.connection_socket );

            receivers.pop();
            senders.pop();

        }
        return EXIT_SUCCESS;
    }

    /**
     * Accept client
     */
    int Mediator::StartAcceptingClients(){
        
        return EXIT_SUCCESS;
    }

}