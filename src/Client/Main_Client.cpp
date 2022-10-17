
#include "Main_Client.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "Minor_Functions.h"
#include "../DHT/DHT_Lookup.h"
#include "../Messages/Message_Ping.h"
#include "../Messages/Message_Proxy.h"
#include "../Messages/Message_Keyed_Proxy.h"

#include <iostream>

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <thread>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
/*pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
 Unknown if it does anything
*/
//DEFAULT_BUFLEN was   512  if stuff breaks
#define DEFAULT_BUFLEN 2048



using namespace paft;


in_addr Main_Client::NULL_Addr()
{
    in_addr a;
    unsigned int b = 0;
    memcpy((char *)&a,(char *)& b, 4);
    return a;

}



void Main_Client::Shutdown_Connection_Gracefully()
{
    if(!set_Up_Properly)
        return;

    char recvBuf[DEFAULT_BUFLEN];
    // shutdown the connection since no more data will be sent
    int iResult = shutdown(server_Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }



    // Receive until the peer closes the connection
    do {
        iResult = recv(server_Socket, recvBuf, DEFAULT_BUFLEN, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(server_Socket);
    WSACleanup();



}



void Main_Client::Add_Self(char buf[])
{
    _160bitnumber self = DHT_Access::Get_Self_ID();
    memcpy(buf+1, (char*)&self, 20); // 160/8=20
    short unsigned int port = DHT_Access::Get_Self_Port();
    memcpy(buf+21, (char*)&port, 2);

}


void Main_Client::Add_Received_Entry_To_DHT(char recvbuf[], int length)
{
    if(!set_Up_Properly)
        return;

    if(length >= 20)
    {


        _160bitnumber senderID;
        memcpy((void*)&senderID, recvbuf, 20);


        DHT_Single_Entry senderEntry;
        senderEntry.addr = server_IP;
        senderEntry.id = senderID;
        senderEntry.port = server_Port;
        senderEntry.is_set = true;


        DHT::Update_Time(senderEntry);

    }
    else
    {
        printf("Entry too short in MainClient::Add_Received_Entry_To_DHT()\n");
    }
}


void Main_Client::Add_Received_Entry_To_DHT(_160bitnumber ID)
{
    DHT_Single_Entry senderEntry;

    senderEntry.addr = server_IP;
    senderEntry.id = ID;
    senderEntry.port = server_Port;
    senderEntry.is_set = true;


    DHT::Update_Time(senderEntry);

}


void Main_Client::Ping_Received_Nodes_If_Not_File(char recvbuf[], int length, _160bitnumber file)
{
    if(!set_Up_Properly)
        return;

    for(int i=0; i<3;i++)
    {
        //Should be sorted but checking all anyway
        if(length >= 46+26*i)
        {

            _160bitnumber sentID;
            short unsigned int port;
            in_addr addr;
            memcpy((void*)&sentID, recvbuf+20+26*i, 20);
            memcpy((void*)&port, recvbuf+40+26*i, 2);
            memcpy((void*)&addr, recvbuf+42+26*i, 4);


            //Create a new thread and ping
            if(DHT::Is_Equal(file, sentID))
            {
                //Store file
                DHT_Single_Entry storing;
                storing.id = sentID;
                storing.addr = addr;
                storing.port = port;
                storing.is_set = true;
                DHT_Access::Store_File_ID(storing);
            }
            else
            {
                //Ping Node if its up
                Main_Client receivedClient(addr, port);
                receivedClient.Ping();

            }

        }


    }
}




void Main_Client::Ping_Received_Nodes(char recvbuf[], int length)
{
    if(!set_Up_Properly)
        return;


    for(int i=0; i<3;i++)
    {
        if(length >= 46+26*i)
        {

            _160bitnumber senderID;
            short unsigned int port;
            in_addr addr;
            memcpy((void*)&senderID, recvbuf+20+26*i, 20);
            memcpy((void*)&port, recvbuf+40+26*i, 2);
            memcpy((void*)&addr, recvbuf+42+26*i, 4);


            //Create a new thread and ping

            Main_Client receivedClient(addr, port);

            receivedClient.Ping();
        }


    }
}



int Main_Client::Store_File(DHT_Single_Entry file)
{
    if(!set_Up_Properly)
        return -1;

    int bufLen = 49;

    char sendBuf[bufLen];
    sendBuf[0] = 0x05;
    Add_Self(sendBuf);

    memcpy(sendBuf+23, (char*)&file.id, 20);
    memcpy(sendBuf+43, (char*)&file.port, 2);
    memcpy(sendBuf+45, (char*)&file.addr, 4);



    int iResult = send( server_Socket, sendBuf, bufLen, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);


    Shutdown_Connection_Gracefully();



    return 0;


}




int Main_Client::Find_File(_160bitnumber file)
{
    if(!set_Up_Properly)
        return -1;
    std::cout << "MainClient Find_File \n";
    char sendbuf[43];
    sendbuf[0] = 0x04;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&file, 20);



    int iResult = send( server_Socket, sendbuf, 43, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);

    Ping_Received_Nodes_If_Not_File(recvbuf, iResult, file);


    Shutdown_Connection_Gracefully();



    return 0;


}








int Main_Client::Find_Node(_160bitnumber node)
{
    if(!set_Up_Properly)
        return -1;

    char sendbuf[43];
    sendbuf[0] = 0x03;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&node, 20);



    int iResult = send( server_Socket, sendbuf, 43, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);

    Ping_Received_Nodes(recvbuf, iResult);


    Shutdown_Connection_Gracefully();



    return 0;


}


