#include <iostream>
#include <thread>
#include "src/Server/Main_Server.h"
#include "src/Client/Main_CLI.h"
#include "src/DHT/DHT.h"
#include "src/UnitTests/MainUnitTests.h"

int main(int argc, char *argv[])
{
    paft::MainUnitTests::Run_All_Auto_Tests();

    paft::DHT::Init();

    if(argc > 1)
        paft::MainServer::Set_Port_Num(  atoi(argv [1]) );

    std::thread thread(paft::MainServer::Start_Server);

    paft::Main_CLI::Start_Client();


    return 0;
}
