#include "../header/load_balancing.h"

namespace LoadBalancing{

    Executable::Executable( char* fname, char* ofname ){
        this->fname = fname;
        this->ofname = ofname;
    }

    int Executable::Execute(){

        char sys_cmd[128];
        sprintf( sys_cmd, "chmod +x %s", fname );   // make file executable
        system( sys_cmd );

        pid_t pid = fork();


        if( pid == 0 ){  // child process

            int fd = open( ofname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
            dup2( fd, 1 );  // redirect stdout to file
            close( fd );

            char* argv_list[] = { fname, NULL };
            execv( fname, argv_list ); 

            return 0;   
        }
        else if( pid > 0 ){ // parent process ( pid = child_pid )

            child_pid = pid;
            // int status;
            // waitpid( child_pid, &status, 0 ); // wait for child process to finish

            // int status = waitpid( child_pid, &status, WNOHANG ); // non blocking wait
            // returns 0 -> nothing has died
            // returns child_pid -> child has died

            return child_pid;
        }
        //sprintf( sys_cmd, "./%s > %s", fname, ofname );   // execute and save output to file
        //system( sys_cmd );

    }
}