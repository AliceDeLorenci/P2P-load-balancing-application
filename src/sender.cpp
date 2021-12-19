#include "../header/sender.h"
#include "../header/load_balancing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace LoadBalancing::Network::Sender{

    /**
     * Constructor
     */
    Sender::Sender( char* IP, int port ){
        this->IP = IP;
        this->port = port;
    }

    /**
     * Destructor
     */
    Sender::~Sender(){}

    /**
     * Establish connection with receiver
     */
    int Sender::CreateTCPConnection(){

        // create a socket
        if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ExitWithMessage("Failed to create a socket.");

        // receiver socket address
        struct sockaddr_in addr;
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons( port );
        addr.sin_addr.s_addr = inet_addr( IP );
        memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

        // connect to receiver
        if( connect( socketfd, (struct sockaddr*)&addr, sizeof(addr) ) < 0 )
            ExitWithMessage("Failed to connect to receiver.");

        return EXIT_SUCCESS;
    }

    /**
     * Send executable to receiver
     */
    void Sender::SendFile( char* efname ){

        this->efname = efname;

        FILE *efd;

        if( !(efd = fopen(efname, "rb+")) )
            ExitWithMessage( "Couldn't open file." );

        // get executable size in bytes
        fseek( efd, 0, SEEK_END );
        int size = ftell( efd );  

        // send file size to receiver
        if( send( socketfd, &size, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Coundn't send executable file size to receiver." );

        // reposition file pointer
        fseek( efd, 0, SEEK_SET );

        // read file
        unsigned char *buffer;
        buffer = (unsigned char*)calloc( size, sizeof( unsigned char ) );
        fread( buffer, sizeof(unsigned char), size*sizeof( unsigned char ), efd );

        // send file to receiver
        if( send( socketfd, buffer, size, 0 ) < 0 )
            ExitWithMessage( "Coundn't send executable file to receiver." );

        fclose( efd );
    }  

    /**
     * Check whether there is any new output or whether the executable has finished running.
     * If there is new output, returns next block size, else, returns -1.
     */
    int Sender::CheckReceiveOutput(){
        
        // next block size
        int size;
        if( recv( socketfd, &size, sizeof( int ), 0 ) < 0 )
            ExitWithMessage( "Couldn't receive next block size." );
        return size;
    }

    /**
     * Constantly receive executable output from receiver.
     */
    void Sender::ReceiveOutput(){

        int size = 0;

        while( (size = CheckReceiveOutput()) >= 0 ){    // while the process hasn't finished

            // receive new block
            unsigned char* buffer;
            buffer = (unsigned char*)calloc( size, sizeof( unsigned char ) );
            if( recv( socketfd, buffer, size*sizeof( unsigned char ), 0 ) < 0 )
                ExitWithMessage( "Counldn't receive new block." );

            // print to terminal
            std::cout << buffer;
        }
    }
}
