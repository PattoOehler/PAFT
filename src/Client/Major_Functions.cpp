
#include "Major_Functions.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Lookup.h"
#include "../DHT/DHT_Access.h"
#include "Main_Client.h"
#include "../FileIO/Meta_Files.h"
#include "../FileIO/sha256.h"


#include <iostream>
#include <fstream>

#include <unistd.h>

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


void Major_Functions::Upload_File_To_Network(const char *local_file_location, const char *public_File_Name, _160bitnumber fileID)
{

    Meta_Files::Make_File(local_file_location, public_File_Name, fileID);

    DHT_Single_Entry self = DHT_Access::Access_DHT(159*20);
    if(!self.is_set)
    {
        std::cout << "ERROR in Upload_File_To_Network -- DHT[159*20] isn't set to SELF!!!\n";
        return;
    }

    self.id = fileID;

    int fileIDpos = DHT_Access::Store_FileId(self);

    DHT_Access::Set_Local_File_Location(local_file_location, fileIDpos);

    three_DHT closest_In_Network = Three_Closest_Peers_In_Network(fileID);
    for(int i=0;i<3;i++)
    {
        if(closest_In_Network.entry[i].is_set)
        {
            MainClient client = MainClient(closest_In_Network.entry[i].addr, closest_In_Network.entry[i].port);
            client.Store_File(self);
        }
    }




}


void Major_Functions::getMetaDataFile(_160bitnumber ID, std::string checksum_expected, DHT_Single_Entry entry)
{
    std::cout << "Major Functions -- client.Get_MetaData_File\n";
    sleep(2);
    MainClient client = MainClient(entry.addr, entry.port);
    char *recvbuf = client.Get_MetaData_File(ID);
    std::cout << "Major Functions -- client.Get_MetaData_File done!\n";

    int len;
    memcpy((char *)&len,recvbuf,4);

    std::string checksum_received = sha256(recvbuf+4, len-4);
    //TODO verify checksum
    std::cout << "Checksum: " << checksum_received << "\n";

    //Write to the file
    if(len -4 > 0)
    {
        std::ofstream wf("Test_Metafiles/meta.paft", std::ios::out | std::ios::binary);
        if(!wf) {
          std::cout << "Cannot open file!\n" << std::endl;
          return;
        }

        wf.write(recvbuf+4, len-4);
        wf.close();
    }
    else
    {
        std::cout << "Major_Functions::getMetaDataFile length is < 0! (=" << len << ")\n";

    }

}



