char *Main_Client::Get_Metadata_File(_160bitnumber fileid)
{
    if(!set_Up_Properly)
        return nullptr;

    char sendbuf[47];
    sendbuf[0] = 0x06;

    Add_Self(sendbuf);


    int n1 = -1;  //Asking for chunk[-1] which means the meta-data file
    memcpy(sendbuf+23, (char*)&n1, 4);
    memcpy(sendbuf+27, (char*)&fileid, 20);


    int iResult = send( server_Socket, sendbuf, 47, 0 );

    if (iResult == SOCKET_ERROR) {
        printf("MainClient::Get_MetaData_File send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return nullptr;
    }






    char buf[DEFAULT_BUFLEN];

    iResult = recv(server_Socket, buf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return nullptr;
    }
    Add_Received_Entry_To_DHT(buf, iResult);






    int Eight_MiB = 8000000;

    char *recvbuf;
    recvbuf = (char *) malloc(Eight_MiB + 4);

    int recvbuf_Count=4; //Leaves room for the length in the buffer
    //Get the data
    //If this errors out they don't have the file
    iResult = 1;
    while(iResult > 0)
    {
        iResult = recv(server_Socket, recvbuf+recvbuf_Count, DEFAULT_BUFLEN, 0);
        //std::cout << "MainClient::Get_MetaData_File received a message of length(for the file) " << iResult << "\n\n";
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(server_Socket);
            WSACleanup();
            return nullptr;
        }
        recvbuf_Count += iResult;

    }





    Shutdown_Connection_Gracefully();

    memcpy(recvbuf, (char*)&recvbuf_Count, 4); //Put the length in the buffer

    return recvbuf;


}










