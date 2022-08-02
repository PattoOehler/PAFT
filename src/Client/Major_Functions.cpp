
#include "Major_Functions.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Lookup.h"
#include "../DHT/DHT_Access.h"
#include "Main_Client.h"
#include "../FileIO/Meta_Files.h"
#include "../FileIO/SHA_256.h"
#include "../FileIO/File_Functions.h"
#include "../DHT/DHT_Search.h"

#include <iostream>
#include <fstream>

#include <unistd.h>

using namespace paft;


three_DHT Major_Functions::Three_Closest_Peers_In_Network(_160bitnumber id)
{
    int dht_Identifier = DHT_Lookup::Get_Available_Three_DHT();

    three_DHT in_DHT = DHT_Search::Lookup(id);
    DHT_Lookup::Write_To_Three_DHT(in_DHT, dht_Identifier);
    for(int i=0;i<3;i++)
    {
        if(in_DHT.entry[i].is_set)
        {
            Main_Client client = Main_Client(in_DHT.entry[i].addr, in_DHT.entry[i].port);
            client.Find_Node_Recursive(id, dht_Identifier);
        }

    }

    DHT_Lookup::Write_To_Three_DHT_Is_Available(true, dht_Identifier);

    return in_DHT;
}


three_DHT Major_Functions::Find_File_On_Network(_160bitnumber id)
{
    int dht_Identifier = DHT_Lookup::Get_Available_Three_DHT();

    three_DHT in_DHT = DHT_Search::Lookup(id);
    DHT_Lookup::Write_To_Three_DHT(in_DHT, dht_Identifier);
    for(int i=0;i<3;i++)
    {
        if(in_DHT.entry[i].is_set)
        {
            Main_Client client = Main_Client(in_DHT.entry[i].addr, in_DHT.entry[i].port);
            client.Find_File_Recursive(id, dht_Identifier);
        }

    }

    three_DHT found_Values = DHT_Lookup::Access_Three_DHT(dht_Identifier);

    DHT_Lookup::Write_To_Three_DHT_Is_Available(true, dht_Identifier);

    return found_Values;
}



/*void Major_Functions::Upload_File_To_Network(const char *local_file_location, const char *public_File_Name)
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
    self.
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




} */




void Major_Functions::Upload_File_To_Network(const char *local_file_location, const char *public_File_Name)
{
    _160bitnumber created_File_ID = DHT::Random_ID();

    Meta_Files::Make_File(local_file_location, public_File_Name, created_File_ID);

    DHT_Single_Entry self;
    self.is_set = true;
    self.id = created_File_ID;
    unsigned int z=0;
    memcpy((char *)&self.addr,(char *) &z, 4);
    self.port = DHT_Access::Get_Self_Port();


    int fileIDpos = DHT_Access::Store_File_ID(self);

    DHT_Access::Set_Local_File_Location(local_file_location, fileIDpos);


    three_DHT closest_In_Network = Three_Closest_Peers_In_Network(created_File_ID);
    for(int i=0;i<3;i++)
    {
        if(closest_In_Network.entry[i].is_set)
        {
            Main_Client client = Main_Client(closest_In_Network.entry[i].addr, closest_In_Network.entry[i].port);
            client.Store_File(self);
        }
    }




}





void Major_Functions::Upload_File_To_Network(const char *local_file_location, const char *public_File_Name, _160bitnumber fileID)
{

    Meta_Files::Make_File(local_file_location, public_File_Name, fileID);

    DHT_Single_Entry self = DHT_Access::Get_Self();
    self.id = fileID;
    self.addr = Main_Client::NULL_Addr();

    int fileIDpos = DHT_Access::Store_File_ID(self);

    DHT_Access::Set_Local_File_Location(local_file_location, fileIDpos);


    three_DHT closest_In_Network = Three_Closest_Peers_In_Network(fileID);
    for(int i=0;i<3;i++)
    {
        if(closest_In_Network.entry[i].is_set)
        {
            Main_Client client = Main_Client(closest_In_Network.entry[i].addr, closest_In_Network.entry[i].port);
            client.Store_File(self);
        }
    }

}



