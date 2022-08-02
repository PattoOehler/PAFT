
#include "Message_Proxy.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "Base_Message.h"

#include <iostream>

using namespace paft;


/*  FORMAT BASE
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte == 8
1       20      Sender ID (for DHT)
21      2       Sender Port (for DHT)
                Below is not able to be read for the next position
23      1       Command byte to forward
24+     ?       Data for the command
*/


/*  FORMAT 6 Get Chunk
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte == 8
1       20      Sender ID (for DHT)
21      2       Sender Port (for DHT)
23      1       Command byte to forward == 6
24      4       Address to send to
28      2       Port to send to
30      20      ID of the file
50      4       Chunk ID int
*/


/*  FORMAT 7 Find File Network
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte == 8
1       20      Sender ID (for DHT)
21      2       Sender Port (for DHT)
23      1       Command byte to forward == 7
24      20      ID of the file
*/



/*  FORMAT 8 Forward
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte == 8
1       20      Sender ID (for DHT)
21      2       Sender Port (for DHT)
23      1       Command byte to forward == 8
24      4       Address to send to
28      2       Port to send to
30+     ?       Message
*/


char *Message_Proxy::Create_Base_Responce(char nextCommandByte, char *message, int messageLen)
{
    if(messageLen < 0 )
        return NULL;

    char *returnmssg = new char[23+messageLen];
    returnmssg[0] = nextCommandByte;

    Base_Message::Add_Base(returnmssg, 23); // Adds Own ID and Own Port
    memcpy(returnmssg+23, message, messageLen);

    return returnmssg;
}


BaseResponce Message_Proxy::Read_Base(char *responce, int len)
{
    if(len >= 29)
    {
        BaseResponce resp;
        memcpy((char *)&resp.senderID,   responce+1, 20);
        memcpy((char *)&resp.senderPort, responce+21, 2);
        memcpy((char *)&resp.forwardCommandByte, responce+23, 1);

        resp.isSet=true;

        return resp;
    }
    else
    {
        std::cout << "\n\n\n\nERROR: Improper length(" << len << ") in Message_Proxy::Read_Base\n\n\n\n";
        BaseResponce resp;
        return resp;
    }

}


Proxy_Responce Message_Proxy::Read_Proxy_8(char *responce, int len)
{
    if(len >= 30)
    {
        Proxy_Responce resp;
        memcpy((char *)&resp.sendToAddr,   responce+24, 4);
        memcpy((char *)&resp.sendToPort,   responce+28, 2);


        return resp;
    }
    else
    {
        std::cout << "\n\n\n\nERROR: Improper length(" << len << ") in Message_Proxy::Read_Proxy_8\n\n\n\n";
        Proxy_Responce resp;
        return resp;
    }

}

ChunkResponce Message_Proxy::Read_Chunk_6(char *responce, int len)
{
    if(len >= 54)
    {
        ChunkResponce resp;
        memcpy((char *)&resp.sendToAddr,   responce+24, 4);
        memcpy((char *)&resp.sendToPort,   responce+28, 2);
        memcpy((char *)&resp.fileID, responce + 30, 20);
        memcpy((char *)&resp.chunkID, responce + 50, 4);
        return resp;
    }
    else
    {
        std::cout << "\n\n\n\nERROR: Improper length(" << len << ") in Message_Proxy::Read_Chunk_6\n\n\n\n";
        ChunkResponce resp;
        return resp;
    }

}


char *Message_Proxy::Create_Get_Chunk_Msg(ChunkResponce info)
{

    char *returnmsg = new char[54];
    returnmsg[0] = 8;
    Base_Message::Add_Base(returnmsg, 23); // Adds Own ID and Own Port
    returnmsg[23] = 6;

    memcpy(returnmsg+24, (char *)&info.sendToAddr, 4);
    memcpy(returnmsg+28, (char *)&info.sendToPort, 2);
    memcpy(returnmsg+30, (char *)&info.fileID, 20);
    memcpy(returnmsg+50, (char *)&info.chunkID, 4);

    return returnmsg;
}

/*  FORMAT 8 Forward
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte == 8
1       20      Sender ID (for DHT)
21      2       Sender Port (for DHT)
23      1       Command byte to forward == 8
24      4       Address to send to
28      2       Port to send to
30+     ?       Message
*/

char *Message_Proxy::Create_Forward_Msg(char *msg, int len, DHT_Single_Entry next)
{

    char *returnmsg = new char[30+len];
    returnmsg[0] = 8;
    Base_Message::Add_Base(returnmsg, 23); // Adds Own ID and Own Port
    returnmsg[23] = 8;

    memcpy(returnmsg+24, (char *)&next.addr, 4);
    memcpy(returnmsg+28, (char *)&next.port, 2);
    memcpy(returnmsg+30, msg, len);

    return returnmsg;
}