char *Main_Client::Get_File_Chunk(_160bitnumber fileid, int chunk)
{
    if(!set_Up_Properly)
        return nullptr;
    if(chunk < -1)
        return nullptr;

    char sendbuf[47];
    sendbuf[0] = 0x06;

    Add_Self(sendbuf);


    memcpy(sendbuf+23, (char*)&chunk, 4);
    memcpy(sendbuf+27, (char*)&fileid, 20);


    int iResult = send( server_Socket, sendbuf, 47, 0 );

    if (iResult == SOCKET_ERROR) {
        printf("MainClient::GetFileChunk send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return nullptr;
    }






    char buf[DEFAULT_BUFLEN];

    iResult = recv(server_Socket, buf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("MainClient::GetFileChunk Send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return nullptr;
    }
    Add_Received_Entry_To_DHT(buf, iResult);






    int Eight_MiB = 8000000 + 4;  // +4 for the length(recvBuf_Count)

    char *recvbuf;
    recvbuf = (char *) malloc(Eight_MiB);

    int recvbuf_Count=4; //4 Leaves room for the length in the buffer
    //Get the data
    //If this errors out they don't have the file
    iResult = 1;
    int totalRecievedBytes=0;
    while(iResult > 0)
    {
        iResult = recv(server_Socket, recvbuf+recvbuf_Count, DEFAULT_BUFLEN, 0);
        totalRecievedBytes += iResult;
        if (iResult == SOCKET_ERROR)
        {
            if(totalRecievedBytes == Eight_MiB)
            {
                std::cout << "totReceived = EightMiB - Continuing...\n";
                iResult = 0;
                continue;

            }
            printf("MainClient::GetFileChunk send failed with error: %d\n", WSAGetLastError());
            closesocket(server_Socket);
            WSACleanup();
            return nullptr;
        }
        recvbuf_Count += iResult;

    }





    Shutdown_Connection_Gracefully();

    memcpy(recvbuf, (char*)&recvbuf_Count, 4); //Put the length in the buffer

    return recvbuf;


}












int Main_Client::Find_Node_Recursive(_160bitnumber node, int lookup_Identifier)
{
    if(!set_Up_Properly)
        return -1;


    char sendbuf[43];
    sendbuf[0] = 0x03;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&node, 20);



    int iResult = send( server_Socket, sendbuf, 43, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    DHT_Single_Entry current_Connection = Add_Received_Entry_To_DHT_And_Return_Entry(recvbuf, iResult);

    three_DHT received_Nodes = Return_Received_Nodes(recvbuf, iResult);

    Shutdown_Connection_Gracefully();

    //Write this connection to lookup_DHT if they are closer then currently stored
    Minor_Functions::Add_To_Lookup_DHT(lookup_Identifier, current_Connection, node);

    //Check all received_Nodes if they are closer then currently stored - then do this lookup if so
    Minor_Functions::Do_Lookup_If_Closer(lookup_Identifier, received_Nodes, node);

    return 0;


}






int Main_Client::Find_File_Recursive(_160bitnumber fileID, int lookup_Identifier)
{
    if(!set_Up_Properly)
        return -1;

    std::cout << "MainClient Find_File_recursive \n";
    char sendbuf[43];
    sendbuf[0] = 0x04;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&fileID, 20);



    int iResult = send( server_Socket, sendbuf, 43, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }

    DHT_Single_Entry current_Connection = Add_Received_Entry_To_DHT_And_Return_Entry(recvbuf, iResult);




    three_DHT received_Nodes = Return_Received_Nodes(recvbuf, iResult);



    Shutdown_Connection_Gracefully();




    /*unsigned int a = 0;
    unsigned int b;
    memcpy( (char *)&b, (char *)&received_Nodes.entry[0].addr, 4);
    if(a == b)
        std::cout << "MainClient::Find_File_Recursive received_Nodes.entry[0].addr = 0.0.0.0\n"; */

    //Check for an entry for the correct ID as well as a IP of 0.0.0.0
    unsigned int a;
    memcpy((char *)&a, (char *) &received_Nodes.entry[0].addr, 4);
    if(DHT::Is_Equal(fileID, received_Nodes.entry[0].id) && a == 0)
    {
        std::cout << "File found!!!\n";
        //Add in this entry to the top and end the search

        received_Nodes.entry[0].addr = server_IP;
        Minor_Functions::Write_Single_Entry_To_DHT_Lookup(received_Nodes.entry[0], lookup_Identifier, fileID);

        return 0;
    }
    else
    {
        //std::cout << "They do not have the file!\nID:" << DHT::ID_To_String(received_Nodes.entry[0].id)
        //<< " with an addr of " << a << "\n\n";

    }

    //Write this connection to lookup_DHT if they are closer then currently stored
    Minor_Functions::Add_To_Lookup_DHT(lookup_Identifier, current_Connection, fileID);




    //Check all received_Nodes if they are closer then currently stored - then do this lookup if so
    Minor_Functions::Do_Lookup_If_Closer(lookup_Identifier, received_Nodes, fileID);


    //Needs to do something if the ID is the same as requested


    return 0;


}










DHT_Single_Entry Main_Client::Add_Received_Entry_To_DHT_And_Return_Entry(char recvbuf[], int length)
{
    if(!set_Up_Properly)
    {
        DHT_Single_Entry a;
        return a;
    }



    if(length >= 20)
    {


        _160bitnumber sender_Id;
        memcpy((void*)&sender_Id, recvbuf, 20);


        DHT_Single_Entry sender_DHT_Entry;
        sender_DHT_Entry.addr = server_IP;
        sender_DHT_Entry.id = sender_Id;
        sender_DHT_Entry.port = server_Port;
        sender_DHT_Entry.is_set = true;


        DHT::Update_Time(sender_DHT_Entry);

        return sender_DHT_Entry;

    }
    else
    {
        printf("Entry too short in MainClient::Add_Received_Entry_To_DHT_And_Return_Entry()\n");
        DHT_Single_Entry a;
        a.is_set = false;
        return a;
    }
}




three_DHT Main_Client::Return_Received_Nodes(char recvbuf[], int length)
{
    three_DHT recived_Nodes;
    if(!set_Up_Properly)
        return recived_Nodes;



    for(int i=0; i<3;i++)
    {
        if(length >= 46+26*i)
        {

            _160bitnumber sender_Id;
            short unsigned int port;
            in_addr addr;
            memcpy((void*)&sender_Id, recvbuf+20+26*i, 20);
            memcpy((void*)&port, recvbuf+40+26*i, 2);
            memcpy((void*)&addr, recvbuf+42+26*i, 4);

            recived_Nodes.entry[i].addr = addr;
            recived_Nodes.entry[i].port = port;
            recived_Nodes.entry[i].id = sender_Id;
            recived_Nodes.entry[i].is_set = true;



        }


    }

    return recived_Nodes;

}




int Main_Client::Ping()
{
    if(!set_Up_Properly)
        return -1;

    char *sendbuf = Message_Ping::Ping_Create_Request();
    int iResult = send( server_Socket, sendbuf, 23, 0 );
    delete[] sendbuf;


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }


    char recvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return 1;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(recvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);


    Shutdown_Connection_Gracefully();
    return 0;

}



