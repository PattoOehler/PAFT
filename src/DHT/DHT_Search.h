

#ifndef __PAFT_DHT_DHT_SEARCH_H
#define __PAFT_DHT_DHT_SEARCH_H

#include "DHT.h"


namespace paft
{
    class DHT_Search
    {
    public:
        static DHT_Single_Entry Next_Closest_In_Bucket(int bucket, _160bitnumber id_to_find, _160bitnumber previous_id);
        static three_DHT Lookup(_160bitnumber id);
        static three_DHT Find_Value(_160bitnumber id);
        static three_DHT Lookup_One_Bucket(_160bitnumber id, int bucket);
    };



}


#endif
