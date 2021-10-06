
#include "../include/Main_Client.h"

#include <iostream>

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
/*pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
 Unknown if it does anything
*/
#define DEFAULT_BUFLEN 512



using namespace paft;



int MainClient::Ping()
{
    const char *sendbuf = "\x02Pinging bro\0";
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
    std::cout << "Got a ping back saying " << recvbuf << std::endl;

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


    return 0;

}

int MainClient::GetFile(char *filename)
{

    const char *sendbuf = "\x01Hello.txt\x00";  //TODO REMOVE WITH filename

    Sleep(1);
    printf("SENDING!!---------------------------------------------\n");


    // Send a buffer telling the server to send a file
    int iResult = send( Socket, sendbuf, (int)strlen(sendbuf), 0 );
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
    fp = fopen("CLIENT_FILES/asdf.txt", "a+b");  // TODO remove with filename
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


    return 0;
}

MainClient::MainClient(const char *addr, const char *port)
{

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


}

