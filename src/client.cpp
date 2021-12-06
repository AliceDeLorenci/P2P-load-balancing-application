#include "../header/client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    void Client::SendFile(){

        FILE *input, *output;

        input = fopen( "teste", "rb+" );
        output = fopen( "copia", "wb+" );

        if( (input == NULL) || (output == NULL) )
            spdlog::error("Could not open file!");

        fseek( input, 0, SEEK_END );
        int size = ftell( input );  // executable size in bytes

        fseek( input, 0, SEEK_SET );

        unsigned char *buffer;
        buffer = (unsigned char*)calloc( size, sizeof( unsigned char ) );

        fread( buffer, sizeof(unsigned char), size, input );
        fwrite( buffer, sizeof(unsigned char), size, output );

        fclose( input );
        fclose( output );

        system("chmod +x copia && ./copia");
        

    }  
}


/*
        std::ifstream input( "/home/alice/GIT/SSC0904/teste", std::ios::binary );
        //std::byte
        // copies all data into buffer
        std::vector<unsigned char> buffer( std::istreambuf_iterator<char>(input), {} );


        std::ofstream output( "/home/alice/GIT/SSC0904/copia", std::ios::binary );

        char* buffer1 = new char[ buffer.size() ];
        output.write( buffer1, buffer.size() );

        input.close();
        output.close();

        system("chmod +x copia && ./copia");
*/
/*
        output.CopyTo(Stream);

        
        const int64_t fileSize = GetFileSize(fileName);
        if (fileSize < 0) { return -1; }

        std::ifstream file(fileName, std::ifstream::binary);
        if (file.fail()) { return -1; }

        if (SendBuffer(s, reinterpret_cast<const char*>(&fileSize),
            sizeof(fileSize)) != sizeof(fileSize)) {
            return -2;
        }

        char* buffer = new char[chunkSize];
        bool errored = false;
        int64_t i = fileSize;
        while (i != 0) {
            const int64_t ssize = __min(i, (int64_t)chunkSize);
            if (!file.read(buffer, ssize)) { errored = true; break; }
            const int l = SendBuffer(s, buffer, (int)ssize);
            if (l < 0) { errored = true; break; }
            i -= l;
        }
        delete[] buffer;

        file.close();

        return errored ? -3 : fileSize;
        */
       
      
    