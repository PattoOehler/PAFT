
#ifndef __PAFT_MAINCLIENT_H
#define __PAFT_MAINCLIENT_H
#include <winsock2.h>
#include "../DHT/DHT.h"

namespace paft
{
    class MainClient{
    private:
        SOCKET Socket;
        in_addr Server_IP;
        unsigned short int Server_Port;

        bool setUpProperly=true;

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
        void Ping_Received_Nodes_If_Not_File(char recvbuf[], int length, _160bitnumber file);
        int Find_File(_160bitnumber file);
        int Store_File(DHT_Single_Entry file);
        char *Get_MetaData_File(_160bitnumber fileid);
        char *GetFileChunk(_160bitnumber fileid, int chunk);

        int Find_Node_Recursive(_160bitnumber node, int lookup_Identifier);
        three_DHT Return_Received_Nodes(char recvbuf[], int length);
        DHT_Single_Entry Add_Received_Entry_To_DHT_And_Return_Entry(char recvbuf[], int length);
        int Find_File_Recursive(_160bitnumber fileID, int lookup_Identifier);

    };

}





#endif // __PAFT_MAINCLIENT_H
