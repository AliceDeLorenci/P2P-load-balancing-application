#include "../header/client.h"
#include <iostream>

namespace Network::Client{

    Client::Client(){}

    int Client::CreateTCPConnection(){

        // create client socket
        if ( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            spdlog::error("Failed to create a client socket.");
            if (errno) perror("");
            exit(EXIT_FAILURE);
        }

        // server socket address
        struct sockaddr_in addr;
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(port);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

        // connect to server
        if( connect( client_socket, (struct sockaddr*)&addr, sizeof(addr) ) < 0 ){
            spdlog::error("Failed to connect to server.");
            if (errno) perror("");
            return EXIT_FAILURE;
        }

        return 0;
    }

    int Client::SendMessage(){

        char mensagem[MAXLINE];
        std::cout << "Cliente: ";
        std::cin >> mensagem;

        send( client_socket, mensagem, strlen(mensagem), 0 );

        return 0;
    }

}