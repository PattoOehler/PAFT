#include "Connection.h"
#include "MainServer.h"
#include "../include/dht.h"

#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


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


    char sendbuf[20];
    _160bitnumber self = DHT::Get_SELF();
    memcpy(sendbuf, (char*)&self, 20); // 160/8=20


    send(current_client,sendbuf,20,0);



}


//DEPRECIATED
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


void Connection::Lookup_Peer(LPVOID lpParam, char buf[], int len)
{

    SOCKET current_client = (SOCKET)lpParam;


    char sendbuf[512];
    _160bitnumber self = DHT::Get_SELF();
    memcpy(sendbuf, (char*)&self, 20); // 160/8=20




    if(len < 42)
    {
        return;
    }

    _160bitnumber a;
    memcpy((char*)&a, buf+22, 20);
    three_DHT closest_Three = DHT::Find_Value(a);

    printf("Lookup_Peer before all of the memcpy's\n");
    int counter=0;
    if(closest_Three.entry[0].is_set)
    {
        memcpy(sendbuf+20, (char*)&closest_Three.entry[0].id, 20);
        memcpy(sendbuf+40, (char*)&closest_Three.entry[0].port, 2);
        memcpy(sendbuf+42, (char*)&closest_Three.entry[0].addr, 4);
        counter=1;

    }
    if(closest_Three.entry[1].is_set)
    {
        memcpy(sendbuf+46, (char*)&closest_Three.entry[0].id, 20);
        memcpy(sendbuf+66, (char*)&closest_Three.entry[0].port, 2);
        memcpy(sendbuf+68, (char*)&closest_Three.entry[0].addr, 4);
        counter=2;

    }
    if(closest_Three.entry[2].is_set)
    {
        memcpy(sendbuf+72, (char*)&closest_Three.entry[0].id, 20);
        memcpy(sendbuf+92, (char*)&closest_Three.entry[0].port, 2);
        memcpy(sendbuf+94, (char*)&closest_Three.entry[0].addr, 4);
        counter=3;

    }
    printf("Lookup_Peer after all of the memcpy's\n");
    send(current_client,sendbuf,20+26*counter,0);
    printf("Lookup_Peer after the send\n");
}




void Connection::Run_Proper_Command(char *buf, longsocket long_client, int len)
{
    char sendData[100];

    if(buf[0] == 0x01)
        {
            //Client is asking for file

            Connection::Send_File((LPVOID)long_client.client);

        }
        else if(buf[0] == 0x02)
        {
            //Pinging
            //std::cout << "Client is asking for a ping response " << buf +1 << std::endl;
            Connection::Ping((LPVOID)long_client.client);


        }
        else if(buf[0] == 0x03)
        {
            //Lookup Peer
            std::cout << "Client is asking to lookup a peer \n";
            Lookup_Peer((LPVOID)long_client.client, buf, len);


        }
        else if(buf[0] == 0x04)
        {
            //Lookup File
            std::cout << "Client is asking to lookup a file \n";

            std::cout << "NOT IMPLEMTNED YET \n";


        }
        else
        {
            printf("Invalid cmd %x\n", buf[0]);
            strcpy(sendData,"Invalid cmd\n");
            Sleep(10);
            send(long_client.client,sendData,sizeof(sendData),0);

        }

        shutdown(long_client.client, SD_SEND);


}

void Connection::Update_DHT(longsocket client, char recvdata[])
{
    _160bitnumber sender_Id;
    memcpy((void*)&sender_Id, recvdata+1, 20);

    DHT_Single_Entry sender_DHT_Entry;
    sender_DHT_Entry.addr = client.from.sin_addr;
    sender_DHT_Entry.id = sender_Id;
    sender_DHT_Entry.port = 1234;
    sender_DHT_Entry.is_set = true;

    DHT::Update_Time(sender_DHT_Entry);

}


void Connection::Handle_Client(LPVOID lpParam)
{
    //This method should always update the DHT

    //casting to longsocket
    longsocket long_client = *(longsocket *)(lpParam);

    //if((long_client.client == INVALID_SOCKET))
    //{
        //std::cout << "INVALID SOCKET IN Connection::Handle_Client";
        //ExitThread(7);
    //}


    char receivedData[512];
    int res = recv(long_client.client,receivedData,sizeof(receivedData) ,0); // recv cmds

    if(res <= 22)
    {
        std::cout << "Message length = " << res << ". Exiting...\n";
        closesocket(long_client.client);
        ExitThread(0);
    }

    Update_DHT(long_client, receivedData);
    Run_Proper_Command( receivedData, long_client, res);


}
