
#ifndef __PAFT_MINOR_FUNCTIONS_H
#define __PAFT_MINOR_FUNCTIONS_H

#include "../DHT/DHT.h"

namespace paft
{
    class Minor_Functions
    {
    private:
        static void Not_All_Set(DHT_Single_Entry entry, int lookupID);

        static void Put_In_First(DHT_Single_Entry, int lookupID, three_DHT putIn);
        static void Put_In_Second(DHT_Single_Entry, int lookupID, three_DHT putIn);
        static void Put_In_Third(DHT_Single_Entry, int lookupID, three_DHT putIn1);

    public:
        static void Add_To_Lookup_DHT(int lookupID, DHT_Single_Entry entry, _160bitnumber lookingFor);
        static void Write_Single_Entry_To_DHT_Lookup(DHT_Single_Entry entry, int lookupID, _160bitnumber lookingFor);

        static void Do_Lookup_If_Closer(int lookupID, three_DHT received_values, _160bitnumber lookingFor);

    };

}





#endif // __PAFT_MAJOR_FUNCTIONS_H