void Major_Functions::Upload_To_Network(const char *local_file_location, _160bitnumber fileID)
{

    //Meta_Files::Make_File(local_file_location, public_File_Name, fileID);

    DHT_Single_Entry self = DHT_Access::Get_Self();
    self.id = fileID;
    self.addr = Main_Client::NULL_Addr();

    int fileIDpos = DHT_Access::Store_File_ID(self);

    DHT_Access::Set_Local_File_Location(local_file_location, fileIDpos);


    three_DHT closest_In_Network = Three_Closest_Peers_In_Network(fileID);
    for(int i=0;i<3;i++)
    {
        if(closest_In_Network.entry[i].is_set)
        {
            Main_Client client = Main_Client(closest_In_Network.entry[i].addr, closest_In_Network.entry[i].port);
            client.Store_File(self);
        }
    }

}







//Returns 0 upon success anything else for an error
int Major_Functions::Get_Metadata_File_Self(_160bitnumber ID, std::string checksum_expected, DHT_Single_Entry entry)
{
    std::cout << "Major Functions -- client.Get_MetaData_File\n";
    sleep(2);
    Main_Client client = Main_Client(entry.addr, entry.port);
    char *recvbuf = client.Get_Metadata_File(ID);
    std::cout << "Major Functions -- client.Get_MetaData_File done!\n";

    int len;
    memcpy((char *)&len,recvbuf,4);

    std::string checksum_received = sha256(recvbuf+4, len-4);
    if(checksum_expected == checksum_received)
    {
        std::cout << "CHECKSUM CONFIRMED!!!! Writing the metadata to a file!\n";
    }
    else
    {
        std::cout << "CHECKSUM INCORRECT!!!! EXITING!\n";
        return -1;
    }


    //Write to the file
    if(len -4 > 0)
    {
        std::ofstream wf("Test_Metafiles/meta.paft", std::ios::out | std::ios::binary);
        if(!wf) {
          std::cout << "Cannot open file!\n" << std::endl;
          return -1;
        }

        wf.write(recvbuf+4, len-4);
        wf.close();
    }
    else
    {
        std::cout << "Major_Functions::getMetaDataFile length is <= 0! (=" << len-4 << ")\n";

    }
    return 0;
}



//Returns 0 upon success anything else for an error
int Major_Functions::Get_Metadata_File(_160bitnumber ID, std::string checksum_expected, DHT_Single_Entry entry)
{
    std::cout << "Major Functions -- client.Get_MetaData_File\n";
    sleep(2);
    Main_Client client = Main_Client(entry.addr, entry.port);
    char *recvbuf = client.Get_Metadata_File(ID);
    std::cout << "Major Functions -- client.Get_MetaData_File done!\n";

    int len;
    memcpy((char *)&len,recvbuf,4);

    std::string checksum_received = sha256(recvbuf+4, len-4);
    if(checksum_expected == checksum_received)
    {
        std::cout << "CHECKSUM CONFIRMED!!!! Writing the metadata to a file!\n";
    }
    else
    {
        std::cout << "CHECKSUM INCORRECT!!!! EXITING!\n";
        return -1;
    }


    //Write to the file
    if(len -4 > 0)
    {
        std::ofstream wf(Meta_Files::Get_Output_File_Name(ID), std::ios::out | std::ios::binary);
        if(!wf) {
          std::cout << "Cannot open file!\n" << std::endl;
          return -1;
        }

        wf.write(recvbuf+4, len-4);
        wf.close();
    }
    else
    {
        std::cout << "Major_Functions::getMetaDataFile length is <= 0! (=" << len-4 << ")\n";

    }
    free(recvbuf);
    return 0;
}




