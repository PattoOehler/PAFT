

#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "Connection.h"
#include "Main_Server.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "../DHT/DHT_Search.h"
#include "../FileIO/Meta_Files.h"
#include "../Messages/Message_Ping.h"
#include "../Messages/Message_Proxy.h"
#include "../Messages/Message_Keyed_Proxy.h"
#include "../Client/Main_Client.h"
#include "../Peers/Peer_Access.h"
#include "../Client/Major_Functions.h"

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
    SOCKET currentClient = (SOCKET)lpParam;

    char *sendBuf = Message_Ping::Create_Ping_Responce();
    send(currentClient,sendBuf,20,0);
    delete[] sendBuf;

}



void Connection::Lookup_Peer(LPVOID lpParam, char buf[], int len)
{
    if(len < 43)
        return;



    SOCKET currentClient = (SOCKET)lpParam;


    char sendBuf[512];
    _160bitnumber self = DHT_Access::Get_Self_ID();
    memcpy(sendBuf, (char*)&self, 20); // 160/8=20






    _160bitnumber a;
    memcpy((char*)&a, buf+23, 20);
    three_DHT closestThree = DHT_Search::Lookup(a);

    int counter=0;
    if(closestThree.entry[0].is_set)
    {
        memcpy(sendBuf+20, (char*)&closestThree.entry[0].id, 20);
        memcpy(sendBuf+40, (char*)&closestThree.entry[0].port, 2);
        memcpy(sendBuf+42, (char*)&closestThree.entry[0].addr, 4);
        counter=1;

    }
    if(closestThree.entry[1].is_set)
    {
        memcpy(sendBuf+46, (char*)&closestThree.entry[0].id, 20);
        memcpy(sendBuf+66, (char*)&closestThree.entry[0].port, 2);
        memcpy(sendBuf+68, (char*)&closestThree.entry[0].addr, 4);
        counter=2;

    }
    if(closestThree.entry[2].is_set)
    {
        memcpy(sendBuf+72, (char*)&closestThree.entry[0].id, 20);
        memcpy(sendBuf+92, (char*)&closestThree.entry[0].port, 2);
        memcpy(sendBuf+94, (char*)&closestThree.entry[0].addr, 4);
        counter=3;

    }
    send(currentClient,sendBuf,20+26*counter,0);
}


void Connection::Lookup_File(LPVOID lpParam, char buf[], int len)
{
    if(len < 43)
        return;


    SOCKET currentClient = (SOCKET)lpParam;


    char sendBuf[512];
    _160bitnumber self = DHT_Access::Get_Self_ID();
    memcpy(sendBuf, (char*)&self, 20);



    _160bitnumber a;
    memcpy((char*)&a, buf+23, 20);


    three_DHT closestThree = DHT_Search::Find_Value(a);

    int counter=0;
    if(closestThree.entry[0].is_set)
    {
        memcpy(sendBuf+20, (char*)&closestThree.entry[0].id, 20);
        memcpy(sendBuf+40, (char*)&closestThree.entry[0].port, 2);
        memcpy(sendBuf+42, (char*)&closestThree.entry[0].addr, 4);
        counter=1;


    }
    if(closestThree.entry[1].is_set)
    {
        memcpy(sendBuf+46, (char*)&closestThree.entry[1].id, 20);
        memcpy(sendBuf+66, (char*)&closestThree.entry[1].port, 2);
        memcpy(sendBuf+68, (char*)&closestThree.entry[1].addr, 4);
        counter=2;

    }
    if(closestThree.entry[2].is_set)
    {
        memcpy(sendBuf+72, (char*)&closestThree.entry[2].id, 20);
        memcpy(sendBuf+92, (char*)&closestThree.entry[2].port, 2);
        memcpy(sendBuf+94, (char*)&closestThree.entry[2].addr, 4);
        counter=3;

    }



    send(currentClient,sendBuf,20+26*counter,0);

}



