
#include "Minor_Functions.h"
#include "../DHT/DHT_Lookup.h"
#include "../DHT/DHT.h"

using namespace paft;

void Minor_Functions::Add_To_Lookup_DHT(int lookupID, DHT_Single_Entry entry, _160bitnumber lookingFor)
{
    three_DHT current_Three = DHT_Lookup::Access_Three_DHT(lookupID);

    //If there is a not set value it should be in entry[2]
    if(!current_Three.entry[2].is_set)
    {
        Write_Single_Entry_To_DHT_Lookup(entry, lookupID, lookingFor);
        return;
    }

    bool is_third_closer = DHT::Compare(entry.id, current_Three.entry[2].id, lookingFor);

    if(is_third_closer)
    {
        Write_Single_Entry_To_DHT_Lookup(entry, lookupID, lookingFor);

    }
}


void Minor_Functions::Write_Single_Entry_To_DHT_Lookup(DHT_Single_Entry entry, int lookupID, _160bitnumber lookingFor)
{
    //Todo Might need to lock the mutex until DHT_Lookup is written to
    three_DHT current_Three = DHT_Lookup::Access_Three_DHT(lookupID);

    bool should_Go_In_0 = DHT::Compare(entry.id, current_Three.entry[0].id, lookingFor) && current_Three.entry[0].is_set;
    if(should_Go_In_0)
    {
        Put_In_First(entry, lookupID, current_Three);
        return;
    }

    bool should_Go_In_1 = DHT::Compare(entry.id, current_Three.entry[1].id, lookingFor) && current_Three.entry[1].is_set;
    if(should_Go_In_1)
    {
        Put_In_Second(entry, lookupID, current_Three);
        return;
    }

    bool should_Go_In_2 = DHT::Compare(entry.id, current_Three.entry[2].id, lookingFor) && current_Three.entry[2].is_set;
    if(should_Go_In_2)
    {
        Put_In_Third(entry, lookupID, current_Three);
        return;
    }


}


void Minor_Functions::Put_In_First(DHT_Single_Entry entry, int lookupID, three_DHT putIn)
{
    putIn.entry[2] = putIn.entry[1];
    putIn.entry[1] = putIn.entry[0];
    putIn.entry[0] = entry;

    DHT_Lookup::Write_To_Three_DHT(putIn, lookupID);


}

void Minor_Functions::Put_In_Second(DHT_Single_Entry entry, int lookupID, three_DHT putIn)
{
    putIn.entry[2] = putIn.entry[1];
    putIn.entry[1] = entry;

    DHT_Lookup::Write_To_Three_DHT(putIn, lookupID);

}

void Minor_Functions::Put_In_Third(DHT_Single_Entry entry, int lookupID, three_DHT putIn)
{
    putIn.entry[2] = entry;

    DHT_Lookup::Write_To_Three_DHT(putIn, lookupID);

}
