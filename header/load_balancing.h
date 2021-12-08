#ifndef LOAD_BALANCING_H
#define LOAD_BALANCING_H

#include "../header/server.h"
#include "../header/client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "spdlog/spdlog.h"

namespace LoadBalancing{

#ifdef SERVER
    constexpr const char* efname = "exec_copy";   // executable file name (received from client)
    constexpr const char* ofname = "output.txt";    // file to which the executable output is saved 
#endif 

    void ExitWithMessage( const char* );          // print error message and errno description
    void ExitWithMessage( const char*, pid_t );   // same as above but also kills child process

    // Manages the executable execution
    class Executable{
        private:
            char* efname;       // executable file name
            char* ofname;       // file where the executable output is saved 
            pid_t child_pid;    // child process id

        public:
            Executable( char*, char* );
            virtual ~Executable();

            int Execute();      // executes the executable
    };

    // Controller class
    class LoadBalancing{

        public:
            virtual ~LoadBalancing();
            int RunApplication();

    #ifdef SERVER

        public:
            LoadBalancing( int );

        private:
            Network::Server::Server server;     // manages server side connection
            Executable executable;              // manages executable execution

            //const char* efname = "exec_copy";   // executable file name (received from client)
            //const char* ofname = "output.txt";  // file where the executable output is saved 

    #elif CLIENT

        public:
            LoadBalancing( char*, int );
            int GetExecutableName();           // ask user for executable name

        private:
            Network::Client::Client client;

            char* efname;                       // executable file name (sent to client)


    #endif 

    };

}


#endif // LOAD_BALANCING_H