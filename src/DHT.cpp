#include "../include/DHT.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>

using namespace paft;

DHT::DHT(){

    //This will set the SELF


    //First check if the number is already set - if so should keep probs TODO
        //Ya know do stuff - gonna just set it for now

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


    if(top_distance == 0 )
    {
        //Distance is less then the top 64 bits
        if(mid_distance == 0)
        {
            if(bot_distance == 0)
            {
                //They are the same number -- log2 of 0 is -infinity
                DHT::Insert_Entry(160, Entry); // They are the same number
            }else
            {
                unsigned int Distance = (int)(log2(bot_distance));
                DHT::Insert_Entry(Distance+128, Entry); //+128 because the top_distance is in the first 127 the mid section starts at 128
            }


        }else
        {

            int Distance = (int)(log2(mid_distance));
            DHT::Insert_Entry(Distance+64, Entry); //+64 because the top_distance is in the first 63 the mid section starts at 64
            //std::cout << Distance << " is the mid_distance " << mid_distance << std::endl;
        }




    }else
    {
        //Distance is in the top 64 bits
        int Distance = (int)(log2(top_distance));
        //std::cout << "Distance is == " << top_distance << " (log of)\n";
        DHT::Insert_Entry(Distance, Entry);

    }

    return 0;
}


int DHT::Test_Add_Entry()
{
    DHT_Single_Entry Testing;

    Testing.id.top = 8446744073709551615; //18446744073709551615 is max int

    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);

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
    DHT::Add_Entry(Testing);
    DHT::Add_Entry(Testing);


    //std::cout << "Hello";
    //DHT::Print_DHT();


    return 0;
}

void DHT::Print_DHT()
{

    for(int i=0;i<161;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(DHT::DHT_ALL[i*20+j].is_set)
            {
                std::cout << std::hex << DHT::DHT_ALL[i*20+j].id.top <<
                             std::hex << DHT::DHT_ALL[i*20+j].id.mid <<
                             std::hex << DHT::DHT_ALL[i*20+j].id.bot;
                std::cout << "    " << std::dec << (i) << ' ' << j << std::endl;
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
