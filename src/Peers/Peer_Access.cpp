#include "Peer_Access.h"

#include <iostream>


using namespace paft;
#define NUM_PEERS 200


Peer* Peer_Access::all_Peers = new Peer[NUM_PEERS];





void Peer_Access::Add_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey, const char *localFileLocation, int localFileLocationLength, _160bitnumber fileID)
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(!all_Peers[i].Is_Set())
        {
                all_Peers[i].Set_Data(peer1, peerKey, localFileLocation, localFileLocationLength, fileID);
                return;
        }
    }

}





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


void Peer_Access::Add_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey, _160bitnumber fileID)
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(!all_Peers[i].Is_Set())
        {
                all_Peers[i].Set_Data(peer1, peerKey, fileID);
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

    DHT_Single_Entry a;
    return a;

}


Peer Peer_Access::Find_Peer(_160bitnumber fileID)
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(all_Peers[i].Is_Set())
        {
            if( all_Peers[i].Is_File_Proxy(fileID) )
            {
                return all_Peers[i];

            }
        }
    }

    Peer a;
    return a;

}



Peer Peer_Access::Find_Peer_With_Key(_160bitnumber key)
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(all_Peers[i].Is_Set())
        {
            if( all_Peers[i].Is_Equal_Key(key) )
            {
                return all_Peers[i];

            }
        }
    }

    Peer a;
    return a;

}



void Peer_Access::Print_Peers()
{
    for(int i=0; i<NUM_PEERS; i++)
    {
        if(all_Peers[i].Is_Set())
        {
            std::cout << "Peer Key = " << DHT::ID_To_String(all_Peers[i].Get_Peer_Key()) << " ";
            if(all_Peers[i].Is_End_Point())
            {
                if(all_Peers[i].Has_File())
                {
                    DHT_Single_Entry peer2 = all_Peers[i].Get_Peer_To();
                    std::cout << "Peer To " << DHT::IP_To_String(peer2.addr) << " " << peer2.port;
                }
                else
                {
                    DHT_Single_Entry peer2 = all_Peers[i].Get_Peer_From();
                    std::cout << "Peer From " << DHT::IP_To_String(peer2.addr) << " " << peer2.port;

                }

            }
            else
            {
                DHT_Single_Entry peer1 = all_Peers[i].Get_Peer_From();
                DHT_Single_Entry peer2 = all_Peers[i].Get_Peer_To();

                std::cout << "Peer 1 " << DHT::IP_To_String(peer1.addr) << " " << peer1.port;
                std::cout << " Peer 2 " << DHT::IP_To_String(peer2.addr) << " " << peer2.port;

            }
            std::cout << "\n";
        }
    }


}
