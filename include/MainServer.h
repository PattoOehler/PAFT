

#ifndef __PAFT_MAINSERVER_H
#define __PAFT_MAINSERVER_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>


namespace paft
{
    class MainServer
    {
    public:
        static int Start_Server(int a);
        //static void receive_cmds(LPVOID lpParam);
        //static int Send_File(LPVOID lpParam);

    };

}





#endif // __PAFT_MAINSERVER_H
