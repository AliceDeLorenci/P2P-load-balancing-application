#include "../header/server.h" 
#include <iostream>

namespace Network::Server{

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
        char buffer[MAXLINE];
        int n = recv( connection_socket, buffer, MAXLINE, 0 );
        buffer[n] = '\0';

        std::cout << buffer << std::endl;

        return 0;
    }

}