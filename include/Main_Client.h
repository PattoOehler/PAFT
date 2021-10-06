
#ifndef __PAFT_MAINCLIENT_H
#define __PAFT_MAINCLIENT_H
#include <winsock2.h>


namespace paft
{
    class MainClient{
    private:
        SOCKET Socket;

    public:
        int GetFile(char *filename);
        int Ping();
        MainClient(const char *addr,const char *port);




    };

}





#endif // __PAFT_MAINCLIENT_H
