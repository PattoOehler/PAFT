#include "../include/DHT.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>

using namespace paft;

DHT_Single_Entry* DHT::DHT_ALL = new DHT_Single_Entry[160*20];
_160bitnumber* DHT::SELF = new _160bitnumber;


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
    {
        return top_distance1 < top_distance2 ? true : false;

    }
    else if(mid_distance1 != mid_distance2)
    {
        return mid_distance1 < mid_distance2 ? true : false;

    }
    else if(bot_distance1 != bot_distance2)
    {
        return bot_distance1 < bot_distance2 ? true : false;

    }
    else
    {
        //Should not happen unless they are the same
        std::cout << "Error in DHT::Compare - they are the same";
        return false;
    }


}




three_DHT DHT::Lookup_One_Bucket(_160bitnumber id, int bucket)
{

    three_DHT closest;
    for(int i=0;i<3;i++)
        closest.entry[i].is_set =false;


    DHT_Single_Entry tmp, tmp2;
    for(int i=0;i<20;i++)
    {
        if(DHT::DHT_ALL[bucket*20+i].is_set)
        {
            if(closest.entry[0].is_set)
            {
                if(Compare(DHT::DHT_ALL[bucket*20+i].id,closest.entry[0].id, id))
                {
                    //The new DHT_ALL is closer then closest.entry[0]
                    //Move set the closest[0] and move the rest down
                    tmp = closest.entry[0];
                    closest.entry[0] = DHT::DHT_ALL[bucket*20+i];
                    tmp2 = closest.entry[1];
                    closest.entry[1] = tmp;
                    closest.entry[2] = tmp2;
                }
                else if(Compare(DHT::DHT_ALL[bucket*20+i].id,closest.entry[1].id, id))
                {
                    tmp = closest.entry[1];
                    closest.entry[1] = DHT::DHT_ALL[bucket*20+i];
                    closest.entry[2] = tmp;

                }
                else if(Compare(DHT::DHT_ALL[bucket*20+i].id,closest.entry[2].id, id))
                {
                    closest.entry[2] = DHT::DHT_ALL[bucket*20+i];

                }

            }
            else
            {
                closest.entry[0] = DHT::DHT_ALL[bucket*20+i];

            }


        }

    }

    return closest;



}



three_DHT DHT::Lookup(_160bitnumber id)
{
    int distance = Distance(id, *SELF);
    three_DHT closest = Lookup_One_Bucket(id, distance);

    if(closest.entry[2].is_set)
        return closest;

    three_DHT next_Buckets[2];

    bool run=true;


    int distance_Counter = 1;
    while(run)
    {
        if(closest.entry[2].is_set)
            return closest;

        //Test for the Buckets bounds and if both are out of range exit
        //If its just one it should not be set and the other will work
        if(distance-distance_Counter >= 0)
            next_Buckets[0] = Lookup_One_Bucket(id, distance-distance_Counter);
        else
        {
            next_Buckets[0].entry[0].is_set = false;
            next_Buckets[0].entry[1].is_set = false;
            next_Buckets[0].entry[2].is_set = false;

        }
        if(distance+distance_Counter <= 159)
            next_Buckets[1] = Lookup_One_Bucket(id, distance+distance_Counter);
        else
        {
            next_Buckets[1].entry[0].is_set = false;
            next_Buckets[1].entry[1].is_set = false;
            next_Buckets[1].entry[2].is_set = false;

        }

        if((distance+distance_Counter > 159) & (distance-distance_Counter < 0))
        {
            //Ran through the whole DHT and not all were found
            return closest;

        }
        distance_Counter++;

        //It should run a maximum of three times
        for(int three=0; three<3;three++)
        {

            if(next_Buckets[0].entry[0].is_set && next_Buckets[1].entry[0].is_set)
            {
                if(Compare(next_Buckets[0].entry[0].id, next_Buckets[1].entry[0].id, id))
                {
                    for(int i=0;i<3;i++)
                    {
                        if(!closest.entry[i].is_set)
                        {
                            closest.entry[i] = next_Buckets[0].entry[0];
                            next_Buckets[0].entry[0] = next_Buckets[0].entry[1];
                            next_Buckets[0].entry[1] = next_Buckets[0].entry[2];
                            next_Buckets[0].entry[2].is_set = false;
                            break;

                        }

                    }

                }
                else
                {
                    for(int i=0;i<3;i++)
                    {
                        if(!closest.entry[i].is_set)
                        {
                            closest.entry[i] = next_Buckets[1].entry[0];
                            next_Buckets[1].entry[0] = next_Buckets[1].entry[1];
                            next_Buckets[1].entry[1] = next_Buckets[1].entry[2];
                            next_Buckets[1].entry[2].is_set = false;
                            break;

                        }

                    }

                }


            }
            else if(next_Buckets[0].entry[0].is_set | next_Buckets[1].entry[0].is_set)
            {


                if(next_Buckets[0].entry[0].is_set)
                {

                    for(int i=0;i<3;i++)
                    {
                        if(!closest.entry[i].is_set)
                        {
                            closest.entry[i] = next_Buckets[0].entry[0];
                            next_Buckets[0].entry[0] = next_Buckets[0].entry[1];
                            next_Buckets[0].entry[1] = next_Buckets[0].entry[2];
                            next_Buckets[0].entry[2].is_set = false;
                            break;

                        }

                    }


                }
                else
                {
                    //next_Buckets[1].entry[0].is_set = true so set it to the most first possible place
                    for(int i=0;i<3;i++)
                    {
                        if(!closest.entry[i].is_set)
                        {
                            closest.entry[i] = next_Buckets[1].entry[0];
                            next_Buckets[1].entry[0] = next_Buckets[1].entry[1];
                            next_Buckets[1].entry[1] = next_Buckets[1].entry[2];
                            next_Buckets[1].entry[2].is_set = false;
                            break;

                        }

                    }


                }

            }
            else
            {
                //Breaks out of the 3 loops - not necessary but increases runtime
                break;
            }

        }
    }




    return closest;

}