int Major_Functions::Get_Metadata_File_Proxy(std::string checksum_expected, DHT_Single_Entry connectTo, ChunkResponce info)
{
    std::cout << "Major Functions -- client.Get_Metadata_File_Proxy\n";
    sleep(2);
    Main_Client client = Main_Client(connectTo.addr, connectTo.port);

    Message msg = client.Proxy_Get_Chunk(info);
    msg.msgLength-=4; //Makes it work
    std::cout << "Major Functions -- client.Get_Metadata_File_Proxy done!\n";
    if(msg.msgLength == 0)
    {
        std::cout << "msg.msgLength == 0 in Major_Functions::Get_Metadata_File_Proxy Returning\n";
        return 0;
    }

    std::string checksum_received = sha256(msg.message, msg.msgLength);
    if(checksum_expected == checksum_received)
    {
        std::cout << "CHECKSUM CONFIRMED!!!! Writing the metadata to a file!\n";
    }
    else
    {
        std::cout << "CHECKSUM INCORRECT!!!! EXITING len=" << msg.msgLength << "!\n";
        return -1;
    }


    //Write to the file
    if(msg.msgLength > 0)
    {
        std::ofstream wf(Meta_Files::Get_Output_File_Name(info.fileID), std::ios::out | std::ios::binary);
        if(!wf) {
          std::cout << "Cannot open file!\n" << std::endl;
          return -1;
        }

        wf.write(msg.message, msg.msgLength);
        wf.close();
    }
    else
    {
        std::cout << "Major_Functions::Get_Metadata_File_Proxy length is <= 0! (=" << msg.msgLength << ")\n";

    }
    free(msg.message);
    return 0;
}



void Major_Functions::Get_File_Chunk_Proxy(std::string checksum_expected, DHT_Single_Entry entry, ChunkResponce info)
{
    if(info.chunkID < 0)
        return;

    sleep(2);
    Main_Client client = Main_Client(entry.addr, entry.port);
    Message msg = client.Proxy_Get_Chunk(info);

    std::string checksum_received = sha256(msg.message, msg.msgLength); // -4 on msgLength if doesn't work ++ also below

    if( checksum_received == checksum_expected)
        std::cout << "Major_Functions::getFileChunk CHECKSUM IS GOOD\n";
    else
    {
        std::cout << "Major_Functions::getFileChunk CHECKSUM IS " << checksum_received << " while expecting " << checksum_expected << " len=" << msg.msgLength << "\n";
    }



    //Write to the file
    if(msg.msgLength > 0)
    {
        std::ofstream wf("Test_Metafiles/Downloaded_File", std::ios::in | std::ios::out | std::ios::binary);
        if(!wf) {
          std::cout << "Cannot open file!\n" << std::endl;
          return;
        }
        wf.seekp( 8000000 * info.chunkID );
        wf.write(msg.message, msg.msgLength);
        wf.close();
    }
    else
    {
        std::cout << "Major_Functions::getFileChunk length is <= 0! (len=" << msg.msgLength << ")\n";

    }



}










void Major_Functions::Get_File_Chunk(_160bitnumber ID, std::string checksum_expected, DHT_Single_Entry entry, int chunkNum)
{
    if(chunkNum < 0)
        return;

    sleep(2);
    Main_Client client = Main_Client(entry.addr, entry.port);
    char *recvbuf = client.Get_File_Chunk(ID, chunkNum);

    int len;
    memcpy((char *)&len,recvbuf,4);

    std::string checksum_received = sha256(recvbuf+4, len-4);
    //TODO verify checksum
    if( checksum_received == checksum_expected)
        std::cout << "Major_Functions::getFileChunk CHECKSUM IS GOOD\n";
    else
    {
        std::cout << "Major_Functions::getFileChunk CHECKSUM IS " << checksum_received << " while expecting " << checksum_expected << "\n";
    }



    //Write to the file
    if(len -4 > 0)
    {
        std::ofstream wf("Test_Metafiles/Downloaded_File", std::ios::in | std::ios::out | std::ios::binary);
        if(!wf) {
          std::cout << "Cannot open file!\n" << std::endl;
          return;
        }
        wf.seekp( 8000000 * chunkNum );
        wf.write(recvbuf+4, len-4);
        wf.close();
    }
    else
    {
        std::cout << "Major_Functions::getFileChunk length is <= 0! (len=" << len-4 << ")\n";

    }



}








