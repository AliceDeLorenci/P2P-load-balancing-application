#ifndef NETWORK_H
#define NETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "spdlog/spdlog.h"



namespace LoadBalancing::Network{

    constexpr int MAXLINE = 1024;   // verificar dps qual o tamanho maximo que pode mandar por TCP!!!

    // payload that the client will send to the server containing the executable file
    struct Payload{
        int size;
        unsigned char* buffer;
    };

    // peer identification
    struct PeerID{
        int connection_socket;          // connection socket with server
        struct sockaddr_storage addr;   // peer socket address
        char ipstr[INET6_ADDRSTRLEN];   // peer IP address
        int port;                       // peer port (connection with mediator)

        int type;                       // peer type
        int server_port;                // peer's server port (only when type == RECEIVER)
    };

    // server identification 
    struct ReceiverID{
        char ipstr[INET6_ADDRSTRLEN];   // server IP address
        int port;                       // server port
    };

    // se necessario, pode ser usada para implementar funcoes comuns
    // a clientes e servidores
}

#endif // NETWORK_H