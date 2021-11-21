#include <iostream>
#include <thread>
#include "include/MainServer.h"
#include "include/MainCLI.h"
#include "include/DHT.h"


int main()
{
    paft::DHT::Init();


    //paft::MainServer Server;
    std::thread thread(paft::MainServer::Start_Server);



    paft::MainCli::Start_Client();


    return 0;
}
