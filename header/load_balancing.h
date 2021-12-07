#ifndef LOAD_BALANCING_H
#define LOAD_BALANCING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <thread>

namespace LoadBalancing{

    class Executable{
        private:
            char* fname;
            char* ofname;
            pid_t child_pid;

        public:
            Executable( char*, char* );
            int Execute();
    };

}


#endif // LOAD_BALANCING_H