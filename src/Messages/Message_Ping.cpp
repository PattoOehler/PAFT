#include "Message_Ping.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "Base_Message.h"

#include <iostream>

using namespace paft;


/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte
1       20      Own ID (for servers DHT)
21      2       Own Port (for servers DHT)
*/
char *Message_Ping::Ping_Create_Request()
{
    char *pingMessage = new char[23];
    pingMessage[0] = 0x01; //Ping Command

    Base_Message::Add_Base(pingMessage, 23); // Adds Own ID and Own Port

    return pingMessage;
}




/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       20      Servers ID
*/
_160bitnumber Message_Ping::Read_Ping_Responce(char *responce, int len)
{
    if(len == 20)
    {
        _160bitnumber sender_Id;
        memcpy((void*)&sender_Id, responce, 20);

        return sender_Id;
    }
    else
    {
        std::cout << "\n\n\n\nERROR: Improper length(" << len << ") in Message_Ping::Read_Ping_Responce\n\n\n\n";
        return DHT_Access::Get_Self_ID();
    }

}

/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       20      Own ID
*/
char *Message_Ping::Create_Ping_Responce()
{
    char *pingResponce = new char[20];

    _160bitnumber self = DHT_Access::Get_Self_ID();
    memcpy(pingResponce, (char*)&self, 20);

    return pingResponce;


}
