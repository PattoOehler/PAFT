#include "Connection.h"

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
        std::cout << "filesize = " << filesize << " and counter*512 = " << 512*counter << std::endl;
        Sleep(10);
        if(filesize > 512*counter)
            std::cout << "Hurray we going again in the server\n";

    }

    Sleep(20);
    fclose(asdf);

    //return 0;
}




void Connection::Recv_Command(LPVOID lpParam)
{

    if((SOCKET)lpParam == INVALID_SOCKET)
        printf("INVALID SOCKET\n");

    printf("Socket Value of %i\n", (SOCKET)lpParam);

    printf("Recv_Command \n");
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
        //printf("Hit the while loop\n");
        res = recv(current_client,buf,sizeof(buf),0); // recv cmds
        Sleep(10);

        if(res == 0)
        {
            printf("Res=0!!! Error\n");
            //MessageBox(0,"error","error",MB_OK);
            closesocket(current_client);
            ExitThread(0);
        }

        printf("%i\n", WSAGetLastError());
        if(buf[0] == 0x01)
        {
            //Client is asking for file
            printf("asdf\n");
            std::cout << "Client is asking for a file named " << buf +1 << std::endl; //buf[0] is the command byte
            //strcpy(sendData,"1234567891011121314151617181920");
            Connection::Send_File((LPVOID)current_client);
            shutdown(current_client, SD_SEND);

        }
        else
        {
            printf("Invalid cmd %x\n", buf[1]);
            strcpy(sendData,"Invalid cmd\n");
            Sleep(10);
            send(current_client,sendData,sizeof(sendData),0);

        }

        // clear buffers
        strcpy(sendData,"");
        strcpy(buf,"");
   }
}
