#include <iostream>
#include <thread>
#include "src/Server/Main_Server.h"
#include "src/Client/Main_CLI.h"
#include "src/DHT/DHT.h"
#include "src/UnitTests/MainUnitTests.h"

int main()
{
    paft::MainUnitTests::Run_All_Auto_Tests();

    paft::DHT::Init();

    std::thread thread(paft::MainServer::Start_Server);

    paft::Main_CLI::Start_Client();


    return 0;
}
