#include "Print_DHT.h"
#include "DHT_Access.h"

#include <iostream>

using namespace paft;




void Print_DHT::Print_Main_DHT()
{

    for(int i=0;i<160;i++)
    {
        for(int j=0;j<20;j++)
        {
            DHT_Single_Entry tmp = DHT_Access::Access_DHT(20*i+j);
            if(tmp.is_set)
                std::cout << DHT::ID_To_String(tmp.id) << "    In k-bucket " << std::dec << (i) << " position " << j << " with timeout time " << ctime(&tmp.time_To_Timeout ); //ctime has a \n at the end

        }
    }
}






void Print_DHT::Print_Files()
{

    for(int i=0;i<100;i++)
    {
        DHT_Single_Entry fileEntry = DHT_Access::Access_File_IDs(i);
        if(fileEntry.is_set)
            std::cout << DHT::ID_To_String(fileEntry.id) << "    In part " << i << "\n";

    }

}




void Print_DHT::Print_File_Locations()
{

    for(int i=0;i<100;i++)
    {
        std::string local_File = DHT_Access::Get_Local_File_Location(i);
        if(!local_File.empty())
            std::cout << i << " " << local_File << "\n";

    }


}

