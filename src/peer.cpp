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

        // inform peer type to mediator
        if( send( mediator_socket, &peer_type, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Coundn't send peer type to mediator." );

        if( peer_type == RECEIVER ){
            
            // generate random port number
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_int_distribution<int> distr( MIN_PORT, MAX_PORT );

            int server_port = distr(eng);
            server = std::make_unique<Server::Server> ( server_port, const_cast<char*>(EFNAME), const_cast<char*>(OFNAME) );    // change this (EFNAME, OFNAME)
            server->CreateTCPConnection();

            spdlog::info( "Peer server port: {}", server_port );

            // inform peer's server port to mediator
            if( send( mediator_socket, &server_port, sizeof( int ), 0 ) < 0 )
                ExitWithMessage( "Coundn't send peer's server port to mediator." );
            
            // accept client ...

        }else if( peer_type == SENDER ){
            
            // wait for a RECEIVER's address
            struct ServerID receiver;
            if( recv( mediator_socket, &receiver, sizeof( struct ServerID ), 0 ) < 0 )
                ExitWithMessage( "Error receiving receiver information." );

            spdlog::info( "Receiver peer IP and port: {}, {}", receiver.ipstr, receiver.port );

            client = std::make_unique<Client::Client> ( receiver.ipstr, receiver.port );

            // create connection ...
        }

        return EXIT_SUCCESS;
    }

    /**
     * Establishes TCP connection with mediator
     */
    int Peer::CreateConnection(){

        // create client socket
        if ( (mediator_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ExitWithMessage("Failed to create a peer socket.");

        // server socket address
        struct sockaddr_in addr;
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons( mediator_port );
        addr.sin_addr.s_addr = inet_addr( mediator_IP );
        memset( &addr.sin_zero,0,sizeof(addr.sin_zero) );

        // connect to server
        if( connect( mediator_socket, (struct sockaddr*)&addr, sizeof(addr) ) < 0 )
            ExitWithMessage("Failed to connect to mediator.");

        return EXIT_SUCCESS;
    }

    /**
     * A RECEIVER type peer gets a load.
     */
    int Peer::GetLoad( ){

        if( peer_type != RECEIVER ){
            return EXIT_FAILURE;
        }

        server->AcceptClient();          // establish connection with client
        server->ReceiveFile();           // receive executable

        Executable executable( const_cast<char*>(EFNAME), const_cast<char*>(OFNAME) );
        int epid = executable.Execute();// run executable
        server->SendOutput( epid );      // send output to client (run on a thread??)

        return EXIT_SUCCESS;
    }

    /**
     * A SENDER type peer sends a load.
     */
    int Peer::SendLoad( char* efname ){

        if( peer_type != SENDER ){
            return EXIT_FAILURE;
        }

        client->CreateTCPConnection();   // establish connection with RECEIVER
        client->SendFile( efname );      // send executable file name
        client->ReceiveOutput();         // receive executable output

        return EXIT_SUCCESS;
    }

    int Peer::SendMessage( int socket, void* msg, int msg_type_sizeof, int size ){

        if( send( socket, msg, msg_type_sizeof, size ) < 0 )
            return EXIT_FAILURE;
        return EXIT_SUCCESS;

    }

    // void Peer::AskForJob(char * fname){

    //     std::cout << "Asking for a job" << std::endl;
    //     LoadBalancing::Network::Client::Client client;
    //     client.CreateTCPConnection();
    //     client.SendFile( fname );
    //     client.ReceiveOutput();

    // };

    // void Peer::ReceiveJobe(char * fname){

    //     servidor.CreateTCPConnection();
    //     servidor.AcceptClient();
    //     servidor.ReceiveFile( fname );

    // }

}