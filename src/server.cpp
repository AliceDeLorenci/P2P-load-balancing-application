#include "../header/server.h" 
#include "../header/load_balancing.h"
#include <iostream>

namespace LoadBalancing::Network::Server{

    /**
     * Constructor 
     */
    Server::Server( int port, char* efname, char* ofname ){
        this->port = port;      // server port
        this->efname = efname;  // name that will be given to the executable file
        this->ofname = ofname;  // executable output file name
    }

    /**
     * Destructor 
     */
    Server::~Server(){}

    /**
     * Open TCP connection
     */
    int Server::CreateTCPConnection(){

        // allocate a socket for the server
        if ( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
            ExitWithMessage( "Failed to create TCP server socket." );

        // attempt to stop port from blocking after repeated executions
        int allow = 1;
        if ( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &allow, sizeof(int)) < 0 )
            ExitWithMessage( "Failed to configure TCP server socket." );

        // assign a port and IP address to the socket
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;          // local host
        memset( &addr.sin_zero, 0, sizeof(addr.sin_zero) );

        if ( bind( server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0 )
            ExitWithMessage( "Failed to bind server socket." );

        // start listening to clients
        if ( listen( server_socket, 1 ) == -1) 
            ExitWithMessage( "Failed to listen for clients." );

        return EXIT_SUCCESS;
    }

    /**
     *  Connect to client
     */
    int Server::AcceptClient(){

        spdlog::info( "Waiting for client..." );

        // accept client
        if( (connection_socket = accept( server_socket, 0, 0)) < 0 )
            ExitWithMessage("Failed to accept client.");

        return EXIT_SUCCESS;
    }

    /**
     * Receive executable file from client
     */
    int Server::ReceiveFile(){

        FILE *efd;  // executable file descriptor

        if( !(efd = fopen( efname, "wb+" )) )   // open binary file for writing
            ExitWithMessage( "Couldn't open executable file." );


        struct Payload payload;

        // receive executable file size
        if( recv( connection_socket, &payload.size, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Error receiving executable size from client." );
        spdlog::info( "Executable file size: {}", payload.size );


        // receive executable
        payload.buffer = (unsigned char*)calloc( payload.size, sizeof( unsigned char ) );
        if( recv( connection_socket, payload.buffer, payload.size*sizeof( unsigned char ), 0 ) < 0 )
            ExitWithMessage( "Error receiving executable from client." );

        // create file
        fwrite( payload.buffer, sizeof(unsigned char), payload.size, efd );
        fclose( efd );

        return EXIT_SUCCESS;
    }  

    
    /**
     * Send executable output to client
     */
    void Server::SendOutput( int epid ){

        FILE* output;

        if( !( output = fopen( ofname, "rb" ) ) )
            ExitWithMessage( "Couldn't open output file.", epid );

        // parameters related to the amount of data that will be sent
        int block_size, size, last_size = 0;

        // non blocking wait: waitpid( pid, &status, WNOHANG )
        // return:
        //  0   - process hasn't died
        //  pid - process has died
        int status;
        while( !waitpid( epid, &status, WNOHANG ) ){    // while the chiold process is running

            // current output size
            fseek( output, 0, SEEK_END );
            size = ftell( output ); 

            while( size > last_size ){ // while there is something new to send

                block_size = size - last_size;

                fseek( output, last_size, SEEK_SET );   // set file pointer to last sent byte

                // send block size
                if( send( connection_socket, &block_size, sizeof( int ), 0 ) < 0 )    
                    ExitWithMessage( "Couldn't send block size to client.", epid );

                // read new block
                unsigned char *buffer;
                buffer = (unsigned char*)calloc( block_size, sizeof( unsigned char ) );
                fread( buffer, sizeof(unsigned char), block_size*sizeof( unsigned char ), output );
                
                // send block
                if( send( connection_socket, buffer, block_size, 0 ) < 0 )
                    ExitWithMessage( "Couldn't send block to client.", epid );

                last_size = size;
                fseek( output, 0, SEEK_END );
                size = ftell( output );
            }


            // PROTOCOL: send -1 when EOF is reached and the executable has finished too
            int flag = -1;
            if( send( connection_socket, &flag, sizeof( int ), 0 ) < 0 )
                ExitWithMessage( "Couldn't send EOF message to client." );
        }
    }

}