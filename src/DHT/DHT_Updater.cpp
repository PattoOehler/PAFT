
#include "DHT_Updater.h"
#include "DHT.h"
#include "DHT_Access.h"

#include "../Client/Main_Client.h"

#include <time.h>
#include <thread>
#include <Windows.h>
#include <iostream>


#define DHT_File_Len 100

using namespace paft;


void DHT_Updater::Call_Ping(DHT_Single_Entry entry)
{
    Main_Client *a = new Main_Client(entry.addr, entry.port);
    a->Ping();

    std::cout << "Fixed timed out entry!!!\n";
}



void DHT_Updater::Update_Main_DHT()
{
    for(int i=0; i<160*20;i++)
    {

        DHT_Single_Entry entry = DHT_Access::Access_DHT(i);
        if(entry.is_set)
        {
            if(entry.time_To_Timeout < time(0))
            {
                std::cout << "Found a timed out DHT_Entry\n";
                entry.is_set = false;
                DHT_Access::Write_To_DHT(entry, i);
                Sleep(10);

                std::thread thread(Call_Ping, entry);
                thread.detach();
            }

        }

    }

}

void DHT_Updater::Update_File_DHT()
{
    for(int i=0;i<DHT_File_Len;i++)
    {
        DHT_Single_Entry entry = DHT_Access::Access_File_IDs(i);
        if(entry.is_set)
        {

            if(entry.time_To_Timeout < time(0))
            {
                std::cout << "Found a timed out file entry\n";
                entry.is_set = false;
                DHT_Access::Write_To_File_IDs(entry,i);

            }

        }

    }


}

void DHT_Updater::Keep_DHT_Updated()
{
    while(true)
    {
        Sleep(60 * 1000); // 30seconds

        std::cout << "Keeping the DHT Updated\n";
        Update_Main_DHT();
        Update_File_DHT();


    }



}