void Connection::Store_File(LPVOID lpParam, char buf[], int len, in_addr currentClientIP)
{
    SOCKET currentClient = (SOCKET)lpParam;


    char sendBuf[512];
    _160bitnumber self = DHT_Access::Get_Self_ID();
    memcpy(sendBuf, (char*)&self, 20); // 160/8=20

    if(len < 49)
    {
        printf("Too short of a sent message(%i) in Connection::Store_File!\n", len);
        return;
    }


    DHT_Single_Entry fileToAdd;

    memcpy((char*)&fileToAdd.id,buf+23,20);
    memcpy((char*)&fileToAdd.port, buf+43,2);
    memcpy((char*)&fileToAdd.addr, buf+45, 4);

    int *aa = (int *)&fileToAdd.addr;

    //Uses 0.0.0.0 to check if the person storing the file has the file.
    if( (*aa) == 0)
        fileToAdd.addr = currentClientIP;


    DHT_Access::Store_File_ID(fileToAdd);

    send(currentClient,sendBuf,20,0);

}




void Connection::Run_Proper_Command(char *buf, longsocket longClient, int len)
{
    char sendData[100];

    if(buf[0] == 0x01)
        {
            //Ping Command
            Connection::Ping((LPVOID)longClient.client);

        }
        else if(buf[0] == 0x02)
        {

        }
        else if(buf[0] == 0x03)
        {
            //Lookup Peer
            std::cout << "Client is asking to lookup a peer \n";
            Lookup_Peer((LPVOID)longClient.client, buf, len);


        }
        else if(buf[0] == 0x04)
        {
            //Lookup File
            std::cout << "Client is asking to lookup a file \n";

            Lookup_File((LPVOID)longClient.client, buf, len);


        }
        else if(buf[0] == 0x05)
        {
            //Lookup File
            std::cout << "Client is asking me to store a file ID\n";

            Store_File((LPVOID)longClient.client, buf, len, longClient.from.sin_addr);


        }
        else if(buf[0] == 0x06)
        {
            //Send chunk of a file
            std::cout << "Client is asking to download a chunk of a file\n";

            Send_File_Chunk((LPVOID)longClient.client, buf, len);


        }
        else if(buf[0] == 0x08)
        {
            //Forward a message
            std::cout << "Client is asking for me to be a proxy\n";

            Be_Proxy((LPVOID)longClient.client, buf, len);


        }
        else if(buf[0] == 0x09)
        {
            //Forward a message
            std::cout << "Client is asking for me to be an onion proxy NEW VERSION\n";

            Be_Onion_Proxy(longClient, buf, len);


        }
        else
        {
            printf("\nConnection::Run_Proper_Command Improper command %x\n", buf[0]);
            strcpy(sendData,"Invalid cmd\n");
            Sleep(10);
            send(longClient.client,sendData,sizeof(sendData),0);

        }
        shutdown(longClient.client, SD_SEND);

}

void Connection::Be_Proxy(LPVOID lpParam, char buf[], int len)
{

    if(len < 29 )
    {
        std::cout << "Connection::Be_Proxy len = " << len << "\nExiting\n";
        return;

    }
    SOCKET currentClient = (SOCKET)lpParam;
    Ping(lpParam); //To give them my DHT entry

    BaseResponce baseResp = Message_Proxy::Read_Base(buf, len);
    if(!baseResp.isSet)
    {
        std::cout << "\n\n BIG TIME ERROR IN Connection::Be_Proxy\n\n";
        return;
    }

    if(baseResp.forwardCommandByte == 8)
    {


        Proxy_Responce msg = Message_Proxy::Read_Proxy_8(buf, len);

        Main_Client client = Main_Client(msg.sendToAddr, msg.sendToPort);
        Message responce = client.Proxy(baseResp.forwardCommandByte, buf+30, len-30); //TODO make 30 not static

        send(currentClient,responce.message,responce.msgLength,0);
        free(responce.message);
    }
    else if(baseResp.forwardCommandByte == 6)
    {
        //Get Chunk Message
        ChunkResponce msg = Message_Proxy::Read_Chunk_6(buf, len);
        Main_Client client = Main_Client(msg.sendToAddr, msg.sendToPort);
        char *chunk = client.Get_File_Chunk(msg.fileID, msg.chunkID);
        if(chunk == nullptr)
        {
            std::cout << "Get_File_Chunk is NULL in connection:Be_Proxy() Exiting\n";
            return;
        }
        int chunkLen;
        memcpy((char *)&chunkLen, chunk, 4);
        chunk+=4;

        send(currentClient, chunk, chunkLen, 0);


    }
    else
    {
        std::cout << "Error in Connection:Be_Proxy() Unknown forwardCommandByte=" << baseResp.forwardCommandByte << "!!\n\n";

    }




}



