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

        char localFileLocation[100];
        _160bitnumber fileID;
        bool hasFile = false;

    public:
        void Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey, const char *localFileLocation, int localFileLocationLength, _160bitnumber fileID);
        void Set_Data(DHT_Single_Entry peer1, DHT_Single_Entry peer2, _160bitnumber peerKey);
        void Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey);
        void Unset_Data();

        bool Is_Equal_Key(_160bitnumber key);
        bool Is_Set();

        DHT_Single_Entry Other_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey);

    };

}


#endif // __PAFT_PEER_H
