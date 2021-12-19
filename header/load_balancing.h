#ifndef LOAD_BALANCING_H
#define LOAD_BALANCING_H

#include "../header/receiver.h"
#include "../header/sender.h"
#include "../header/peer.h"
#include "../header/mediator.h"

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

    constexpr const char* EFNAME = "exec_copy";     // name with which the received executable file will be saved 
    constexpr const char* OFNAME = "output.txt";    // file to which the executable output is saved 


    enum PeerType { RECEIVER, SENDER };           // types of peers

    void ExitWithMessage( const char* );          // print error message and errno description
    void ExitWithMessage( const char*, pid_t );   // same as above but also kill the process identified by the given pid

    // Manages the executable execution
    class Executable{
        private:
            char* efname;       // executable file name
            char* ofname;       // file where the executable output is saved 
            pid_t child_pid;    // child process id

        public:
            Executable( char*, char* );
            virtual ~Executable();

            int Execute();      // executes 
    };

    // Controller class
    class LoadBalancing{

        public:
            virtual ~LoadBalancing();
            int RunApplication();

    #if PEER

        public:
            LoadBalancing( char*, int );        
            int GetPeerType();                 // ask user for peer type (receiver or sender)
            char* GetExecutableName();         // ask user for executable name
            
        private:
            int peer_type;
            Network::Peer::Peer peer;

            std::unique_ptr<Executable> executable;     // manages executable execution (used only by RECEIVER peer)

    #elif MEDIATOR

        public:
            LoadBalancing( int );   

        private:
            Network::Mediator::Mediator mediator;
            
    #endif 

    };

}


#endif // LOAD_BALANCING_H