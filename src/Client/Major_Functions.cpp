
#include "Major_Functions.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Lookup.h"
#include "../DHT/DHT_Access.h"
#include "Main_Client.h"
#include "../FileIO/Meta_Files.h"

using namespace paft;


three_DHT Major_Functions::Three_Closest_Peers_In_Network(_160bitnumber id)
{
    int dht_Identifier = DHT_Lookup::Get_Available_Three_DHT();

    three_DHT in_DHT = DHT::Lookup(id);
    DHT_Lookup::Write_To_Three_DHT(in_DHT, dht_Identifier);
    for(int i=0;i<3;i++)
    {
        if(in_DHT.entry[i].is_set)
        {
            MainClient client = MainClient(in_DHT.entry[i].addr, in_DHT.entry[i].port);
            client.Find_Node_Recursive(id, dht_Identifier);
        }
    }

    DHT_Lookup::Write_To_Three_DHT_Is_Available(true, dht_Identifier);

    return in_DHT;
}


void Major_Functions::Upload_File_To_Network(const char *local_file_location, const char *public_File_Name)
{
    _160bitnumber created_File_ID = DHT::Random_ID();

    Meta_Files::Make_File(local_file_location, public_File_Name, created_File_ID);

    DHT_Single_Entry self = DHT_Access::Access_DHT(159*20);
    if(!self.is_set)
    {
        std::cout << "ERROR in Upload_File_To_Network -- DHT[159*20] isn't set to SELF!!!\n";
        return;
    }

    self.id = created_File_ID;

    int fileIDpos = DHT_Access::Store_FileId(self);

    DHT_Access::Set_Local_File_Location(local_file_location, fileIDpos);

    three_DHT closest_In_Network = Three_Closest_Peers_In_Network(created_File_ID);
    for(int i=0;i<3;i++)
    {
        if(closest_In_Network.entry[i].is_set)
        {
            MainClient client = MainClient(closest_In_Network.entry[i].addr, closest_In_Network.entry[i].port);
            client.Store_File(self);
        }
    }




}
