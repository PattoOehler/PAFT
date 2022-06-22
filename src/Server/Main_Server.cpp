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


int MainServer::Start_Server()
{
    printf("Starting up the server on port 1234?\n");

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

    unsigned short int b = 1234;
    DHT_Access::Set_Self_Port(b);
    DHT_Access::Set_Self_Address("127.0.0.1");
    server.sin_port=htons(b); // listen on port

    // create our socket
    sock=socket(AF_INET,SOCK_STREAM,0);

    if(sock == INVALID_SOCKET)
    {
        return 0;
    }

    // bind our socket to a port(port 1234)
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