Message Main_Client::Proxy(char nextCommandByte, char *message, int msgLen)
{
    Message failure;
    if(!set_Up_Properly)
        return failure;

    char *sendbuf;
    if(nextCommandByte == 6)
    {
        std::cout << "Next Command is to Get the file SHOULD NOT GET HERE!\n";
        return failure;
    }
    else if(nextCommandByte == 8)
    {
        std::cout << "Next Command is to Forward!\n";
        ChunkResponce a = Message_Proxy::Read_Chunk_6(message,msgLen);
        sendbuf = Message_Proxy::Create_Get_Chunk_Msg(a);
    }
    else
    {
        std::cout << "Next command should not exist ERROR Returning\n";
        return failure;
    }
    int iResult = send( server_Socket, sendbuf, 23+msgLen, 0 );
    delete[] sendbuf;


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return failure;
    }


    char pingrecvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(server_Socket, pingrecvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return failure;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(pingrecvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);


    //Receive Data and return it TODO
    int Eight_MiB = 8000000 + 100;  // +100 for some padding room

    char *recvbuf;
    recvbuf = (char *) malloc(Eight_MiB);

    int recvbuf_Count=0;
    iResult = 1;
    while(iResult > 0)
    {
        iResult = recv(server_Socket, recvbuf+recvbuf_Count, DEFAULT_BUFLEN, 0);
        if (iResult == SOCKET_ERROR)
        {
            if(recvbuf_Count == Eight_MiB)
            {
                std::cout << "totReceived = EightMiB - Continuing...\n";
                iResult = 0;
                continue;

            }
            printf("MainClient::GetFileChunk send failed with error: %d\n", WSAGetLastError());
            closesocket(server_Socket);
            WSACleanup();
            return failure;
        }
        recvbuf_Count += iResult;

    }

    Shutdown_Connection_Gracefully();

    Message msg;
    msg.message = recvbuf;
    msg.msgLength = recvbuf_Count;
    return msg;
}





