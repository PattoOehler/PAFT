#include <iostream>
#include <thread>
#include "src/Server/MainServer.h"
#include "src/Client/MainCLI.h"
#include "src/DHT/DHT.h"


int main()
{
    paft::DHT::Init();


    //paft::MainServer Server;
    std::thread thread(paft::MainServer::Start_Server);



    paft::MainCli::Start_Client();


    return 0;
}
