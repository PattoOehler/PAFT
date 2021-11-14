#include <iostream>
#include <thread>
#include "include/MainServer.h"
#include "include/MainCLI.h"
#include "include/DHT.h"


int main()
{
    //paft::DHT dht;

    paft::DHT::Init();

    //paft::DHT::Test_Add_Entry();
    //paft::DHT::Print_DHT();

    //dht.Test_Add_Entry();

    paft::MainServer Server;


    // The one is there to make the function work - number does not matter TODO
    //Running this --- paft::MainServer::Start_Server(); on a new thread
    std::thread thread(paft::MainServer::Start_Server, 1);



    paft::MainCli::Start_Client();


    return 0;
}
