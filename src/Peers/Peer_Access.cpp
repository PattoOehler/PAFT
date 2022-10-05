#include "Peer_Access.h"

using namespace paft;
#define NUM_PEERS 200


Peer* Peer_Access::all_Peers = new Peer[NUM_PEERS];



void Peer_Access::Add_Peer(DHT_Single_Entry peer1, DHT_Single_Entry peer2, _160bitnumber peerKey)
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(!all_Peers[i].Is_Set())
        {
                all_Peers[i].Set_Data(peer1, peer2, peerKey);
                return;
        }
    }

}

void Peer_Access::Add_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey)
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(!all_Peers[i].Is_Set())
        {
                all_Peers[i].Set_Data(peer1, peerKey);
                return;
        }
    }

}


DHT_Single_Entry Peer_Access::Find_Peer(DHT_Single_Entry peerFrom, _160bitnumber peerKey)
{

    for(int i=0; i<NUM_PEERS; i++)
    {
        if(all_Peers[i].Is_Set())
        {
            if( all_Peers[i].Is_Equal_Key(peerKey) )
            {
                return all_Peers[i].Other_Peer(peerFrom, peerKey);

            }
        }
    }
}
