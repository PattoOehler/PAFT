#include "Message_Keyed_Proxy.h"

#include "Base_Message.h"


using namespace paft;





/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte 0x09
1       20      Own ID (for servers DHT)
21      2       Own Port (for servers DHT)
23      20      Key
43      4       The 2nd/middle peer IP
47      2       The 2nd/middle peer port
49      4       The 3rd/last peer IP
53      2       The 3rd/last peer port
55      20      The file ID to be uploaded
*/
char *Message_Keyed_Proxy::Create_Upload_Message(_160bitnumber key, DHT_Single_Entry middlePeer, DHT_Single_Entry lastPeer, _160bitnumber fileID)
{
    char *returnmsg = new char[75];
    returnmsg[0] = 0x09; //Ping Command

    Base_Message::Add_Base(returnmsg, 23); // Adds Own ID and Own Port

    memcpy(returnmsg+23, (char *)&key, 20);
    memcpy(returnmsg+43, (char *)&middlePeer.addr, 4);
    memcpy(returnmsg+47, (char *)&middlePeer.port, 2);
    memcpy(returnmsg+49, (char *)&lastPeer.addr, 4);
    memcpy(returnmsg+53, (char *)&lastPeer.port, 2);
    memcpy(returnmsg+55, (char *)&fileID, 20);

    return returnmsg;
}


/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte 0x09
1       20      Own ID (for servers DHT)
21      2       Own Port (for servers DHT)
23      20      Key
43      26-32   Remaining info(hidden)
*/
char *Message_Keyed_Proxy::Create_Upload_Message(char *message, int messageLen, _160bitnumber key)
{
    //Message should be equal to 75 or 69 depending if you are the first in the chain or the second
    if(messageLen == 75)
    {
        char *returnmsg = new char[69];
        returnmsg[0] = 0x09; //Ping Command

        Base_Message::Add_Base(returnmsg, 23); // Adds Own ID and Own Port

        memcpy(returnmsg+23, (char *)&key, 20);
        memcpy(returnmsg+43, message+49, 26);
        return returnmsg;
    }
    else if(messageLen == 69)
    {
        char *returnmsg = new char[63];
        returnmsg[0] = 0x09; //Ping Command

        Base_Message::Add_Base(returnmsg, 23); // Adds Own ID and Own Port

        memcpy(returnmsg+23, (char *)&key, 20);
        memcpy(returnmsg+43, message+49, 20);
        return returnmsg;

    }
    else
    {
        return nullptr;
    }



}






Keyed_Proxy_Responce Message_Keyed_Proxy::Read_Upload_Message(char *message, int length)
{
    Keyed_Proxy_Responce info;
    info.code = -1;

    if(length == 75 || length==69)
    {
        memcpy((char *)&info.key, message+23, 20);
        memcpy((char *)&info.sendToAddr, message+43, 4);
        memcpy((char *)&info.sendToPort, message+47, 2);
        info.code = 0;
    }
    if(length == 63)
    {
        memcpy((char *)&info.key, message+23, 20);
        memcpy((char *)&info.fileID, message+43, 20);

        info.code = 1;
    }

    return info;
}








/*  FORMAT
OFFSET  LENGTH  DESCRIPTION
0       1       Command Byte 0x09
1       20      Own ID (for servers DHT)
21      2       Own Port (for servers DHT)
23      20      Key
43      20      The file ID to be uploaded
63      4       The chunk requested
*/
char *Message_Keyed_Proxy::Create_Download_Message(_160bitnumber key, _160bitnumber fileID, int chunk)
{
    char *returnmsg = new char[67];
    returnmsg[0] = 0x0a; //Ping Command

    Base_Message::Add_Base(returnmsg, 23); // Adds Own ID and Own Port

    memcpy(returnmsg+23, (char *)&key, 20);
    memcpy(returnmsg+43, (char *)&fileID, 20);
    memcpy(returnmsg+63, (char *)&chunk, 4);

    return returnmsg;
}






Keyed_Download_Responce Message_Keyed_Proxy::Read_Download_Message(char *message, int length)
{
    Keyed_Download_Responce info;
    info.code = -1;

    if(length == 67)
    {
        memcpy((char *)&info.key,    message+23, 20);
        memcpy((char *)&info.fileID, message+43, 20);
        memcpy((char *)&info.chunk,  message+63,  4);
        info.code = 0;
    }


    return info;
}





