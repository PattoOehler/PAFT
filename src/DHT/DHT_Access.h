
#ifndef __PAFT_DHT_DHT_ACCESS_H
#define __PAFT_DHT_DHT_ACCESS_H

#include <stdint.h>
#include <mutex>
#include <winsock.h>

#include "DHT.h"


namespace paft
{
    class DHT_Access
    {
    private:
        static DHT_Single_Entry* all_Peers;
        static DHT_Single_Entry* all_Files;
        static std::string *local_File_Locations;

        static std::mutex* all_Peers_Mutex;
        static std::mutex* all_Files_Mutex;

        static _160bitnumber* own_ID;
        static unsigned short int own_Port;



    public:
        static DHT_Single_Entry Access_DHT(int position);
        static void Write_To_DHT(DHT_Single_Entry write, int position);


        static DHT_Single_Entry Access_FileIds(int position);
        static void Write_To_FileIds(DHT_Single_Entry write, int position);
        static int Store_FileId(DHT_Single_Entry entry);

        static int Find_Stored_File(_160bitnumber entry);

        static _160bitnumber Get_SELF();
        static void Set_Self(_160bitnumber own_Id);

        static void Set_Self_Port(unsigned short int port);
        static unsigned short int Get_Self_Port();

        static void Set_Local_File_Location(std::string local_File_Location, int position);
        static std::string Get_Local_File_Location(int position);

    };



}


#endif
