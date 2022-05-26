#include "DHT.h"
#include "DHT_Access.h"
#include "DHT_Updater.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>
#include <mutex>
#include <ws2tcpip.h>
#include <thread>


using namespace paft;


std::mt19937_64 DHT::gen(time(nullptr));


DHT_Single_Entry DHT::Next_Closest_In_Bucket(int bucket, _160bitnumber id_to_find, _160bitnumber previous_id)
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
                if(Compare(previous_id, access.id, id_to_find))
                    closest=access;
            }
            else
            {
                if(Compare(access.id, closest.id, id_to_find) && Compare(previous_id, access.id, id_to_find))
                    closest=access;


            }
        }

    }

    return closest;

}


three_DHT DHT::Lookup(_160bitnumber id)
{

    int bucket = Distance(id, DHT_Access::Get_SELF_ID());
    three_DHT closest = Lookup_One_Bucket(id, bucket);
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
        upEntry =   Next_Closest_In_Bucket(bucket+bucket_counter, id, previous_UpID);
        downEntry = Next_Closest_In_Bucket(bucket-bucket_counter, id, previous_DownID);

        while( (upEntry.is_set | downEntry.is_set) & (entryCounter < 3) )
        {
            if(upEntry.is_set && downEntry.is_set)
            {
                if(Compare(upEntry.id, downEntry.id, id))
                {
                    closest.entry[entryCounter] = upEntry;
                    previous_UpID = upEntry.id;
                    upEntry =   Next_Closest_In_Bucket(bucket+bucket_counter, id, previous_UpID);
                }
                else
                {
                    closest.entry[entryCounter] = downEntry;
                    previous_DownID = downEntry.id;
                    downEntry = Next_Closest_In_Bucket(bucket-bucket_counter, id, previous_DownID);
                }
                entryCounter++;
            }
            else
            {
                if(upEntry.is_set)
                {
                    closest.entry[entryCounter] = upEntry;
                    previous_UpID = upEntry.id;
                    upEntry =   Next_Closest_In_Bucket(bucket+bucket_counter, id, previous_UpID);
                }
                else
                {
                    closest.entry[entryCounter] = downEntry;
                    previous_DownID = downEntry.id;
                    downEntry = Next_Closest_In_Bucket(bucket-bucket_counter, id, previous_DownID);
                }
            }
        }

        bucket_counter++;
    }

    return closest;

}






bool DHT::IsEqual(_160bitnumber id,_160bitnumber id2)
{
    if(id.top == id2.top)
    {
        if(id.mid == id2.mid)
        {
            if(id.bot == id2.bot)
                return true;
        }

    }
    return false;


}

three_DHT DHT::Find_Value(_160bitnumber id)
{
    //Returns a three_DHT either an exact match in the FileIds or the closest peers
    //(In that order)

    three_DHT closest;
    int closest_counter=0;

    for(int i=0;i<100;i++)
    {
        DHT_Single_Entry tmp = DHT_Access::Access_FileIds(i);
        if(tmp.is_set)
        {
            if(IsEqual(id, tmp.id))
            {
                if(closest_counter < 3)
                    closest.entry[closest_counter] = tmp;
                closest_counter++;

            }

        }

    }

    if(closest_counter == 0)
        return Lookup(id);
    else if(closest_counter >=3)
        return closest;
    else
    {
        int tmp_counter=0;
        three_DHT tmp = Lookup(id);
        while(closest_counter<3)
        {
            closest.entry[closest_counter] = tmp.entry[tmp_counter];
            tmp_counter++;
            closest_counter++;

        }
    }
    return closest;



}






//Returns true if the first id is closer to the compare_To false otherwise
bool DHT::Compare(_160bitnumber id,_160bitnumber id2, _160bitnumber compare_To)
{
    unsigned long long top_distance1 = id.top ^ compare_To.top;
    unsigned long long mid_distance1 = id.mid ^ compare_To.mid;
    unsigned long bot_distance1 = id.bot ^ compare_To.bot;

    unsigned long long top_distance2 = id2.top ^ compare_To.top;
    unsigned long long mid_distance2 = id2.mid ^ compare_To.mid;
    unsigned long bot_distance2 = id2.bot ^ compare_To.bot;


    if(top_distance1 != top_distance2)
        return top_distance1 < top_distance2 ? true : false;

    else if(mid_distance1 != mid_distance2)
        return mid_distance1 < mid_distance2 ? true : false;

    else if(bot_distance1 != bot_distance2)
        return bot_distance1 < bot_distance2 ? true : false;

    //Returns false when id=id2
    else
        return false;



}