void Connection::Be_Onion_Proxy(longsocket longClient, char buf[], int len)
{

    //SOCKET currentClient = longClient.client;
    Ping((LPVOID)longClient.client); //To give them my DHT entry



    Keyed_Proxy_Responce resp = Message_Keyed_Proxy::Read_Upload_Message(buf, len);
    if(resp.code == -1)
    {
        std::cout << "\n\nERROR IN Connection::Be_Onion_Proxy\n\n";

    }
    else if(resp.code == 0)
    {
        DHT_Single_Entry from, to;
        from.addr = longClient.from.sin_addr;
        from.port = longClient.from.sin_port;
        to.addr = resp.sendToAddr;
        to.port = resp.sendToPort;

        Peer_Access::Add_Peer(from, to, resp.key);

        Main_Client client(resp.sendToAddr, resp.sendToPort);
        client.Proxy_Onion(resp.key, buf, len);


    }
    else if(resp.code == 1)
    {
        std::cout << "\n\nStoring File On Network Onion Proxy\n\n";

        DHT_Single_Entry self;
        self.is_set = true;
        self.id = resp.fileID;
        unsigned int z=0;
        memcpy((char *)&self.addr,(char *) &z, 4);
        self.port = DHT_Access::Get_Self_Port();


        DHT_Access::Store_File_ID(self);


        three_DHT closestInNetwork = Major_Functions::Three_Closest_Peers_In_Network(resp.fileID);
        for(int i=0;i<3;i++)
        {
            if(closestInNetwork.entry[i].is_set)
            {
                Main_Client client = Main_Client(closestInNetwork.entry[i].addr, closestInNetwork.entry[i].port);
                client.Store_File(self);
            }
        }

    }


}






void Connection::Send_File_Chunk(LPVOID lpParam, char buf[], int len)
{

    if(len < 47 )
    {
        std::cout << "Connection::Seld_File_Chunk len = " << len << "\nExiting\n";
        return;

    }
    std::cout << "Connection::Send_File_Chunk is getting called\n";

    SOCKET currentClient = (SOCKET)lpParam;


    _160bitnumber fileID;
    int desiredChunk;
    memcpy((char*)&desiredChunk,buf+23,4);
    memcpy((char*)&fileID, buf+27,20);


    Ping(lpParam);
    int FileLocation = DHT_Access::Find_Stored_File(fileID);
    if(FileLocation == -1)
    {
        std::cout << "\n\nThe FileLocation is not found in Connection::Send_File_Chunk! - " << DHT::ID_To_String(fileID) << "\n\n";
        return; //We don't have the file stored so exit the connection
    }



    if(desiredChunk == -1)
    {
        std::cout << "Connection::Send_File_Chunk Desired Chunk == -1\n";
        //The meta-data file is what needs to be returned
        std::string metafilePath = Meta_Files::Get_Output_File_Name(fileID);
        int Eight_MiB = 8000000;

        char *buf;
        buf = (char *) malloc(Eight_MiB); //TODO check that this is free()



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
                send(currentClient,buf,bytesRead,0);
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
            send(currentClient,buf,bytesRead,0);

        }


    }


}







void Connection::Update_DHT(longsocket client, Base_Return data)
{

    DHT_Single_Entry senderEntry;
    senderEntry.addr = client.from.sin_addr;
    senderEntry.id = data.ID;

    senderEntry.port = data.port; //TODO

    senderEntry.is_set = true;

    DHT::Update_Time(senderEntry);

}


void Connection::Handle_Client(LPVOID lpParam)
{
    //This method should always update the DHT

    //casting to longsocket
    longsocket longClient = *(longsocket *)(lpParam);

    if((longClient.client == INVALID_SOCKET))
    {
        std::cout << "INVALID SOCKET IN Connection::Handle_Client";
        return ;
    }


    char receivedData[512];
    int res = recv(longClient.client,receivedData,sizeof(receivedData) ,0); // recv cmds

    if(res <= 22)
    {
        std::cout << "Message length in Connection::Handle_Client = " << res << ". Exiting...\n";
        closesocket(longClient.client);
        ExitThread(0);
    }

    Base_Return data = Base_Message::Read_Base(receivedData, res);

    Update_DHT(longClient, data);
    Run_Proper_Command( receivedData, longClient, res);


}
