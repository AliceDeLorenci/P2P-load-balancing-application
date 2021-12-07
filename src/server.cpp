#include "../header/server.h" 
#include "../header/load_balancing.h"
#include <iostream>

namespace LoadBalancing::Network::Server{

    Server::Server(){}

    int Server::CreateTCPConnection(){

        // allocate a socket for the server
        if ( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            spdlog::error("Failed to create TCP server socket.");
            if (errno) perror("");
            exit(EXIT_FAILURE);
        }

        // attempt to stop port from blocking after repeated executions
        int allow = 1;
        if ( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &allow, sizeof(int)) < 0 )
            spdlog::error("Failed to create TCP server socket.");

        // assign a port and IP address to the socket
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;          // LOCAL HOST
        memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

        if ( bind( server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0 ) {
            spdlog::error("Failed to bind server socket.");
            if (errno) perror("");
            exit(EXIT_FAILURE);
        }

        // start listening to clients
        if ( listen( server_socket, 1 ) == -1) {
            spdlog::error("Failed to listen to clients.");
            if (errno) perror("");
            return EXIT_FAILURE;
        }

        return 0;
    }

    int Server::AcceptClient(){

        std::cout << "Waiting for client..." << std::endl;

        // accept client
        if( (connection_socket = accept( server_socket, 0, 0)) < 0 ){
            spdlog::error("Failed to accept client.");
            if (errno) perror("");
            return EXIT_FAILURE;
        }

        // listen to client
        /*
        char buffer[MAXLINE];
        int n = recv( connection_socket, buffer, MAXLINE, 0 );
        buffer[n] = '\0';

        std::cout << buffer << std::endl;
        */

        return 0;
    }

    void Server::ReceiveFile( char* fname ){

        this->fname = fname;

        FILE *output;

        output = fopen( fname, "wb+" );

        if( output == NULL )
            spdlog::error("Could not open file!");

        struct Payload payload;

        int n;
        n = recv( connection_socket, &payload.size, sizeof( int ), 0 );

        std::cout << "File size: " << payload.size << std::endl;

        payload.buffer = (unsigned char*)calloc( payload.size, sizeof( unsigned char ) );

        n = recv( connection_socket, payload.buffer, payload.size*sizeof( unsigned char ), 0 );

        // unsigned char *buffer;
        // buffer = (unsigned char*)calloc( size, sizeof( unsigned char ) );

        fwrite( payload.buffer, sizeof(unsigned char), payload.size, output );

        fclose( output );

        Executable exec( fname, ofname );
        int exec_pid = exec.Execute();

        SendOutput( exec_pid );
    
    }  

    // void Server::StartSendingOutput(){

    // }

    void Server::SendOutput( int exec_pid ){

        FILE* output;

        if( !( output = fopen( ofname, "rb" ) ) )
            spdlog::error("Could not open output file!");


        int status;
        int block_size, size, last_size = 0;
        while( !waitpid( exec_pid, &status, WNOHANG ) ){

            fseek( output, 0, SEEK_END );
            size = ftell( output );

            while( size > last_size ){ // if the file has increased, send block

                block_size = size - last_size;

                fseek( output, last_size, SEEK_SET );   // set file pointer to last position

                send( connection_socket, &block_size, sizeof( int ), 0 );   // send block size

                // send file block
                unsigned char *buffer;
                buffer = (unsigned char*)calloc( block_size, sizeof( unsigned char ) );
                fread( buffer, sizeof(unsigned char), block_size*sizeof( unsigned char ), output );

                send( connection_socket, buffer, block_size, 0 );

                last_size = size;
                fseek( output, 0, SEEK_END );
                size = ftell( output );

                // PROTOCOL: send -1 when EOF is reached

            }

            int flag = -1;
            send( connection_socket, &flag, sizeof( int ), 0 );   // send block size

        }


    }

}