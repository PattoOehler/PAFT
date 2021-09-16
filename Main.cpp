#include <iostream>
#include <thread>
#include "include/MainServer.h"
#include "include/MainCLI.h"
#include "include/DHT.h"


int main()
{
    paft::DHT dht;

    dht.Test_Add_Entry();

    paft::MainServer Server;


    // The one is there to make the function work - number does not matter TODO
    std::thread thread(paft::MainServer::Start_Server, 1);

    //paft::MainServer::Start_Server();
    paft::MainCli::Start_Client();


    return 0;
}
