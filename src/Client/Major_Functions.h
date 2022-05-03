
#ifndef __PAFT_MAJOR_FUNCTIONS_H
#define __PAFT_MAJOR_FUNCTIONS_H

#include "../DHT/DHT.h"

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
        static void getMetaDataFile(_160bitnumber ID, std::string checksum, DHT_Single_Entry entry);
        static void getFileChunk(_160bitnumber ID, std::string checksum_expected, DHT_Single_Entry entry, int chunkNum);
        static three_DHT Find_File_On_Network(_160bitnumber id);

    };

}





#endif // __PAFT_MAJOR_FUNCTIONS_H
