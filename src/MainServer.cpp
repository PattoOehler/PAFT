#include "../include/MainServer.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#include <iostream>

using namespace paft;

// our thread for recving commands
DWORD WINAPI MainServer::receive_cmds(LPVOID lpParam)
{
  printf("thread created\r\n");

  // set our socket to the socket passed in as a parameter
  SOCKET current_client = (SOCKET)lpParam;

  // buffer to hold our recived data
  char buf[100];
  // buffer to hold our sent data
  char sendData[100];
  // for error checking
  int res;

  // our recv loop
  while(true)
  {
     res = recv(current_client,buf,sizeof(buf),0); // recv cmds
     Sleep(10);

     if(res == 0)
     {
      //MessageBox(0,"error","error",MB_OK);
      closesocket(current_client);
      ExitThread(0);
     }


    if(buf[0] == 0x01)
    {
        //Client is asking for file
        std::cout << "Client is asking for a file named " << buf +1 << std::endl; //buf[0] is the command byte
        //strcpy(sendData,"1234567891011121314151617181920");
        MainServer::Send_File((LPVOID)current_client);
        shutdown(current_client, SD_SEND);

    }
    else
     {
       strcpy(sendData,"Invalid cmd\n");
       Sleep(10);
       send(current_client,sendData,sizeof(sendData),0);

     }

     // clear buffers
       strcpy(sendData,"");
       strcpy(buf,"");
   }
}

int MainServer::Send_File(LPVOID lpParam)
{
    // set our socket to the socket passed in as a parameter
    SOCKET current_client = (SOCKET)lpParam;

    static char FileName[] = "SERVER_FILES/asdf.txt";  //TODO replace with the file gathered in MainServer::receive_cmds()

    char Filebuf[512];
    FILE *asdf = fopen( FileName, "rb" );


    fseek(asdf, 0L, SEEK_END);
    int filesize = ftell(asdf);

    int counter=0;
    while(filesize > 512*counter)
    {

        if(counter == 0)
            fseek(asdf, 0, SEEK_SET);
        //fgets(Filebuf, 512, (FILE*)asdf);
        fread(Filebuf, 512, 1, (FILE*)asdf);


        //Calculate how many bytes are going to be sent
        int Bytes_To_Send=512;
        if(filesize-512 < 512*counter)
            Bytes_To_Send = filesize - (512*counter);

        send(current_client,Filebuf,Bytes_To_Send,0);
        counter++;
        std::cout << "filesize = " << filesize << " and counter*512 = " << 512*counter << std::endl;
        Sleep(10);
        if(filesize > 512*counter)
            std::cout << "HuRRay we going again in the server\n";

    }

    Sleep(20);
    fclose(asdf);

    return 0;
}



int MainServer::Start_Server(int a)
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
 {
    return 0;
 }

 // fill in winsock struct ...
 server.sin_family=AF_INET;
 server.sin_addr.s_addr=INADDR_ANY;
 server.sin_port=htons(1234); // listen on port

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

 // socket that we snedzrecv data on
 SOCKET client;

 sockaddr_in from;
 int fromlen = sizeof(from);



 // loop forever
 while(true)
 {
  // accept connections
  client = accept(sock,(struct sockaddr*)&from,&fromlen);
  printf("Client connected\r\n");

  // create our recv_cmds thread and parse client socket as a parameter
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&receive_cmds,(LPVOID)client, 0, &thread);
 }

 // shutdown winsock
 closesocket(sock);
 WSACleanup();

 // exit
 return 0;
}
