

#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "Connection.h"
#include "MainServer.h"
#include "../DHT/dht.h"
#include "../DHT/DHT_Access.h"
#include "../FileIO/Meta_Files.h"

#include <ws2tcpip.h>


#include <iostream>






using namespace paft;

Connection::Connection()
{
    //ctor
}
Connection::~Connection()
{
    //ctor
}


void Connection::Ping(LPVOID lpParam)
{
    SOCKET current_client = (SOCKET)lpParam;


    char sendbuf[20];
    _160bitnumber self = DHT_Access::Get_SELF_ID();
    memcpy(sendbuf, (char*)&self, 20);


    send(current_client,sendbuf,20,0);


}


//DEPRECIATED
void Connection::Send_File(LPVOID lpParam)
{

    // set our socket to the socket passed in as a parameter
    SOCKET current_client = (SOCKET)lpParam;

    static char FileName[] = "SERVER_FILES/asdf.txt";
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
        Sleep(10);


    }

    Sleep(20);
    fclose(asdf);
}


void Connection::Lookup_Peer(LPVOID lpParam, char buf[], int len)
{

    SOCKET current_client = (SOCKET)lpParam;


    char sendbuf[512];
    _160bitnumber self = DHT_Access::Get_SELF_ID();
    memcpy(sendbuf, (char*)&self, 20); // 160/8=20




    if(len < 43)
    {
        return;
    }

    _160bitnumber a;
    memcpy((char*)&a, buf+23, 20);
    three_DHT closest_Three = DHT::Lookup(a);

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
    send(current_client,sendbuf,20+26*counter,0);
}


void Connection::Lookup_File(LPVOID lpParam, char buf[], int len)
{

    SOCKET current_client = (SOCKET)lpParam;


    char sendbuf[512];
    _160bitnumber self = DHT_Access::Get_SELF_ID();
    memcpy(sendbuf, (char*)&self, 20);




    if(len < 43)
    {
        return;
    }

    _160bitnumber a;
    memcpy((char*)&a, buf+23, 20);
    three_DHT closest_Three = DHT::Find_Value(a);

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
    send(current_client,sendbuf,20+26*counter,0);

}



void Connection::Store_File(LPVOID lpParam, char buf[], int len, in_addr current_client_ip)
{
    //TODO
    //Currently uses the position 159*20 to check for ip address 127.0.0.1 - but needs to be more robust
    // i.e. use something along the lines of 0.0.0.0 or force 127.0.0.1 and not allow 127.0.1.1

    SOCKET current_client = (SOCKET)lpParam;


    char sendbuf[512];
    _160bitnumber self = DHT_Access::Get_SELF_ID();
    memcpy(sendbuf, (char*)&self, 20); // 160/8=20

    if(len < 49)
    {
        printf("Too short of a sent message(%i) in Connection::Store_File!\n", len);
        return;
    }


    DHT_Single_Entry file_To_Add;

    memcpy((char*)&file_To_Add.id,buf+23,20);
    memcpy((char*)&file_To_Add.port, buf+43,2);
    memcpy((char*)&file_To_Add.addr, buf+45, 4);

    DHT_Single_Entry self_entry = DHT_Access::Access_DHT(159*20);
    int *aa = (int *)&file_To_Add.addr;
    int *bb = (int *)&self_entry.addr;

    if(&aa == &bb)
        file_To_Add.addr = current_client_ip;

    DHT_Access::Store_FileId(file_To_Add);

    send(current_client,sendbuf,20,0);

}




void Connection::Run_Proper_Command(char *buf, longsocket long_client, int len)
{
    char sendData[100];

    if(buf[0] == 0x01)
        {
            //Client is asking for FULL file
            // Depreciated TODO delete
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

            Lookup_File((LPVOID)long_client.client, buf, len);


        }
        else if(buf[0] == 0x05)
        {
            //Lookup File
            std::cout << "Client is asking to store a file ID\n";

            Store_File((LPVOID)long_client.client, buf, len, long_client.from.sin_addr);


        }
        else if(buf[0] == 0x06)
        {
            //Send chunk of a file
            std::cout << "Client is asking to download a chunk of a file\n";

            Send_File_Chunk((LPVOID)long_client.client, buf, len);


        }
        else
        {
            printf("\nConnection::Run_Proper_Command Improper command %x\n", buf[0]);
            strcpy(sendData,"Invalid cmd\n");
            Sleep(10);
            send(long_client.client,sendData,sizeof(sendData),0);

        }
        shutdown(long_client.client, SD_SEND);

}

void Connection::Send_File_Chunk(LPVOID lpParam, char buf[], int len)
{

    if(len < 47 )
    {
        std::cout << "Connection::Seld_File_Chunk len = " << len << "\nExiting\n";
        return;

    }
    std::cout << "Connection::Send_File_Chunk is getting called\n";

    SOCKET current_client = (SOCKET)lpParam;


    _160bitnumber FileID;
    int desiredChunk;
    memcpy((char*)&desiredChunk,buf+23,4);
    memcpy((char*)&FileID, buf+27,20);


    Ping(lpParam);
    int FileLocation = DHT_Access::Find_Stored_File(FileID);
    if(FileLocation == -1)
    {
        std::cout << "\n\nThe FileLocation is not found in Connection::Send_File_Chunk! - ";
        DHT::Print_ID(FileID);
        std::cout << "\n\n";
        return; //We don't have the file stored so exit the connection
    }



    if(desiredChunk == -1)
    {
        std::cout << "Connection::Send_File_Chunk Desired Chunk == -1\n";
        //The meta-data file is what needs to be returned
        std::string metafilePath = Meta_Files::getOutput_File_Name(FileID);
        int Eight_MiB = 8000000;

        char *buf;
        buf = (char *) malloc(Eight_MiB);



        //Send the bytes - already pinged
        FILE *file = NULL;
        size_t bytesRead = 0;

        file = fopen(metafilePath.c_str(), "rb");

        if (file != NULL)
        {
            std::cout << "Connection::Send_File_Chunk file not null\n";
            while ((bytesRead = fread(buf, 1, Eight_MiB, file)) > 0)
            {
                // process bytesRead worth of data in buffer
                std::cout << "Connection::Send_File_Chunk While loop\n";
                send(current_client,buf,bytesRead,0);
            }
        }


    }
    else
    {
        //Return the chunk at position desiredChunk
        std::cout << "The chunk desired is " << desiredChunk << " in Connection::Send_File_Chunk\n\n";



        //The chunk is what needs to be returned
        std::string filePath = DHT_Access::Get_Local_File_Location(FileLocation);;
        int Eight_MiB = 8000000;

        char *buf;
        buf = (char *) malloc(Eight_MiB);



        //Send the bytes - already pinged
        FILE *file = NULL;
        size_t bytesRead = 0;

        file = fopen(filePath.c_str(), "rb");

        fseek(file, desiredChunk*Eight_MiB, SEEK_SET);


        if (file != NULL)
        {
            std::cout << "Connection::Send_File_Chunk file not null\n";

            bytesRead = fread(buf, 1, Eight_MiB, file);

            // process bytesRead worth of data in buffer
            std::cout << "Connection::Send_File_Chunk While loop\n";
            send(current_client,buf,bytesRead,0);

        }


    }


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
        std::cout << "Message length in Connection::Handle_Client = " << res << ". Exiting...\n";
        closesocket(long_client.client);
        ExitThread(0);
    }

    Update_DHT(long_client, receivedData);
    Run_Proper_Command( receivedData, long_client, res);


}
