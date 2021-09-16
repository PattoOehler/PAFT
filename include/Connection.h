#ifndef __PAFT_CONNECTION_H
#define __PAFT_CONNECTION_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>

namespace paft{
    class Connection
    {
        public:
            Connection();
            virtual ~Connection();

            static void Recv_Command(LPVOID lpParam);
            static void Send_File(LPVOID lpParam);


    };
}

#endif // CONNECTION_H
