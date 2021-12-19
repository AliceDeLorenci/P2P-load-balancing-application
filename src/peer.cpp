#include "../header/peer.h" 
#include "../header/load_balancing.h"
#include <iostream>
#include <random>

namespace LoadBalancing::Network::Peer{

    Peer::Peer( char* mediator_IP, int mediator_port ){
        this->mediator_IP = mediator_IP;
        this->mediator_port = mediator_port;
    }

    Peer::~Peer(){}

    /**
     * Get peer type
     */
    int Peer::GetPeerType(){ return peer_type; }

    /**
     * Set peer type
     */
    int Peer::SetPeerType( int type ){ 

        if( (type != RECEIVER) && (type != SENDER) ){
            return EXIT_FAILURE;
        }

        peer_type = type;
        return EXIT_SUCCESS;

    }

    /**
     * Connects to mediator 
     */
    int Peer::ConnectToMediator(){

        // establish TCP connection with mediator
        CreateConnection();     
                        
        if( send( mediator_socket, &peer_type, sizeof( int ), 0 ) < 0 )   // inform peer type to mediator
            ExitWithMessage( "Coundn't send peer type to mediator." );

        if( peer_type == RECEIVER ){
            
            // generate random port number
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_int_distribution<int> distr( MIN_PORT, MAX_PORT );

            // instantiate RECEIVER
            int receiver_port = distr(eng);
            receiver = std::make_unique<Receiver::Receiver> ( receiver_port, const_cast<char*>(EFNAME), const_cast<char*>(OFNAME) );  
            receiver->CreateTCPConnection();

            spdlog::info( "Peer receiver port: {}", receiver_port );

            // inform peer's receiver port to mediator
            if( send( mediator_socket, &receiver_port, sizeof( int ), 0 ) < 0 )
                ExitWithMessage( "Coundn't send peer's port to mediator." );

        }else if( peer_type == SENDER ){
            
            spdlog::info( "Waiting for receiver..." );

            // wait for a RECEIVER's address
            struct ReceiverID receiver;
            if( recv( mediator_socket, &receiver, sizeof( struct ReceiverID ), 0 ) < 0 )
                ExitWithMessage( "Error receiving receiver information." );

            spdlog::info( "RECEIVER: [IP:{}, port:{}]", receiver.ipstr, receiver.port );

            // instantiate SENDER
            sender = std::make_unique<Sender::Sender> ( receiver.ipstr, receiver.port );
        }

        return EXIT_SUCCESS;
    }

    /**
     * Establishes TCP connection with mediator
     */
    int Peer::CreateConnection(){

        // create sender socket
        if ( (mediator_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ExitWithMessage("Failed to create a peer socket.");

        // receiver socket address
        struct sockaddr_in addr;
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons( mediator_port );
        addr.sin_addr.s_addr = inet_addr( mediator_IP );
        memset( &addr.sin_zero,0,sizeof(addr.sin_zero) );

        // connect to receiver
        if( connect( mediator_socket, (struct sockaddr*)&addr, sizeof(addr) ) < 0 )
            ExitWithMessage("Failed to connect to mediator.");

        return EXIT_SUCCESS;
    }

    /**
     * A RECEIVER peer gets a load.
     */
    int Peer::GetLoad( ){

        if( peer_type != RECEIVER ){
            return EXIT_FAILURE;
        }

        receiver->AcceptSender();         // establish connection with sender
        receiver->ReceiveFile();          // receive executable

        Executable executable( const_cast<char*>(EFNAME), const_cast<char*>(OFNAME) );
        int epid = executable.Execute();    // run executable
        spdlog::info( "Executing..." );

        receiver->SendOutput( epid );      // send output to sender

        return EXIT_SUCCESS;
    }

    /**
     * A SENDER peer sends a load.
     */
    int Peer::SendLoad( char* efname ){

        if( peer_type != SENDER ){
            return EXIT_FAILURE;
        }

        sender->CreateTCPConnection();   // establish connection with RECEIVER
        sender->SendFile( efname );      // send executable file name
        sender->ReceiveOutput();         // receive executable output

        return EXIT_SUCCESS;
    }

    int Peer::SendMessage( int socket, void* msg, int msg_type_sizeof, int size ){

        if( send( socket, msg, msg_type_sizeof, size ) < 0 )
            return EXIT_FAILURE;
        return EXIT_SUCCESS;

    }

}