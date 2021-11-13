
#ifndef __PAFT_MAINCLIENT_H
#define __PAFT_MAINCLIENT_H
#include <winsock2.h>
#include "DHT.h"

namespace paft
{
    class MainClient{
    private:
        SOCKET Socket;
        in_addr Server_IP;
        unsigned short int Server_Port;

    public:
        int GetFile(char *filename);
        int Ping();
        MainClient(const char *addr,const char *port);
        MainClient(in_addr addr, unsigned short int port);
        int Find_Node(_160bitnumber node);
        void Add_Self(char buf[]);
        void Shutdown_Connection_Gracefully();
        void Add_Received_Entry_To_DHT(char recvbuf[], int length);
        void Ping_Received_Nodes(char recvbuf[], int length);

    };

}





#endif // __PAFT_MAINCLIENT_H
