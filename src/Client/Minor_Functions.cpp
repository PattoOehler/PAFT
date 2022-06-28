
#include "Minor_Functions.h"
#include "../DHT/DHT_Lookup.h"
#include "../DHT/DHT.h"
#include "Main_Client.h"

#include <iostream>

using namespace paft;





void Minor_Functions::Do_Lookup_If_Closer(int lookupID, three_DHT received_values, _160bitnumber lookingFor)
{
    three_DHT current_Three = DHT_Lookup::Access_Three_DHT(lookupID);


    bool is_0_In_Current_Three = DHT::Is_Equal(received_values.entry[0].id,  current_Three.entry[0].id) | DHT::Is_Equal(received_values.entry[0].id,  current_Three.entry[1].id) |
                                            DHT::Is_Equal(received_values.entry[0].id,  current_Three.entry[2].id);
    bool is_1_In_Current_Three = DHT::Is_Equal(received_values.entry[1].id,  current_Three.entry[0].id) | DHT::Is_Equal(received_values.entry[1].id,  current_Three.entry[1].id) |
                                            DHT::Is_Equal(received_values.entry[1].id,  current_Three.entry[2].id);
    bool is_2_In_Current_Three = DHT::Is_Equal(received_values.entry[2].id,  current_Three.entry[0].id) | DHT::Is_Equal(received_values.entry[2].id,  current_Three.entry[1].id) |
                                            DHT::Is_Equal(received_values.entry[2].id,  current_Three.entry[2].id);

    //If any of the values gathered is closer then the furthest away one stored & not already searched then perform the search.
    if(current_Three.entry[0].is_set)
    {
        bool is0Closer = DHT::Compare(received_values.entry[0].id, current_Three.entry[2].id, lookingFor);
        if(is0Closer && (!is_0_In_Current_Three))
        {
            Main_Client client(received_values.entry[0].addr, received_values.entry[0].port);
            client.Find_Node_Recursive(lookingFor, lookupID);

        }
    }
    if(current_Three.entry[1].is_set)
    {
        bool is1Closer = DHT::Compare(received_values.entry[1].id, current_Three.entry[2].id, lookingFor);
        if(is1Closer && (!is_1_In_Current_Three))
        {
            Main_Client client(received_values.entry[1].addr, received_values.entry[1].port);
            client.Find_Node_Recursive(lookingFor, lookupID);

        }
    }

    if(current_Three.entry[2].is_set)
    {
    bool is2Closer = DHT::Compare(received_values.entry[2].id, current_Three.entry[2].id, lookingFor);
        if(is2Closer && (!is_2_In_Current_Three))
        {
            Main_Client client(received_values.entry[2].addr, received_values.entry[2].port);
            client.Find_Node_Recursive(lookingFor, lookupID);

        }
    }

    /*
    three_DHT current_Three = DHT_Lookup::Access_Three_DHT(lookupID);


    //If one of the values is not set(should be sorted) lookup all
    if(!current_Three.entry[2].is_set)
    {

        if(received_values.entry[0].is_set)
        {
            Main_Client client1(received_values.entry[0].addr, received_values.entry[0].port);
            client1.Find_Node_Recursive(lookingFor, lookupID);
        }

        if(received_values.entry[1].is_set)
        {
            Main_Client client2(received_values.entry[1].addr, received_values.entry[1].port);
            client2.Find_Node_Recursive(lookingFor, lookupID);
        }
        if(received_values.entry[2].is_set)
        {
            Main_Client client3(received_values.entry[2].addr, received_values.entry[2].port);
            client3.Find_Node_Recursive(lookingFor, lookupID);
        }
        return;
    }


    bool is_0_In_Current_Three = DHT::Is_Equal(received_values.entry[0].id,  current_Three.entry[0].id) | DHT::Is_Equal(received_values.entry[0].id,  current_Three.entry[1].id) |
                                            DHT::Is_Equal(received_values.entry[0].id,  current_Three.entry[2].id);
    bool is_1_In_Current_Three = DHT::Is_Equal(received_values.entry[1].id,  current_Three.entry[0].id) | DHT::Is_Equal(received_values.entry[1].id,  current_Three.entry[1].id) |
                                            DHT::Is_Equal(received_values.entry[1].id,  current_Three.entry[2].id);
    bool is_2_In_Current_Three = DHT::Is_Equal(received_values.entry[2].id,  current_Three.entry[0].id) | DHT::Is_Equal(received_values.entry[2].id,  current_Three.entry[1].id) |
                                            DHT::Is_Equal(received_values.entry[2].id,  current_Three.entry[2].id);

    //If any of the values gathered is closer then the furthest away one stored & not already searched then perform the search.
    bool is0Closer = DHT::Compare(received_values.entry[0].id, current_Three.entry[2].id, lookingFor);
    if(is0Closer && (!is_0_In_Current_Three))
    {
        Main_Client client(received_values.entry[0].addr, received_values.entry[0].port);
        client.Find_Node_Recursive(lookingFor, lookupID);

    }

    bool is1Closer = DHT::Compare(received_values.entry[1].id, current_Three.entry[2].id, lookingFor);
    if(is1Closer && (!is_1_In_Current_Three))
    {
        Main_Client client(received_values.entry[1].addr, received_values.entry[1].port);
        client.Find_Node_Recursive(lookingFor, lookupID);

    }

    bool is2Closer = DHT::Compare(received_values.entry[2].id, current_Three.entry[2].id, lookingFor);
    if(is2Closer && (!is_2_In_Current_Three))
    {
        Main_Client client(received_values.entry[2].addr, received_values.entry[2].port);
        client.Find_Node_Recursive(lookingFor, lookupID);

    }


    */

}










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
    //Access_Three_DHT_And_Lock needs to be unlocked
    three_DHT current_Three = DHT_Lookup::Access_Three_DHT_And_Lock(lookupID);

    bool should_Go_In_0 = DHT::Compare(entry.id, current_Three.entry[0].id, lookingFor) && current_Three.entry[0].is_set;
    if(should_Go_In_0)
    {
        Put_In_First(entry, lookupID, current_Three);
        DHT_Lookup::Unlock(lookupID);
        return;
    }

    bool should_Go_In_1 = DHT::Compare(entry.id, current_Three.entry[1].id, lookingFor) && current_Three.entry[1].is_set;
    if(should_Go_In_1)
    {
        Put_In_Second(entry, lookupID, current_Three);
        DHT_Lookup::Unlock(lookupID);
        return;
    }

    bool should_Go_In_2 = DHT::Compare(entry.id, current_Three.entry[2].id, lookingFor) && current_Three.entry[2].is_set;
    if(should_Go_In_2)
    {
        Put_In_Third(entry, lookupID, current_Three);
        DHT_Lookup::Unlock(lookupID);
        return;
    }

    DHT_Lookup::Unlock(lookupID);
}


void Minor_Functions::Put_In_First(DHT_Single_Entry entry, int lookupID, three_DHT putIn)
{
    putIn.entry[2] = putIn.entry[1];
    putIn.entry[1] = putIn.entry[0];
    putIn.entry[0] = entry;

    DHT_Lookup::Write_To_Locked_Three_DHT(putIn, lookupID);


}

void Minor_Functions::Put_In_Second(DHT_Single_Entry entry, int lookupID, three_DHT putIn)
{
    putIn.entry[2] = putIn.entry[1];
    putIn.entry[1] = entry;

    DHT_Lookup::Write_To_Locked_Three_DHT(putIn, lookupID);

}

void Minor_Functions::Put_In_Third(DHT_Single_Entry entry, int lookupID, three_DHT putIn)
{
    putIn.entry[2] = entry;

    DHT_Lookup::Write_To_Locked_Three_DHT(putIn, lookupID);

}
