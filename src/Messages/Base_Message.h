
#ifndef __PAFT_BASE_MESSAGE
#define __PAFT_BASE_MESSAGE

#include "../DHT/DHT.h"


typedef struct
{
    _160bitnumber ID;
    unsigned short int port;

} Base_Return;



namespace paft
{
    class Base_Message {
    private:


    public:
        static void Add_Base(  char * &message, int len);
        static Base_Return Read_Base( char *message, int len);
    };

}

#endif // __PAFT_BASE_MESSSAGE
