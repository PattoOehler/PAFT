#include "Peer.h"
#include "../DHT/DHT.h"

using namespace paft;




void Peer::Set_Data(DHT_Single_Entry peer1, DHT_Single_Entry peer2, _160bitnumber peerKey)
{

    this->peerFrom = peer1;
    this->peerTo = peer2;
    this->peerKey = peerKey;
    this->isSet = true;

}
void Peer::Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey)
{
    this->peerTo = peer1;
    this->peerKey = peerKey;
    this->isEndPoint = true;
    this->isSet = true;

}

void Peer::Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey, const char *localFileLocation, int localFileLocationLength, _160bitnumber fileID)
{
    this->peerTo = peer1;
    this->peerKey = peerKey;
    this->isEndPoint = true;
    this->isSet = true;

    memcpy(this->localFileLocation, localFileLocation, localFileLocationLength);
    this->fileID = fileID;
    this->hasFile = true;

}


void Peer::Unset_Data()
{
    this->isEndPoint = false;
    this->isSet = false;
    this->hasFile = false;
}


bool Peer::Is_Equal_Key(_160bitnumber key)
{
    bool a = DHT::Is_Equal(this->peerKey, key);

    return a;

}



bool Peer::Is_Set()
{

    return this->isSet;
}


DHT_Single_Entry Peer::Other_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey)
{


    if( Is_Equal_Key(peerKey) && (!this->isEndPoint))
    {
        if( memcmp( (char *)&this->peerTo.addr, (char *)&peer1.addr, 4 ) && this->peerTo.port == peer1.port )
            return this->peerFrom;
        else if( memcmp((char *)&this->peerFrom.addr, (char *)&peer1.addr, 4 ) && this->peerFrom.port == peer1.port)
            return this->peerTo;
    }

    DHT_Single_Entry a;
    return a;

}


_160bitnumber Peer::Get_Peer_Key()
{
    return peerKey;

}








