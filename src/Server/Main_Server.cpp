#include "Main_Server.h"
#include "Connection.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"

#include <winsock.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


#include <ws2tcpip.h>


#include <iostream>



using namespace paft;

unsigned short int MainServer::portNum = 1234;



int MainServer::Start_Server()
{

    std::cout << "Server starting on port " << Get_Port_Num() << "\n";

    // our masterSocket(socket that listens for connections)
    SOCKET sock;

    // for our thread
    DWORD thread;

    WSADATA wsaData;
    sockaddr_in server;

    // start winsock
    int ret = WSAStartup(0x101,&wsaData); // use highest version of winsock avalible

    if(ret != 0)
        return 0;


    // fill in winsock struct ...
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;

    DHT_Access::Set_Self_Port(Get_Port_Num());
    DHT_Access::Set_Self_Address("127.0.0.1");
    server.sin_port=htons(Get_Port_Num()); // listen on port

    // create our socket
    sock=socket(AF_INET,SOCK_STREAM,0);

    if(sock == INVALID_SOCKET)
    {
        return 0;
    }

    // bind our socket to a port
    if( bind(sock,(sockaddr*)&server,sizeof(server)) !=0 )
    {
        return 0;
    }

     // listen for a connection
    if(listen(sock,5) != 0)
    {
        return 0;
    }

     SOCKET client;

     sockaddr_in from;
     int fromlen = sizeof(from);



     // loop forever
     while(true)
     {
          // accept connections
          client = accept(sock,(struct sockaddr*)&from,&fromlen);
          longsocket *a = new longsocket;
          a->client = client;
          a->from = from;

          CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&paft::Connection::Handle_Client,(LPVOID)a, 0, &thread);
     }

     // shutdown winsock
     closesocket(sock);
     WSACleanup();

     // exit
     return 0;
}






void MainServer::Set_Port_Num(unsigned short int portNum)
{
    MainServer::portNum = portNum;

}


unsigned short int MainServer::Get_Port_Num()
{

    return portNum;
}
