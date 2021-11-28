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



namespace Network{

    constexpr int MAXLINE = 1024;   // verficar dps qual o tamanho maximo que pode mandar por TCP!!!

    // se necessario, pode ser usada para implementar funcoes comuns
    // a clientes e servidores
}

#endif // NETWORK_H