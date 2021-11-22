

#ifndef __PAFT_MAINSERVER_H
#define __PAFT_MAINSERVER_H

#include <winsock.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct
{
    SOCKET client;
    sockaddr_in from;


} longsocket;

namespace paft
{
    class MainServer
    {
    public:
        static int Start_Server();


    };

}





#endif // __PAFT_MAINSERVER_H
