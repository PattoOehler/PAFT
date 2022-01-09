
#include "Major_Functions.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Lookup.h"
#include "Main_Client.h"

using namespace paft;


three_DHT Major_Functions::Three_Closest_Peers_In_Network(_160bitnumber id)
{
    int dht_Identifier = DHT_Lookup::Get_Available_Three_DHT();

    three_DHT in_DHT = DHT::Lookup(id);
    DHT_Lookup::Write_To_Three_DHT(in_DHT, dht_Identifier);
    for(int i=0;i<3;i++)
    {
        MainClient client = MainClient(in_DHT.entry[i].addr, in_DHT.entry[i].port);
        client.Find_Node_Recursive(id, dht_Identifier);
    }

    DHT_Lookup::Write_To_Three_DHT_Is_Available(true, dht_Identifier);

    return in_DHT;
}

