#include "../header/server.h"
#include "../header/client.h"

#include <iostream>
#include <string>

#include <unistd.h>

int GetExecutableName( char* fname ){

    std::cout << "Executable name: ";

    // receive executable file name
    std::string s;
    std::cin >> s;

    fname = (char*)calloc( s.length() + 1, sizeof(char) );
    strcpy( fname, s.c_str() );

    // check if file exists
    if( !fopen( fname, "r" ) ){
        spdlog::error("File doesn't exist.");
        if (errno) perror("");
        return EXIT_FAILURE;
    }

    // check if file is executable
    if( access( fname, X_OK ) == -1 ){
        spdlog::error("File isn't executable.");
        if (errno) perror("");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}

#ifdef SERVER

int main ( int argc, char* argv[] ){

    std::cout << "SERVER" << std::endl;

    char fname[] = "copia";

    LoadBalancing::Network::Server::Server servidor;

    servidor.CreateTCPConnection();
    servidor.AcceptClient();
    servidor.ReceiveFile( fname );

    return 0;
}

#elif CLIENT
int main(){

    
    std::cout << "CLIENTE" << std::endl;

    char* fname;
    if( GetExecutableName( fname ) == EXIT_FAILURE )
        return EXIT_FAILURE;

    LoadBalancing::Network::Client::Client client;
    client.CreateTCPConnection();
    char fname[] = "teste";
    client.SendFile( fname );
    client.ReceiveOutput();
    //client.SendMessage();

    return 0;
}

#elif PEER
int main(){
    
    std::cout << "PEER" << std::endl;

    do{
        std::string s;
        std::cin >> s;
        
        if(s == "c"){
            LoadBalancing::Network::Peer::Peer peer;
            char fname[] = "teste";
            peer.AskForJob( fname );
        }
        else{
            LoadBalancing::Network::Peer::Peer peer;
            char fname[] = "copia";
            peer.ReceiveJobe( fname );
        }

    }while(s != "c" && s != "s");
    
    return 0;
}
#endif











