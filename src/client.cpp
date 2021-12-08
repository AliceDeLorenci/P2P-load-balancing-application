#include "../header/client.h"
#include "../header/load_balancing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

namespace LoadBalancing::Network::Client{

    /**
     * Constructor
     */
    Client::Client( char* IP, int port ){
        this->IP = IP;
        this->port = port;
    }

    /**
     * Destructor
     */
    Client::~Client(){}

    /**
     * Establish connection with server
     */
    int Client::CreateTCPConnection(){

        // create client socket
        if ( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ExitWithMessage("Failed to create a client socket.");

        // server socket address
        struct sockaddr_in addr;
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons( port );
        addr.sin_addr.s_addr = inet_addr( IP );
        memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

        // connect to server
        if( connect( client_socket, (struct sockaddr*)&addr, sizeof(addr) ) < 0 )
            ExitWithMessage("Failed to connect to server.");

        return EXIT_SUCCESS;
    }

    /**
     * Send executable to server
     */
    void Client::SendFile( char* efname ){

        this->efname = efname;

        FILE *efd;

        if( !(efd = fopen(efname, "rb+")) )
            ExitWithMessage( "Couldn't open file." );

        // get executable size in bytes
        fseek( efd, 0, SEEK_END );
        int size = ftell( efd );  

        // send file size to server
        if( send( client_socket, &size, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Coundn't send executable file size to server." );

        // reposition file pointer
        fseek( efd, 0, SEEK_SET );

        // read file
        unsigned char *buffer;
        buffer = (unsigned char*)calloc( size, sizeof( unsigned char ) );
        fread( buffer, sizeof(unsigned char), size*sizeof( unsigned char ), efd );

        // send file to server
        if( send( client_socket, buffer, size, 0 ) < 0 )
            ExitWithMessage( "Coundn't send executable file to server." );

        fclose( efd );
    }  

    /**
     * Check whether there is any new output or whether the executable has finished running.
     * If there is new output, returns next block size, else, returns -1.
     */
    int Client::CheckReceiveOutput(){
        
        // next block size
        int size;
        if( recv( client_socket, &size, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Couldn't receive next block size." );

        return size;
    }

    /**
     * Constantly receive executable output from server.
     */
    void Client::ReceiveOutput(){

        int size = 0;

        while( (size = CheckReceiveOutput()) >= 0 ){    // while the process hasn't finished

            // receive new block
            unsigned char* buffer;
            buffer = (unsigned char*)calloc( size, sizeof( unsigned char ) );
            if( recv( client_socket, buffer, size*sizeof( unsigned char ), 0 ) < 0 )
                ExitWithMessage( "Counldn't receive new block." );

            // print to terminal
            std::cout << buffer << std::endl;
        }
    }
}
