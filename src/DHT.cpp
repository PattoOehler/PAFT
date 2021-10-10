#include "../include/DHT.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>

using namespace paft;

DHT_Single_Entry* DHT::DHT_ALL = new DHT_Single_Entry[160*20];
_160bitnumber* DHT::SELF = new _160bitnumber;



int DHT::Distance_To_Self(_160bitnumber id)
{
    unsigned long long top_distance = id.top ^ SELF->top;
    unsigned long long mid_distance = id.mid ^ SELF->mid;
    unsigned long bot_distance = id.bot ^ SELF->bot;

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


    int distance = 0;






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

int DHT::Insert_Entry(int distance, DHT_Single_Entry Entry)
{
    for(int i=0; i<20; i++)
    {

        if(DHT::DHT_ALL[(distance*20)+i].is_set == false)
        {
            DHT::DHT_ALL[(distance*20)+i] = Entry;
            DHT::DHT_ALL[(distance*20)+i].is_set = true;
            DHT::DHT_ALL[(distance*20)+i].time_To_Timeout = time(0);
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


int DHT::Add_Entry(DHT_Single_Entry Entry)
{

    unsigned long long top_distance = Entry.id.top ^ SELF->top;
    unsigned long long mid_distance = Entry.id.mid ^ SELF->mid;
    unsigned long bot_distance = Entry.id.bot ^ SELF->bot;

    int distance = DHT::Distance_To_Self(Entry.id);
    Insert_Entry(distance, Entry);



    return 0;
}


int DHT::Test_Add_Entry()
{

    DHT_Single_Entry Testing;

    Testing.id.top = 8446744073709551615; //18446744073709551615 is max int
    Testing.id.mid = 999999999999999999;
    Testing.id.bot = 123123123;


    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);
    //DHT::Add_Entry(Testing);

    std::random_device rd;   // non-deterministic generator
    std::mt19937_64 gen(rd()^time(NULL)); // With this set gen() will give a psudo random 64 bit(unsigned long long) int TODO make random

    long long unsigned int tmp_top = gen();
    long long unsigned int tmp_mid = gen();
    long unsigned int tmp_bot = gen() >> 32;

    Testing.id.top = tmp_top;
    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);

    Testing.id.mid = tmp_mid;
    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);

    Testing.id.bot = tmp_bot;

    // VVVV
    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);


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
