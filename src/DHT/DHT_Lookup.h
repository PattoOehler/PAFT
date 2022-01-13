
#ifndef __PAFT_DHT_DHT_LOOKUP_H
#define __PAFT_DHT_DHT_LOOKUP_H

#include <stdint.h>
#include <mutex>

#include "DHT.h"


namespace paft
{
    class DHT_Lookup
    {
    private:
        static three_DHT* three_DHT_For_Lookups;
        static std::mutex* three_DHT_For_Lookups_Mutex;
        static std::mutex* is_Available_Mutex;

        static bool* three_DHT_Is_Available;

    public:
        static three_DHT Access_Three_DHT(int position);
        static three_DHT Access_Three_DHT_And_Lock(int position);

        static void Write_To_Three_DHT(three_DHT write, int position);

        static void Write_To_Three_DHT_Is_Available(bool write, int position);
        static bool Access_To_Three_DHT_Is_Available(int position);

        static int Get_Available_Three_DHT();

        static void Unlock(int position);
        static void Write_To_Locked_Three_DHT(three_DHT write, int position);

    };



}


#endif
