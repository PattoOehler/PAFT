#ifndef __PAFT_MESSAGE_PING
#define __PAFT_MESSAGE_PING

#include "../DHT/DHT.h"

namespace paft
{
    class Message_Ping{
    private:


    public:

        static char *Ping_Create_Request();

        static _160bitnumber Read_Ping_Responce(char *responce, int len);

        static char *Create_Ping_Responce();
    };

}

#endif // __PAFT_MESSAGES=_PING