int DHT::Log2(unsigned long long int n)
{

    if (n == 0) //throw ...
    {
        //Here we have error
        //you can use exception to handle this error or return Zero
        //throw  new exception(std::out_of_range("fault we don't have log2 0"));
        return 0;
    }

    int logValue = -1;
    while (n) {//
        logValue++;
        n >>= 1;
    }
    //std::cout << "Log value is " << logValue << std::endl;
    return logValue;


}






int DHT::Distance(_160bitnumber id, _160bitnumber id2)
{
    unsigned long long top_distance = id.top ^ id2.top;
    unsigned long long mid_distance = id.mid ^ id2.mid;
    unsigned long bot_distance = id.bot ^ id2.bot;

    //This is the distance formula
    if(top_distance != 0)
    {
        return Log2(top_distance);

    }
    else
    {
        if(mid_distance != 0)
        {
            return 64+Log2(mid_distance);

        }
        else
        {
            if(bot_distance != 0)
                return 128+Log2((unsigned long long int)bot_distance);
            else
            {
                return  159;
            }

        }
    }




}




void DHT::Update_Time(DHT_Single_Entry Update)
{


    int distance = Distance(Update.id, *SELF);

    for(int i=0; i<20; i++)
    {

        if(DHT::DHT_ALL[(distance*20)+i].is_set == true)
        {
            if(DHT::DHT_ALL[(distance*20)+i].id.top == Update.id.top && DHT::DHT_ALL[(distance*20)+i].id.mid == Update.id.mid && DHT::DHT_ALL[(distance*20)+i].id.bot == Update.id.bot)
            {
                DHT::DHT_ALL[(distance*20)+i].time_To_Timeout = time(0)+60*60; // 1 hour
                return;
            }

        }

    }

    DHT::Add_Entry(Update);




}



void DHT::Init(){

    //This will set the SELF


    //First check if the number is already set - if so should keep probs TODO

    std::random_device rd;   // non-deterministic generator
    std::mt19937_64 gen(rd()^time(NULL)); // With this set gen() will give a psudo random 64 bit(unsigned long long) int TODO make random

    SELF->top = gen();
    SELF->mid = gen();

    SELF->bot = gen() >> 32;

    std::cout << SELF->top << "  "<< SELF->mid << "  "<< SELF->bot << "  "<<std::endl;



    return;
}



int DHT::Add_Entry(DHT_Single_Entry Entry)
{

    int distance = DHT::Distance(Entry.id, *SELF);


    for(int i=0; i<20; i++)
    {

        if(DHT::DHT_ALL[(distance*20)+i].is_set == false)
        {
            DHT::DHT_ALL[(distance*20)+i] = Entry;
            DHT::DHT_ALL[(distance*20)+i].time_To_Timeout = time(0);
            DHT::DHT_ALL[(distance*20)+i].is_set = true;
            std::cout << "Inserted Entry in " << distance << std::endl;
            break;
        }
        else
        {
            std::cout << "Skipping this attempt trying the next one\n";
        }

    }


    return 0;
}


int DHT::Test_Add_Entry()
{

    DHT_Single_Entry Testing;

    Testing.id.top = 8446744073709551615; //18446744073709551615 is max int
    Testing.id.mid = 999999999999999999;
    Testing.id.bot = 123123123;


    DHT::Update_Time(Testing);
    DHT::Update_Time(Testing);
    //DHT::Add_Entry(Testing);

    std::random_device rd;   // non-deterministic generator
    std::mt19937_64 gen(rd()^time(NULL)); // With this set gen() will give a psudo random 64 bit(unsigned long long) int TODO make random

    long long unsigned int tmp_top = gen();
    long long unsigned int tmp_mid = gen();
    long unsigned int tmp_bot = gen() >> 32;

    Testing.id.top = tmp_top;
    DHT::Update_Time(Testing);
    DHT::Update_Time(Testing);

    Testing.id.mid = tmp_mid;
    DHT::Update_Time(Testing);
    DHT::Update_Time(Testing);

    Testing.id.bot = tmp_bot;

    // VVVV
    DHT::Update_Time(Testing);
    DHT::Update_Time(Testing);


    //std::cout << "Hello";
    //DHT::Print_DHT();


    return 0;
}

void DHT::Print_DHT()
{

    for(int i=0;i<160;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(DHT::DHT_ALL[i*20+j].is_set)
            {
                std::cout << std::hex << DHT::DHT_ALL[i*20+j].id.top <<
                             std::hex << DHT::DHT_ALL[i*20+j].id.mid <<
                             std::hex << DHT::DHT_ALL[i*20+j].id.bot;
                std::cout << "    In k-bucket " << std::dec << (i) << " position " << j << " with timeout time "<<
                ctime(&DHT::DHT_ALL[i*20+j].time_To_Timeout ); //ctime has a \n at the end
            }
                else
            {
                //std::cout << "Causing break on " << i*20+j << std::endl;
                break;
            }


        }

    }

    //DHT::DHT_ALL[(distance*20)+i]


    return;
}
