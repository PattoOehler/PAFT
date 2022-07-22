#include "Base_Message.h"

#include "../DHT/DHT_Access.h"

#include <iostream>

using namespace paft;


/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte
1       20      Own ID (for servers DHT)
21      2       Own Port (for servers DHT)
.........       Extra data
*/
void Base_Message::Add_Base( char * &message, int len)
{
    if(len >= 23)
    {
        _160bitnumber self = DHT_Access::Get_Self_ID();
        memcpy(message+1, (char*)&self, 20);

        short unsigned int port = DHT_Access::Get_Self_Port();
        memcpy(message+21, (char*)&port, 2);
    }
}


/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte ***IGNORE***
1       20      Own ID (for servers DHT)
21      2       Own Port (for servers DHT)
.........       Extra data
*/
Base_Return Base_Message::Read_Base( char *message, int len)
{
    Base_Return current;
    if(len >= 23)
    {
        memcpy((char *)&current.ID, message+1, 20);
        memcpy((char *)&current.port, message+21, 2);
    }
    else
    {
        std::cout << "\n\n\n\nERROR in Base_Message::Read_Base len=" << len << "!!!!!\n\n\n";
    }
    return current;

}