Message Main_Client::Proxy_Get_Chunk(ChunkResponce info)
{
    Message failure;
    if(!set_Up_Properly)
        return failure;




    char *sendbuf = Message_Proxy::Create_Get_Chunk_Msg(info);
    int iResult = send( server_Socket, sendbuf, 54, 0 );
    delete[] sendbuf;


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return failure;
    }


    char pingrecvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(server_Socket, pingrecvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return failure;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(pingrecvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);


    //Receive Data and return it TODO
    int Eight_MiB = 8000000;

    char *recvbuf;
    recvbuf = (char *) malloc(Eight_MiB + 100);  // +100 for extra padding

    int recvbuf_Count=0;
    iResult = 1;
    while(iResult > 0)
    {
        iResult = recv(server_Socket, recvbuf+recvbuf_Count, DEFAULT_BUFLEN, 0);
        if (iResult == SOCKET_ERROR)
        {
            if(recvbuf_Count == Eight_MiB)
            {
                std::cout << "totReceived = EightMiB - Continuing...\n";
                iResult = 0;
                continue;

            }
            printf("MainClient::GetFileChunk send failed with error: %d\n", WSAGetLastError());
            closesocket(server_Socket);
            WSACleanup();
            return failure;
        }
        recvbuf_Count += iResult;

    }

    Shutdown_Connection_Gracefully();

    Message msg;
    msg.message = recvbuf;
    msg.msgLength = recvbuf_Count - 4;
    return msg;

}





Message Main_Client::Proxy_Get_Chunk_2(ChunkResponce info, DHT_Single_Entry connectThru)
{
    Message failure;
    if(!set_Up_Properly)
        return failure;



    char *chunkbuf = Message_Proxy::Create_Get_Chunk_Msg(info);
    char *sendbuf = Message_Proxy::Create_Forward_Msg(chunkbuf, 54, connectThru);




    int iResult = send( server_Socket, sendbuf, 54+30, 0 );
    delete[] sendbuf;
    delete[] chunkbuf;


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return failure;
    }


    char pingrecvbuf[DEFAULT_BUFLEN];
    std::cout << "Proxy_Get_Chunk_2 4\n";
    //Get the ping back
    iResult = recv(server_Socket, pingrecvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return failure;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(pingrecvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);
    std::cout << "Proxy_Get_Chunk_2 5\n";

    //Receive Data and return it TODO
    int Eight_MiB = 8000000;

    char *recvbuf;
    recvbuf = (char *) malloc(Eight_MiB + 100);  // +100 for extra padding

    int recvbuf_Count=0;
    iResult = 1;
    while(iResult > 0)
    {
        iResult = recv(server_Socket, recvbuf+recvbuf_Count, DEFAULT_BUFLEN, 0);
        if (iResult == SOCKET_ERROR)
        {
            if(recvbuf_Count == Eight_MiB)
            {
                std::cout << "totReceived = EightMiB - Continuing...\n";
                iResult = 0;
                continue;

            }
            printf("MainClient::GetFileChunk send failed with error: %d\n", WSAGetLastError());
            closesocket(server_Socket);
            WSACleanup();
            return failure;
        }
        recvbuf_Count += iResult;

    }

    Shutdown_Connection_Gracefully();

    Message msg;
    msg.message = recvbuf;
    msg.msgLength = recvbuf_Count - 4;
    return msg;

}




