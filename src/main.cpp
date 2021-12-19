#include "../header/load_balancing.h"

static void init() {

    #ifndef _DEBUG
        spdlog::set_level(spdlog::level::info);
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    #else
        // exhibits a greater number of runtime messages
        spdlog::set_level(spdlog::level::trace);
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [thread %t] %v");
    #endif

}

#if MEDIATOR

int main ( int argc, char* argv[] ){

    init();

    LoadBalancing::LoadBalancing app( 1234 );
    app.RunApplication();

    return 0;
}

#elif PEER

int main(){

    init();

    LoadBalancing::LoadBalancing app( "127.0.0.1", 1234 );
    app.RunApplication();
    
    return 0;
}

#endif











