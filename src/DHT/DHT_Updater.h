
#ifndef __PAFT_DHT_DHT_UPDATER_H
#define __PAFT_DHT_DHT_UPDATER_H

#include "DHT.h"

namespace paft
{
    class DHT_Updater
    {
    public:
        static void Update_Main_DHT();
        static void Call_Ping(DHT_Single_Entry entry);

        static void Update_File_DHT();
        static void Keep_DHT_Updated();
    };



}



#endif
