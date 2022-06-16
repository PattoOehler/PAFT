
#include "Main_Client.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "Minor_Functions.h"
#include "../DHT/DHT_Lookup.h"

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

    char recvbuf[DEFAULT_BUFLEN];
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
        iResult = recv(server_Socket, recvbuf, DEFAULT_BUFLEN, 0);
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
    _160bitnumber self = DHT_Access::Get_SELF_ID();
    memcpy(buf+1, (char*)&self, 20); // 160/8=20
    short unsigned int port = DHT_Access::Get_Self_Port();
    memcpy(buf+21, (char*)&port, 2);

}

/*
void MainClient::Store_File_To_DHT(char recvbuf[], int length)
{

    if(length >= 49)
    {



        DHT_Single_Entry file_To_Store;

        memcpy((void*)&file_To_Store.id, recvbuf+20, 20);

        file_To_Store.addr = Server_IP;
        file_To_Store.port = Server_Port;
        file_To_Store.is_set = true;

        DHT::Id(file_To_Store);


    }
    else
    {
        printf("Entry too short in MainClient::Store_File_To_DHT()\n");
    }

}
*/


void Main_Client::Add_Received_Entry_To_DHT(char recvbuf[], int length)
{
    if(!set_Up_Properly)
        return;

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

    }
    else
    {
        printf("Entry too short in MainClient::Add_Received_Entry_To_DHT()\n");
    }
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

            _160bitnumber sent_Id;
            short unsigned int port;
            in_addr addr;
            memcpy((void*)&sent_Id, recvbuf+20+26*i, 20);
            memcpy((void*)&port, recvbuf+40+26*i, 2);
            memcpy((void*)&addr, recvbuf+42+26*i, 4);


            //Create a new thread and ping
            if(DHT::IsEqual(file, sent_Id))
            {
                //Store file
                DHT_Single_Entry storing;
                storing.id = sent_Id;
                storing.addr = addr;
                storing.port = port;
                storing.is_set = true;
                DHT_Access::Store_FileId(storing);
            }
            else
            {
                //Ping Node if its up
                Main_Client received_Client(addr, port);
                received_Client.Ping();

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

            _160bitnumber sender_Id;
            short unsigned int port;
            in_addr addr;
            memcpy((void*)&sender_Id, recvbuf+20+26*i, 20);
            memcpy((void*)&port, recvbuf+40+26*i, 2);
            memcpy((void*)&addr, recvbuf+42+26*i, 4);


            //Create a new thread and ping

            Main_Client received_Client(addr, port);

            received_Client.Ping();
        }


    }
}



int Main_Client::Store_File(DHT_Single_Entry file)
{
    if(!set_Up_Properly)
        return -1;

    int bufLen = 49;

    char sendbuf[bufLen];
    sendbuf[0] = 0x05;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&file.id, 20);
    memcpy(sendbuf+43, (char*)&file.port, 2);
    memcpy(sendbuf+45, (char*)&file.addr, 4);



    int iResult = send( server_Socket, sendbuf, bufLen, 0 );


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


char *Main_Client::Get_MetaData_File(_160bitnumber fileid)
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
    recvbuf = (char *) malloc(Eight_MiB);

    int recvbuf_Count=4; //Leaves room for the length in the buffer
    //Get the data
    //If this errors out they don't have the file
    iResult = 1;
    while(iResult > 0)
    {
        iResult = recv(server_Socket, recvbuf+recvbuf_Count, DEFAULT_BUFLEN, 0);
        std::cout << "MainClient::Get_MetaData_File received a message of length(for the file) " << iResult << "\n\n";
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
    if(chunk < 0)
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
        std::cout << "MainClient::GetFileChunk received a message of length(for the file) " << iResult << " << total " << totalRecievedBytes << "\n";
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
    if(DHT::IsEqual(fileID, received_Nodes.entry[0].id) && a == 0)
    {
        std::cout << "They are saying that they have the file!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        //Add in this entry to the top and end the search

        received_Nodes.entry[0].addr = server_IP;
        Minor_Functions::Write_Single_Entry_To_DHT_Lookup(received_Nodes.entry[0], lookup_Identifier, fileID);

        return 0;
    }
    else
    {
        std::cout << "They DO NOT HAVE THE FILE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nID:";
        DHT::Print_ID(received_Nodes.entry[0].id);
        std::cout << " with an addr of " << a << "\n\n";

    }

    //Write this connection to lookup_DHT if they are closer then currently stored
    Minor_Functions::Add_To_Lookup_DHT(lookup_Identifier, current_Connection, fileID);


    //TEST TODO ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

    char sendbuf[23];
    sendbuf[0] = 0x02;



    Add_Self(sendbuf);

    int iResult = send( server_Socket, sendbuf, 23, 0 );


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

    Add_Received_Entry_To_DHT(recvbuf, iResult);


    Shutdown_Connection_Gracefully();
    return 0;

}



//INCREADIBLY INEFFICENT TODO
Main_Client::Main_Client(in_addr addr_In, unsigned short int port_In)
{
    char *addr = inet_ntoa(addr_In);
    char port[10];
    sprintf(port, "%d", port_In);

    printf("Mainclient(%s:%s) being created\n\n",addr,port);

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
    printf("Mainclient(*addr, *port) being created\n\n");

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

