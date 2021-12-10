#include "../header/load_balancing.h"

namespace LoadBalancing{

    /**
     * Print error message and error description
     */
    void ExitWithMessage( const char* msg ){
        spdlog::error( msg );
        if (errno) perror("");
        exit( EXIT_FAILURE );
    }

    /**
     * Print error message and error description and kill child process
     */
    void ExitWithMessage( const char* msg, pid_t pid ){
        kill( pid, SIGKILL );
        ExitWithMessage( msg );
    }

    /**************************************** LOAD BALANCING CLASS ****************************************/

    LoadBalancing::~LoadBalancing() {}

#ifdef SERVER

    LoadBalancing::LoadBalancing( int port ) : server( port, const_cast<char*>(EFNAME), const_cast<char*>(OFNAME) ),
                                                executable( const_cast<char*>(EFNAME), const_cast<char*>(OFNAME) ){ }

    /**
     * Run server side Load Balancing application
     */
    int LoadBalancing::RunApplication(){

        spdlog::info( "SERVER" );

        server.CreateTCPConnection();   // open TCP socket
        server.AcceptClient();          // establish connection with client
        server.ReceiveFile();           // receive executable
        int epid = executable.Execute();// run executable
        server.SendOutput( epid );      // send output to client (run on a thread??)

        return EXIT_SUCCESS;
    }

#elif CLIENT

    LoadBalancing::LoadBalancing( char* IP, int port ) : client( IP, port ){ }

    /**
     * Run client side Load Balancing application
     */
    int LoadBalancing::RunApplication(){

        spdlog::info( "CLIENT" );

        GetExecutableName();        // ask user for executable name

        client.CreateTCPConnection();   // establish connection with server
        client.SendFile( efname );      // send executable file name
        client.ReceiveOutput();         // receive executable output
        
        return EXIT_SUCCESS;
    }

    /**
     * Ask user for executable name
     */
    int LoadBalancing::GetExecutableName(){

        std::cout << "Executable name: ";

        // receive executable file name
        std::string s;
        std::cin >> s;

        efname = (char*)calloc( s.length() + 1, sizeof(char) );
        strcpy( efname, s.c_str() );

        // check if file exists
        if( !fopen( efname, "r" ) )
            ExitWithMessage("File doesn't exist.");

        // check if file is executable
        if( access( efname, X_OK ) == -1 )
            ExitWithMessage("File isn't executable.");

        return EXIT_SUCCESS;
    }

#elif PEER

    LoadBalancing::LoadBalancing( char* mediator_IP, int mediator_port ) : peer( mediator_IP, mediator_port ){ }

    /**
     * Run Load Balancing peer
     */
    int LoadBalancing::RunApplication(){

        spdlog::info( "PEER" );

        GetPeerType();                  // ask user whether it will send or receive a job
        peer.SetPeerType( peer_type );

        peer.ConnectToMediator();       // peer connects to mediator 

        if( peer_type == RECEIVER ){

            peer.GetLoad();

        }else if( peer_type == SENDER ){

            GetExecutableName();        // ask user for executable name
            peer.SendLoad( efname );    // process load sending

        }else{
            return EXIT_FAILURE;
        }


        /*
        GetExecutableName();        // ask user for executable name

        client.CreateTCPConnection();   // establish connection with server
        client.SendFile( efname );      // send executable file name
        client.ReceiveOutput();         // receive executable output
        */
        return EXIT_SUCCESS;
    }

    /**
     * Ask user for peer type
     */
    int LoadBalancing::GetPeerType(){

        std::cout << "Choose an option:\n\t1. Receive job\n\t2. Send job" << std::endl;

        std::string s;
        std::cin >> s;

        if( s.compare("1") == 0 ){
            peer_type = RECEIVER;
        }else if( s.compare("2") == 0 ){
            peer_type = SENDER;
        }else{
            ExitWithMessage( "Invalid peer type." );
        }

        return EXIT_SUCCESS;
    }

    /**
     * Ask user for executable name
     */
    int LoadBalancing::GetExecutableName(){

        std::cout << "Executable name: ";

        // receive executable file name
        std::string s;
        std::cin >> s;

        efname = (char*)calloc( s.length() + 1, sizeof(char) );
        strcpy( efname, s.c_str() );

        // check if file exists
        if( !fopen( efname, "r" ) )
            ExitWithMessage("File doesn't exist.");

        // check if file is executable
        if( access( efname, X_OK ) == -1 )
            ExitWithMessage("File isn't executable.");

        return EXIT_SUCCESS;
    }

#elif MEDIATOR

    LoadBalancing::LoadBalancing( int mediator_port ) : mediator( mediator_port ){ }

    int LoadBalancing::RunApplication(){

        mediator.CreateTCPConnection();

        while( true ){
            mediator.AcceptClient();
            mediator.DistributeLoads();
        }

        return EXIT_SUCCESS;
    }

#endif

    /****************************************** EXECUTABLE CLASS ******************************************/
     
    Executable::Executable( char* efname, char* ofname ){
        this->efname = efname;  // executable file name
        this->ofname = ofname;  // file where the executable output is saved 
    }

    Executable::~Executable() {};

    /**
     * Executes the client program
     */
    int Executable::Execute(){

        char sys_cmd[128];
        sprintf( sys_cmd, "chmod +x %s", efname );   // make file executable
        system( sys_cmd );

        pid_t pid = fork(); // fork new process

        if( pid == 0 ){  // child process

            // redirect stdout to file
            int fd;
            if( (fd = open( ofname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR )) < 0 )
                ExitWithMessage( "Couldn't setup stout redirection." );
            dup2( fd, 1 );  
            close( fd );

            // replace current process with desired executable
            char* argv_list[] = { efname, NULL };
            execv( efname, argv_list ); 

            return 0;   
        }
        else if( pid > 0 ){ // parent process 
            child_pid = pid;
            return child_pid;
        }
        else{
            ExitWithMessage( "Error forking process." );
        }

        return -1;  // should never get here
    }
}