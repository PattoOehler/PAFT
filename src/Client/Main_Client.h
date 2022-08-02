
#ifndef __PAFT_MAIN_CLIENT_H
#define __PAFT_MAIN_CLIENT_H
#include <winsock2.h>
#include "../DHT/DHT.h"
#include "../Messages/Message_Proxy.h"

typedef struct
{
    char *message;
    int msgLength = 0;

} Message;


namespace paft
{
    class Main_Client{
    private:
        SOCKET server_Socket;
        in_addr server_IP;
        unsigned short int server_Port;

        bool set_Up_Properly=true;

    public:
        int Ping();
        Main_Client(const char *addr,const char *port);
        Main_Client(in_addr addr, unsigned short int port);
        int Find_Node(_160bitnumber node);
        void Add_Self(char buf[]);
        void Shutdown_Connection_Gracefully();
        void Add_Received_Entry_To_DHT(char recvbuf[], int length);
        void Add_Received_Entry_To_DHT(_160bitnumber ID);
        void Ping_Received_Nodes(char recvbuf[], int length);
        void Ping_Received_Nodes_If_Not_File(char recvbuf[], int length, _160bitnumber file);
        int Find_File(_160bitnumber file);
        int Store_File(DHT_Single_Entry file);
        char *Get_Metadata_File(_160bitnumber fileid);
        char *Get_File_Chunk(_160bitnumber fileid, int chunk);

        int Find_Node_Recursive(_160bitnumber node, int lookup_Identifier);
        three_DHT Return_Received_Nodes(char recvbuf[], int length);
        DHT_Single_Entry Add_Received_Entry_To_DHT_And_Return_Entry(char recvbuf[], int length);
        int Find_File_Recursive(_160bitnumber fileID, int lookup_Identifier);
        static in_addr NULL_Addr();


        Message Proxy(char nextCommandByte, char *message, int msgLen);
        Message Proxy_Get_Chunk(ChunkResponce info);
        Message Proxy_Get_Chunk_2(ChunkResponce info, DHT_Single_Entry connectThru);
    };

}





#endif // __PAFT_MAIN_CLIENT_H
