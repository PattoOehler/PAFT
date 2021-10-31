#ifndef __PAFT_CONNECTION_H
#define __PAFT_CONNECTION_H


#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include "MainServer.h"



namespace paft{
    class Connection
    {
    private:
        static void Run_Proper_Command(char *buf, longsocket long_client);
        static void Update_DHT(longsocket client, char recvdata[]);

    public:
        Connection();
        virtual ~Connection();

        static void Handle_Client(LPVOID lpParam);
        static void Send_File(LPVOID lpParam);
        static void Ping(LPVOID lpParam);


    };
}

#endif // CONNECTION_H
