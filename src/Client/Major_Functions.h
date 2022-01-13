
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

    };

}





#endif // __PAFT_MAJOR_FUNCTIONS_H
