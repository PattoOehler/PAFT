

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
    private:

    public:
        static unsigned short int portNum;

        static int Start_Server();


        static void Set_Port_Num(unsigned short int portNum);
        static unsigned short int Get_Port_Num();
    };

}





#endif // __PAFT_MAINSERVER_H
