#ifndef __PAFT_PEER_H
#define __PAFT_PEER_H

#include "../DHT/DHT.h"

namespace paft
{
    class Peer
    {
    private:
        DHT_Single_Entry peerFrom, peerTo;
        _160bitnumber peerKey;
        bool isEndPoint = false;
        bool isSet = false;
        _160bitnumber fileID;

        char localFileLocation[100];
        bool hasFile = false;

    public:
        void Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey, const char *localFileLocation, int localFileLocationLength, _160bitnumber fileID);
        void Set_Data(DHT_Single_Entry peer1, DHT_Single_Entry peer2, _160bitnumber peerKey);
        void Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey, _160bitnumber fileID);
        void Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey);
        void Unset_Data();

        bool Is_Equal_Key(_160bitnumber key);
        bool Is_File_Proxy(_160bitnumber fileID);
        bool Is_Set();
        bool Is_End_Point();
        bool Has_File();

        DHT_Single_Entry Get_Peer_From();
        DHT_Single_Entry Get_Peer_To();

        DHT_Single_Entry Other_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey);

        _160bitnumber Get_Peer_Key();

    };

}


#endif // __PAFT_PEER_H