void Main_Client::Upload_File_Onion(_160bitnumber key, DHT_Single_Entry middlePeer, DHT_Single_Entry lastPeer, _160bitnumber fileID)
{
    if(!set_Up_Properly)
        return;

    char *sendbuf = Message_Keyed_Proxy::Create_Upload_Message(key, middlePeer, lastPeer, fileID);
    int iResult = send( server_Socket, sendbuf, 75, 0 );
    delete[] sendbuf;

    if (iResult == SOCKET_ERROR) {
        printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }


    char recvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Recv failed with error: %d in Main_Client::Upload_File_Onion\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(recvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);


    Shutdown_Connection_Gracefully();


    return;
}


void Main_Client::Proxy_Onion(_160bitnumber key, char *msg, int len)
{
    if(!set_Up_Properly)
        return;



    char *sendbuf = Message_Keyed_Proxy::Create_Upload_Message(msg, len, key);

    int iResult = SOCKET_ERROR;
    if(len == 75)
        iResult = send( server_Socket, sendbuf, 69, 0 );
    else if(len == 69)
        iResult = send( server_Socket, sendbuf, 63, 0 );

    delete[] sendbuf;

    if (iResult == SOCKET_ERROR) {
        printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }


    char recvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Recv failed with error: %d in Main_Client::Proxy_Onion\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(recvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);


    Shutdown_Connection_Gracefully();


    return;
}



void Main_Client::Get_Chunk_Onion(_160bitnumber key, _160bitnumber fileID, int chunk)
{
    if(!set_Up_Properly)
        return;


    char *sendbuf = Message_Keyed_Proxy::Create_Download_Message(key, fileID, chunk);


    int iResult = send( server_Socket, sendbuf, 67, 0 );

    delete[] sendbuf;

    if (iResult == SOCKET_ERROR) {
        printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }


    char recvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Recv failed with error: %d in Main_Client::Proxy_Onion\n", WSAGetLastError());
        closesocket(server_Socket);
        WSACleanup();
        return;
    }


    _160bitnumber ID = Message_Ping::Read_Ping_Responce(recvbuf, iResult);
    Add_Received_Entry_To_DHT(ID);


    Shutdown_Connection_Gracefully();


    return;
}








//INEFFICENT TODO
Main_Client::Main_Client(in_addr addr_In, unsigned short int port_In)
{
    char *addr = inet_ntoa(addr_In);
    char port[10];
    sprintf(port, "%d", port_In);

    //printf("Mainclient(%s:%s) being created\n\n",addr,port);
    //std::cout << "Making a client with IP=" << DHT::IP_To_String(addr_In) << " and port=" << port_In << "\n";


    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        set_Up_Properly=false;
        return;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port


    iResult = getaddrinfo(addr, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        set_Up_Properly=false;
        return;
    }

    // Attempt to connect to an address until one succeeds
    ptr=result;
    //for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            set_Up_Properly=false;
            return;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            //continue;
        }
        //break;
    //}

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        set_Up_Properly=false;
        return;
    }
    Main_Client::server_Socket = ConnectSocket;

    unsigned long tmpa  = inet_addr(addr);
    server_IP = (in_addr&)tmpa;

    server_Port = (unsigned short int)atoi(port);


}




Main_Client::Main_Client(const char *addr, const char *port)
{
    //printf("Mainclient(*addr, *port) being created\n\n");

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        set_Up_Properly=false;
        return;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(addr, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        set_Up_Properly=false;
        return;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            set_Up_Properly=false;
            return;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        set_Up_Properly=false;
        return;
    }
    Main_Client::server_Socket = ConnectSocket;

    unsigned long tmpa  = inet_addr(addr);
    server_IP = (in_addr&)tmpa;

    server_Port = (unsigned short int)atoi(port);



}

