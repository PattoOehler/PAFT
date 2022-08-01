
#ifndef __PAFT_MESSAGE_PROXY
#define __PAFT_MESSAGE_PROXY

#include "../DHT/DHT.h"

typedef struct
{
    in_addr sendToAddr;
    unsigned short int sendToPort=0;


} Proxy_Responce;

typedef struct
{
    char commandByte;
    char *message;
    int messageLength;

} CommandLayerResponce;


typedef struct
{
    _160bitnumber senderID;
    unsigned short int senderPort=0;

    char forwardCommandByte;
    bool isSet=false;

} BaseResponce;


typedef struct
{
    in_addr sendToAddr;
    unsigned short int sendToPort=0;
    _160bitnumber fileID;
    int chunkID;


} ChunkResponce;


namespace paft
{
    class Message_Proxy{
    private:


    public:

        static char *Create_Base_Responce(char nextCommandByte, char *message, int messageLen);

        static BaseResponce Read_Base(char *responce, int len);
        static Proxy_Responce Read_Proxy_8(char *responce, int len);
        static ChunkResponce Read_Chunk_6(char *responce, int len);
        static char * Create_Get_Chunk_Msg(ChunkResponce info);
    };

}

#endif // __PAFT_MESSAGE_PROXY
