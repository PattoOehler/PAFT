#include "Connection.h"
#include "MainServer.h"
#include "../include/dht.h"

#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>

#include <ws2tcpip.h>


#include <iostream>






using namespace paft;

Connection::Connection()
{
    //ctor
}

Connection::~Connection()
{
    //dtor
}

void Connection::Ping(LPVOID lpParam)
{
    SOCKET current_client = (SOCKET)lpParam;


    char sendbuf[] = "Pinging back message\0";
    send(current_client,sendbuf,21,0);


}


void Connection::Send_File(LPVOID lpParam)
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
        //std::cout << "filesize = " << filesize << " and counter*512 = " << 512*counter << std::endl;
        Sleep(10);


    }

    Sleep(20);
    fclose(asdf);
}


void Connection::Run_Proper_Command(char *buf, longsocket long_client)
{
    char sendData[100];

    if(buf[0] == 0x01)
        {
            //Client is asking for file

            Connection::Send_File((LPVOID)long_client.client);
            shutdown(long_client.client, SD_SEND);

        }
        else if(buf[0] == 0x02)
        {
            //Pinging
            //std::cout << "Client is asking for a ping response " << buf +1 << std::endl;
            Connection::Ping((LPVOID)long_client.client);


        }
        else
        {
            printf("Invalid cmd %x\n", buf[1]);
            strcpy(sendData,"Invalid cmd\n");
            Sleep(10);
            send(long_client.client,sendData,sizeof(sendData),0);

        }


}

void Connection::Handle_Client(LPVOID lpParam)
{
    //This method should always update the DHT

    //casting to longsocket
    longsocket long_client = *(longsocket *)(lpParam);

    if((long_client.client == INVALID_SOCKET))
        printf("INVALID SOCKET\n");

    char receivedData[100];
    int res;
    while(true)
    {

        res = recv(long_client.client,receivedData,sizeof(receivedData) ,0); // recv cmds
        Sleep(10);

        if(res == 0)
        {
            closesocket(long_client.client);
            ExitThread(0);
        }


        _160bitnumber a;
        if(res >= 21)
        {
            memcpy((void*)&a, receivedData+1, 20);
        }
        else
        {
            std::cout << "Sender did not send both their command and id. Exiting...\n";
            shutdown(long_client.client, SD_SEND);
            ExitThread(0);

        }
        Run_Proper_Command( receivedData, long_client);

        // clear buffers
        strcpy(receivedData,"");
   }

}
