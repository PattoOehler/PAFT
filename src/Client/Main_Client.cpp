
#include "Main_Client.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"

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
#define DEFAULT_BUFLEN 512



using namespace paft;


void MainClient::Shutdown_Connection_Gracefully()
{
    char recvbuf[DEFAULT_BUFLEN];
    // shutdown the connection since no more data will be sent
    int iResult = shutdown(Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return;
    }



    // Receive until the peer closes the connection
    do {
        iResult = recv(Socket, recvbuf, DEFAULT_BUFLEN, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(Socket);
    WSACleanup();



}



void MainClient::Add_Self(char buf[])
{
    _160bitnumber self = DHT_Access::Get_SELF();
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

        DHT::Store_FileId(file_To_Store);


    }
    else
    {
        printf("Entry too short in MainClient::Store_File_To_DHT()\n");
    }

}
*/


void MainClient::Add_Received_Entry_To_DHT(char recvbuf[], int length)
{
    if(length >= 20)
    {


        _160bitnumber sender_Id;
        memcpy((void*)&sender_Id, recvbuf, 20);


        DHT_Single_Entry sender_DHT_Entry;
        sender_DHT_Entry.addr = Server_IP;
        sender_DHT_Entry.id = sender_Id;
        sender_DHT_Entry.port = Server_Port;
        sender_DHT_Entry.is_set = true;


        DHT::Update_Time(sender_DHT_Entry);

    }
    else
    {
        printf("Entry too short in MainClient::Add_Received_Entry_To_DHT()\n");
    }
}


void MainClient::Ping_Received_Nodes_If_Not_File(char recvbuf[], int length, _160bitnumber file)
{


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
                MainClient received_Client(addr, port);
                received_Client.Ping();

            }

        }


    }
}




void MainClient::Ping_Received_Nodes(char recvbuf[], int length)
{

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

            MainClient received_Client(addr, port);

            received_Client.Ping();
        }


    }
}



int MainClient::Store_File(DHT_Single_Entry file)
{
    int bufLen = 49;

    char sendbuf[bufLen];
    sendbuf[0] = 0x05;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&file.id, 20);
    memcpy(sendbuf+43, (char*)&file.port, 2);
    memcpy(sendbuf+45, (char*)&file.addr, 4);



    int iResult = send( Socket, sendbuf, bufLen, 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);


    Shutdown_Connection_Gracefully();



    return 0;


}




int MainClient::Find_File(_160bitnumber file)
{

    char sendbuf[43];
    sendbuf[0] = 0x04;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&file, 20);



    int iResult = send( Socket, sendbuf, (int)strlen(sendbuf), 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);

    Ping_Received_Nodes_If_Not_File(recvbuf, iResult, file);


    Shutdown_Connection_Gracefully();



    return 0;


}








int MainClient::Find_Node(_160bitnumber node)
{

    char sendbuf[43];
    sendbuf[0] = 0x03;
    Add_Self(sendbuf);

    memcpy(sendbuf+23, (char*)&node, 20);



    int iResult = send( Socket, sendbuf, (int)strlen(sendbuf), 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN];
    iResult = recv(Socket, recvbuf, DEFAULT_BUFLEN, 0);

    if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);

    Ping_Received_Nodes(recvbuf, iResult);


    Shutdown_Connection_Gracefully();



    return 0;


}

int MainClient::Ping()
{

    char sendbuf[23];
    sendbuf[0] = 0x02;

    Add_Self(sendbuf);

    int iResult = send( Socket, sendbuf, (int)strlen(sendbuf), 0 );


    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }


    char recvbuf[DEFAULT_BUFLEN];

    //Get the ping back
    iResult = recv(Socket, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    Add_Received_Entry_To_DHT(recvbuf, iResult);



    Shutdown_Connection_Gracefully();

    return 0;

}




//DEPRECIATED
int MainClient::GetFile(char *filename)
{
    // \x01 is the command byte
    //const char *sendbuf = "\x01Hello.txt\x00";  //TODO REMOVE WITH filename

    char command[21];
    command[0] = '\x01';
    _160bitnumber self = DHT_Access::Get_SELF();
    memcpy(command+1, (char*)&self, 20); // 160/8=20

    std::cout << self.top << ' ' << self.mid << ' ' << self.bot << '\n';


    Sleep(1);
    printf("SENDING!!---------------------------------------------\n");


    // Send a buffer telling the server to send a file
    int iResult = send( Socket, command, (int)strlen(command), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    //std::cout << "Output bytes = " << iResult << std::endl;
    Sleep(1);

    char recvbuf[DEFAULT_BUFLEN];
    int res=0;
    FILE *fp;
    fp = fopen("CLIENT_FILES/asdf.txt", "w+b");  // TODO remove with filename
    while(true)
    {
        // Getting file loop
        res = recv(Socket,recvbuf,sizeof(recvbuf),0); // recv file
        std::cout << "RES: " << res << std::endl;
        Sleep(1);

        fwrite(recvbuf, res, 1, fp);

        if(res != 512)
            break;



    }
    fclose(fp);





    // shutdown the connection since no more data will be sent
    iResult = shutdown(Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(Socket, recvbuf, DEFAULT_BUFLEN, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(Socket);
    WSACleanup();


    std::cout << "Exited the connection on the client side";
    return 0;
}




//INCREADIBLY INEFFICENT TODO
MainClient::MainClient(in_addr addr_In, unsigned short int port_In)
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
        exit(1);
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
        exit(1);
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
            exit(1);
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
        exit(1);
    }
    MainClient::Socket = ConnectSocket;

    unsigned long tmpa  = inet_addr(addr);
    Server_IP = (in_addr&)tmpa;

    Server_Port = (unsigned short int)atoi(port);


}




MainClient::MainClient(const char *addr, const char *port)
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
        exit(1);
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
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            exit(1);
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
        exit(1);
    }
    MainClient::Socket = ConnectSocket;

    unsigned long tmpa  = inet_addr(addr);
    Server_IP = (in_addr&)tmpa;

    Server_Port = (unsigned short int)atoi(port);



}

