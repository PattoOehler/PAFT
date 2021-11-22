#ifndef __PAFT_CONNECTION_H
#define __PAFT_CONNECTION_H

#include <winsock.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "MainServer.h"



namespace paft{
    class Connection
    {
    private:
        static void Run_Proper_Command(char *buf, longsocket long_client, int len);
        static void Update_DHT(longsocket client, char recvdata[]);


    public:
        Connection();
        virtual ~Connection();

        static void Handle_Client(LPVOID lpParam);
        static void Send_File(LPVOID lpParam);
        static void Ping(LPVOID lpParam);
        static void Lookup_Peer(LPVOID lpParam, char buf[], int len);
        static void Lookup_File(LPVOID lpParam, char buf[], int len);
        static void Store_File(LPVOID lpParam, char buf[], int len);


    };
}

#endif // CONNECTION_H
