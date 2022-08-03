
#include "DHT_Search.h"
#include "DHT.h"
#include "DHT_Access.h"

using namespace paft;



DHT_Single_Entry DHT_Search::Next_Closest_In_Bucket(int bucket, _160bitnumber id_to_find, _160bitnumber previous_id)
{
    if((bucket > 159) | (bucket < 0))
    {
        DHT_Single_Entry tmp;
        tmp.is_set = false;
        return tmp;
    }


    DHT_Single_Entry closest;
    DHT_Single_Entry access;

    closest.is_set = false;
    access.is_set = false;

    for(int i=0;i<20;i++)
    {
        access = DHT_Access::Access_DHT(bucket*20+i);
        if(access.is_set)
        {
            if(!closest.is_set)
            {
                if(DHT::Compare(previous_id, access.id, id_to_find))
                    closest=access;
            }
            else
            {
                if(DHT::Compare(access.id, closest.id, id_to_find) && DHT::Compare(previous_id, access.id, id_to_find))
                    closest=access;


            }
        }

    }

    return closest;

}






three_DHT DHT_Search::Lookup(_160bitnumber id)
{
    int bucket = DHT::Distance(id, DHT_Access::Get_Self_ID());
    three_DHT closest = DHT_Search::Lookup_One_Bucket(id, bucket);
    int entryCounter = 0;

    if(closest.entry[2].is_set)
        return closest;
    else if(closest.entry[1].is_set)
        entryCounter = 2;
    else if(closest.entry[0].is_set)
        entryCounter = 1;
    else
        entryCounter = 0;




    int bucket_counter=1;
    _160bitnumber previous_UpID = id;
    _160bitnumber previous_DownID = id;

    DHT_Single_Entry upEntry;
    DHT_Single_Entry downEntry;

    while( ((bucket+bucket_counter <= 159) | (bucket-bucket_counter >= 0)) & (entryCounter < 3 ))
    {
        upEntry =   DHT_Search::Next_Closest_In_Bucket(bucket+bucket_counter, id, previous_UpID);
        downEntry = DHT_Search::Next_Closest_In_Bucket(bucket-bucket_counter, id, previous_DownID);

        while( (upEntry.is_set | downEntry.is_set) & (entryCounter < 3) )
        {
            if(upEntry.is_set && downEntry.is_set)
            {
                if(DHT::Compare(upEntry.id, downEntry.id, id))
                {
                    closest.entry[entryCounter] = upEntry;
                    previous_UpID = upEntry.id;
                    upEntry =   DHT_Search::Next_Closest_In_Bucket(bucket+bucket_counter, id, previous_UpID);
                }
                else
                {
                    closest.entry[entryCounter] = downEntry;
                    previous_DownID = downEntry.id;
                    downEntry = DHT_Search::Next_Closest_In_Bucket(bucket-bucket_counter, id, previous_DownID);
                }
                entryCounter++;
            }
            else
            {
                if(upEntry.is_set)
                {
                    closest.entry[entryCounter] = upEntry;
                    previous_UpID = upEntry.id;
                    upEntry =   DHT_Search::Next_Closest_In_Bucket(bucket+bucket_counter, id, previous_UpID);
                }
                else
                {
                    closest.entry[entryCounter] = downEntry;
                    previous_DownID = downEntry.id;
                    downEntry = DHT_Search::Next_Closest_In_Bucket(bucket-bucket_counter, id, previous_DownID);
                }
            }
        }

        bucket_counter++;
    }

    return closest;

}






three_DHT DHT_Search::Find_Value(_160bitnumber id)
{
    //Returns a three_DHT either an exact match in the FileIds or the closest peers
    //(In that order)

    three_DHT closest;
    int closest_counter=0;

    for(int i=0;i<100;i++)
    {
        DHT_Single_Entry tmp = DHT_Access::Access_File_IDs(i);
        if(tmp.is_set)
        {
            if(DHT::Is_Equal(id, tmp.id))
            {
                if(closest_counter < 3)
                    closest.entry[closest_counter] = tmp;
                closest_counter++;

            }

        }

    }

    if(closest_counter == 0)
        return DHT_Search::Lookup(id);
    else if(closest_counter >=3)
        return closest;
    else
    {
        int tmp_counter=0;
        three_DHT tmp = DHT_Search::Lookup(id);
        while(closest_counter<3)
        {
            closest.entry[closest_counter] = tmp.entry[tmp_counter];
            tmp_counter++;
            closest_counter++;

        }
    }
    return closest;



}






three_DHT DHT_Search::Lookup_One_Bucket(_160bitnumber id, int bucket)
{

    three_DHT closest;
    for(int i=0;i<3;i++)
        closest.entry[i].is_set =false;


    DHT_Single_Entry tmp, tmp2;
    DHT_Single_Entry access;
    for(int i=0;i<20;i++)
    {
        access = DHT_Access::Access_DHT(bucket*20+i);
        if(access.is_set)
        {
            if(closest.entry[0].is_set)
            {
                if(DHT::Compare(access.id,closest.entry[0].id, id))
                {
                    //The new DHT_ALL is closer then closest.entry[0]
                    //Move set the closest[0] and move the rest down
                    tmp = closest.entry[0];
                    closest.entry[0] = access;
                    tmp2 = closest.entry[1];
                    closest.entry[1] = tmp;
                    closest.entry[2] = tmp2;
                }
                else if(closest.entry[1].is_set)
                {
                    if(DHT::Compare(access.id,closest.entry[1].id, id))
                    {
                        tmp = closest.entry[1];
                        closest.entry[1] = access;
                        closest.entry[2] = tmp;

                    }
                    else if(access.id,closest.entry[2].is_set)
                    {
                        if(DHT::Compare(access.id,closest.entry[2].id, id))
                        {
                            closest.entry[2] = access;

                        }
                    }
                    else
                    {
                        closest.entry[2] = access;
                    }


                }
                else
                {
                    closest.entry[1] = access;
                }


            }
            else
            {
                closest.entry[0] = access;

            }


        }

    }

    return closest;



}

