#include "Peer.h"
#include "../DHT/DHT.h"

#include <iostream>

using namespace paft;




void Peer::Set_Data(DHT_Single_Entry peer1, DHT_Single_Entry peer2, _160bitnumber peerKey)
{
    peer1.is_set = true;
    peer2.is_set = true;


    this->peerFrom = peer1;
    this->peerTo = peer2;
    this->peerKey = peerKey;
    this->isSet = true;

}

void Peer::Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey)
{
    peer1.is_set = true;

    this->peerTo = peer1;
    this->peerKey = peerKey;
    this->isEndPoint = true;
    this->isSet = true;

}

void Peer::Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey, const char *localFileLocation, int localFileLocationLength, _160bitnumber fileID)
{
    peer1.is_set = true;

    this->peerTo = peer1;
    this->peerKey = peerKey;
    this->isEndPoint = true;
    this->isSet = true;

    memcpy(this->localFileLocation, localFileLocation, localFileLocationLength);
    this->fileID = fileID;
    this->hasFile = true;

}

void Peer::Set_Data(DHT_Single_Entry peer1, _160bitnumber peerKey, _160bitnumber fileID)
{
    peer1.is_set = true;

    this->peerFrom = peer1;
    this->peerKey = peerKey;
    this->isEndPoint = true;
    this->isSet = true;


    this->fileID = fileID;
    this->hasFile = false;

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

bool Peer::Is_File_Proxy(_160bitnumber fileID)
{
    if(this->isSet && this->isEndPoint && (!this->hasFile) )
    {
        if( DHT::Is_Equal(this->fileID, fileID))
            return true;
    }
    return false;

}



bool Peer::Is_Set()
{

    return this->isSet;
}


DHT_Single_Entry Peer::Other_Peer(DHT_Single_Entry peer1, _160bitnumber peerKey)
{


    if( Is_Equal_Key(peerKey) && (!this->isEndPoint))
    {

        if( (memcmp( (char *)&this->peerTo.addr, (char *)&peer1.addr, 4 )==0) && this->peerTo.port == peer1.port )
        {
            return this->peerFrom;
        }

        else if( (memcmp((char *)&this->peerFrom.addr, (char *)&peer1.addr, 4 )==0) && this->peerFrom.port == peer1.port)
        {
            return this->peerTo;
        }
    }


    DHT_Single_Entry a;
    return a;

}


_160bitnumber Peer::Get_Peer_Key()
{
    return peerKey;

}




DHT_Single_Entry Peer::Get_Peer_From()
{
    if(this->Is_Set())
    {
        //Todo more checks to ensure that the peer from is set
        return this->peerFrom;
    }
    DHT_Single_Entry a;
    return a;
}

DHT_Single_Entry Peer::Get_Peer_To()
{
    if(this->Is_Set())
    {
        //Todo more checks to ensure that the peer from is set
        return this->peerTo;
    }
    DHT_Single_Entry a;
    return a;
}


bool Peer::Is_End_Point()
{
    return this->isEndPoint;

}

bool Peer::Has_File()
{
    return this->hasFile;

}
