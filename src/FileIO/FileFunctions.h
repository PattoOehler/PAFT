
#ifndef __PAFT_FILEFUNCTIONS_H
#define __PAFT_FILEFUNCTIONS_H

#include "../DHT/DHT.h"

#include <iostream>
#include <vector>
#include <string>

namespace paft
{
    class File_Functions
    {
    private:

    public:
        static void Load_State();
        static void Save_State();

        static void Save_DHT_Entries();
        static void Save_File_Entries();
        static void Save_Local_File_Locations();

        static void Load_DHT_Entries();
        static void Load_File_Entries();
        static void Load_Local_File_Locations();

        static std::vector<std::string> Get_All_Files_Names_Within_Folder(std::string folder);


        static void Allocate_File();
        static void Allocate_File(std::string filename);
        static long Get_File_Length(std::string filename);

    };

}





#endif // __PAFT_FILEFUNCTIONS_H