three_DHT DHT::Lookup_One_Bucket(_160bitnumber id, int bucket)
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
                if(Compare(access.id,closest.entry[0].id, id))
                {
                    //The new DHT_ALL is closer then closest.entry[0]
                    //Move set the closest[0] and move the rest down
                    tmp = closest.entry[0];
                    closest.entry[0] = access;
                    tmp2 = closest.entry[1];
                    closest.entry[1] = tmp;
                    closest.entry[2] = tmp2;
                }
                else if(Compare(access.id,closest.entry[1].id, id))
                {
                    tmp = closest.entry[1];
                    closest.entry[1] = access;
                    closest.entry[2] = tmp;

                }
                else if(Compare(access.id,closest.entry[2].id, id))
                {
                    closest.entry[2] = access;

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







int DHT::Log2(unsigned long long int n)
{
    if (n == 0)
    {
        return 0;
    }

    int logValue = -1;
    while (n!=0)
    {
        logValue++;
        n >>= 1;
    }
    return logValue;

}






int DHT::Distance(_160bitnumber id, _160bitnumber id2)
{
    unsigned long long int top_distance = id.top ^ id2.top;
    unsigned long long int mid_distance = id.mid ^ id2.mid;
    unsigned long int bot_distance = id.bot ^ id2.bot;


    if(top_distance != 0)
        return Log2(top_distance);

    else
    {
        if(mid_distance != 0)
            return 64+Log2(mid_distance);

        else
        {
            if(bot_distance != 0)
                return 128+Log2((unsigned long long int)bot_distance);
            else
                return  159;
        }
    }




}




void DHT::Update_Time(DHT_Single_Entry Update)
{

    int distance = Distance(Update.id, DHT_Access::Get_SELF_ID());

    for(int i=0; i<20; i++)
    {
        DHT_Single_Entry tmp = DHT_Access::Access_DHT(distance*20+i);

        if(tmp.is_set == true)
        {

            if(IsEqual(tmp.id, Update.id))
            {
                Update.time_To_Timeout = time(0)+60*60; // 1 hour
                Update.is_set = true;
                DHT_Access::Write_To_DHT(Update, distance*20+i);

                return;
            }

        }

    }

    DHT::Add_Entry(Update);




}





void DHT::Init()
{
    std::thread thread(DHT_Updater::Keep_DHT_Updated);
    thread.detach();


    _160bitnumber SELF = DHT::Random_ID();


    std::cout << "SELF: ";
    DHT::Print_ID(SELF);
    std::cout << "\n";


    DHT_Access::Set_Self_ID(SELF);


    return;

}


int DHT::Add_Entry(DHT_Single_Entry Entry)
{

    int distance = DHT::Distance(Entry.id, DHT_Access::Get_SELF_ID());


    for(int i=0; i<20; i++)
    {
        DHT_Single_Entry tmp = DHT_Access::Access_DHT(distance*20+i);

        if(tmp.is_set == false)
        {
            Entry.is_set = true;
            Entry.time_To_Timeout = time(0)+60*60; //1 Hour
            DHT_Access::Write_To_DHT(Entry, distance*20+i);
            break;
        }

    }


    return 0;
}





int DHT::Add_Entry_All_Buckets()
{

    DHT_Single_Entry Testing;


    DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
    if(!a.is_set)
    {
        std::cout << "DHT Position 159*20 IS NOT SET DHT::Add_Entry_All_Buckets cannot run!\n\n";
        return -1;
    }

    Testing.addr = a.addr;
    Testing.port = a.port;


    long long unsigned int tmp_top;
    long long unsigned int tmp_mid;
    long unsigned int tmp_bot;


    _160bitnumber SELF = DHT_Access::Get_SELF_ID();
    Testing.id.bot = SELF.bot;
    Testing.id.mid = SELF.mid;

    tmp_top = 1;
    for(int i=0; i<64; i++)
    {
        Testing.id.top = SELF.top ^ tmp_top;
        tmp_top = tmp_top<<1;
        Update_Time(Testing);


    }

    Testing.id.top = SELF.top;

    tmp_mid = 1;
    for(int i=0; i<64; i++)
    {
        Testing.id.mid = SELF.mid ^ tmp_mid;
        tmp_mid = tmp_mid<<1;
        Update_Time(Testing);


    }

    Testing.id.mid = SELF.mid;
    tmp_bot = 1;
    for(int i=0; i<32; i++)
    {
        Testing.id.bot = SELF.bot ^ tmp_bot;
        tmp_bot = tmp_bot<<1;
        Update_Time(Testing);

    }


    return 0;
}

void DHT::Print_DHT()
{

    for(int i=0;i<160;i++)
    {
        for(int j=0;j<20;j++)
        {
            DHT_Single_Entry tmp = DHT_Access::Access_DHT(20*i+j);
            if(tmp.is_set)
            {
                DHT::Print_ID(tmp.id);
                std::cout << "    In k-bucket " << std::dec << (i) << " position " << j << " with timeout time "<<
                ctime(&tmp.time_To_Timeout ); //ctime has a \n at the end
            }


        }

    }


    return;
}


void DHT::Print_ID(_160bitnumber id_to_print)
{
    unsigned char *pointer =  (unsigned char *) &id_to_print;
    for(int i=0; i<20; i++)
    {
        int counter =0;
        while(pointer[i] != 0)
        {
            pointer[i] --;
            counter++;
        }

        int amount[2] = {counter/16, counter%16};
        char currentLetter;
        for(int i=0;i < 2; i++)
        {
            if(amount[i] > 9)
                currentLetter = 'A' + amount[i]-10;
            else
                currentLetter = '0' + amount[i];
            std::cout << currentLetter;
        }

    }



}


void DHT::Print_Files()
{

    for(int i=0;i<100;i++)
    {

        DHT_Single_Entry tmp = DHT_Access::Access_FileIds(i);
        if(tmp.is_set)
        {
            DHT::Print_ID(tmp.id);
            std::cout << "    In part " << i << "\n";
        }

    }


    return;
}


_160bitnumber DHT::Random_ID()
{
    _160bitnumber random_ID;

    //Produces pseudo-random Numbers based upon startup time.
    //TODO make random numbers cryptographically secure
    random_ID.top = gen();
    random_ID.mid = gen();
    random_ID.bot = gen() >> 32;

    return random_ID;
}





void DHT::Print_File_Locations()
{

    for(int i=0;i<100;i++)
    {
        std::string local_File = DHT_Access::Get_Local_File_Location(i);
        if(!local_File.empty())
            std::cout << i << " " << local_File << "\n";

    }


}



















