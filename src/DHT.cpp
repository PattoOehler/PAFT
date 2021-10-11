#include "../include/DHT.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>

using namespace paft;

DHT_Single_Entry* DHT::DHT_ALL = new DHT_Single_Entry[160*20];
_160bitnumber* DHT::SELF = new _160bitnumber;


three_DHT DHT::Lookup_One_Bucket(_160bitnumber id, int bucket)
{
    /*
    three_DHT closest;
    int closest_counter=0;
    for(int i=0;i<3;i++)
        closest.entry[i].is_set =false;


    int counter=0;
    for(int i=0;i<20;i++)
    {
        if(DHT::DHT_ALL[bucket*20+i].is_set)
            counter++;

    }

    if(counter <= 3)
    {
        //There are less then 3 entries in the k-bucket to return
        for(int i=0;i<20;i++)
        {
            if(DHT::DHT_ALL[distance*20+i].is_set)
            {
                closest.entry[closest_counter] = DHT::DHT_ALL[distance*20+i];
                closest_counter++;
            }


        }

    }
    else
    {

        bool set_first=false;
        //There are more then 3 entries in the k-bucket
        for(int i=0;i<20;i++)
        {
            if(DHT::DHT_ALL[distance*20+i].is_set)
            {
                if(set_first)
                    closest.entry[0] = DHT::DHT_ALL[distance*20+i];
                else
                {
                    //The first entry is already set
                    int distance_checking = Distance(DHT::DHT_ALL[distance*20+i].id, id);
                    int distance_first = Distance(closest.entry[0], id);
                    if(distance_checking < distance_first)
                    {
                        //The distance is less in the
                        DHT_Single_Entry tmp;
                        tmp =

                    }


                }

            }



        }

    }
    */

}



three_DHT DHT::Lookup(_160bitnumber id)
{
    three_DHT closest;
    int closest_counter=0;

    int distance = Distance(id, *SELF);

    int counter=0;
    for(int i=0;i<20;i++)
    {
        if(DHT::DHT_ALL[distance*20+i].is_set)
            counter++;

    }


    if(counter <= 3)
    {
        //There are less then 3 entries in the k-bucket to return
        for(int i=0;i<20;i++)
        {
            if(DHT::DHT_ALL[distance*20+i].is_set)
            {
                closest.entry[closest_counter] = DHT::DHT_ALL[distance*20+i];
                closest_counter++;
            }


        }
    }
    else
    {
    //There are enough to fill up the closest


    }

    return closest;

}








int DHT::Distance(_160bitnumber id, _160bitnumber id2)
{
    unsigned long long top_distance = id.top ^ id2.top;
    unsigned long long mid_distance = id.mid ^ id2.mid;
    unsigned long bot_distance = id.bot ^ id2.bot;

    //This is the distance formula
    if(top_distance != 0)
    {
        return log2(top_distance);

    }
    else
    {
        if(mid_distance != 0)
        {
            return 64+log2(mid_distance);

        }
        else
        {
            if(bot_distance != 0)
                return 128+log2(mid_distance);
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
