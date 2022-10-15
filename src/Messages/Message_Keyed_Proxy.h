#ifndef __PAFT_MESSAGE_KEYED_PROXY
#define __PAFT_MESSAGE_KEYED_PROXY

#include "../DHT/DHT.h"

typedef struct
{
    in_addr sendToAddr;
    unsigned short int sendToPort=0;
    _160bitnumber key;
    _160bitnumber fileID;

    int code; //-1=Failure 0=Success 1=Endpoint

} Keyed_Proxy_Responce; //Todo rename this


typedef struct
{
    _160bitnumber key;
    _160bitnumber fileID;
    int chunk;

    int code; //-1=Failure 0=Success

} Keyed_Download_Responce;



namespace paft
{
    class Message_Keyed_Proxy{
    private:


    public:
        static char *Create_Upload_Message(_160bitnumber key, DHT_Single_Entry middlePeer, DHT_Single_Entry lastPeer, _160bitnumber fileID);
        static char *Create_Upload_Message(char *message, int messageLen, _160bitnumber key);
        static Keyed_Proxy_Responce Read_Upload_Message(char *message, int length);

        static char *Create_Download_Message(_160bitnumber key, _160bitnumber fileID, int chunk);
        static Keyed_Download_Responce Read_Download_Message(char *message, int length);

    };

}

#endif // __PAFT_MESSAGE_KEYED_PROXY
