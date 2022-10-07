#ifndef __PAFT_PEER_ACCESS_H
#define __PAFT_PEER_ACCESS_H

#include "Peer.h"

namespace paft
{
    class Peer_Access
    {
    private:
        static Peer* all_Peers;

    public:
        static void Add_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey, const char *localFileLocation, int localFileLocationLength, _160bitnumber fileID);
        static void Add_Peer(DHT_Single_Entry peer1, DHT_Single_Entry peer2, _160bitnumber peerKey);
        static void Add_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey);

        static DHT_Single_Entry Find_Peer(DHT_Single_Entry peerFrom, _160bitnumber peerKey);

    };

}


#endif // __PAFT_PEER_ACCESS_H
