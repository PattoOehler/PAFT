
#include "DHT_Updater.h"
#include "DHT.h"
#include "DHT_Access.h"

#include <time.h>

using namespace paft;
void DHT_Updater::Update_Main_DHT()
{
    for(int i=0; i<160*20;i++)
    {

        DHT_Single_Entry entry = DHT_Access::Access_DHT(i);
        if(entry.is_set)
        {
            if(entry.time_To_Timeout < time(0))
            {


            }

        }

    }

}
