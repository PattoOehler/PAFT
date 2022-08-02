
#ifndef __PAFT_MAJOR_FUNCTIONS_H
#define __PAFT_MAJOR_FUNCTIONS_H

#include "../DHT/DHT.h"
#include "../Messages/Message_Proxy.h"

#include <iostream>

namespace paft
{
    class Major_Functions
    {
    private:

    public:
        static three_DHT Three_Closest_Peers_In_Network(_160bitnumber id);
        static void Upload_File_To_Network(const char *local_file_location, const char *public_File_Name);
        static void Upload_File_To_Network(const char *local_file_location, const char *public_File_Name, _160bitnumber fileID);
        static int Get_Metadata_File(_160bitnumber ID, std::string checksum, DHT_Single_Entry entry);
        static int Get_Metadata_File_Self(_160bitnumber ID, std::string checksum, DHT_Single_Entry entry);
        static void Get_File_Chunk(_160bitnumber ID, std::string checksum_expected, DHT_Single_Entry entry, int chunkNum);
        static three_DHT Find_File_On_Network(_160bitnumber id);
        static void Upload_To_Network(const char *local_file_location, _160bitnumber fileID);
        static int Get_Metadata_File_Proxy(std::string checksum_expected, DHT_Single_Entry connectTo, ChunkResponce info);
        static int Get_Metadata_File_Proxy_2(std::string checksum_expected, DHT_Single_Entry connectTo, DHT_Single_Entry forwardTo, ChunkResponce info);
        static void Get_File_Chunk_Proxy(std::string checksum_expected, DHT_Single_Entry entry, ChunkResponce info);
    };

}





#endif // __PAFT_MAJOR_FUNCTIONS_H
