#include "../include/DHT.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>
#include <mutex>


using namespace paft;

DHT_Single_Entry* DHT::DHT_ALL = new DHT_Single_Entry[160*20];
DHT_Single_Entry* DHT::FileIds = new DHT_Single_Entry[100]; //Temporarily as 100, might need to change later
_160bitnumber* DHT::SELF = new _160bitnumber;
unsigned short int DHT::SELF_PORT = 1234; //Default value- should be set before accessed
std::mutex* DHT::mutex_All = new std::mutex[160*20];
std::mutex* DHT::mutex_FileIds = new std::mutex[100];



void DHT::Store_FileId(DHT_Single_Entry entry)
{
    //This stores the FileId if there is less then 3 entries already for that ID
    //And does not write anything otherwise-or if its full


    int i=0;
    int counter=0;
    int first_pos=-1;
    while((i<100) & (counter<3))
    {
        DHT_Single_Entry tmp = entry;
        if(tmp.is_set)
        {
            if(IsEqual(tmp.id, entry.id))
            {
                counter++;
            }

        }
        else
        {
            if(first_pos == -1)
            {
                first_pos=i;
            }
        }
        i++;


    }
    if(counter >= 3)
    {
        return;
    }
    if(first_pos != 1)
        Write_To_FileIds(entry,first_pos);

}



DHT_Single_Entry DHT::Access_FileIds(int position)
{
    if((position >= 100) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    mutex_FileIds[position].lock();
    DHT_Single_Entry tmp = FileIds[position];
    mutex_FileIds[position].unlock();

    return tmp;
}

void DHT::Write_To_FileIds(DHT_Single_Entry write, int position)
{
    if((position >= 100) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    mutex_FileIds[position].lock();
    FileIds[position] = write;
    mutex_FileIds[position].unlock();
}






bool DHT::IsEqual(_160bitnumber id,_160bitnumber id2)
{
    if(id.top == id2.top)
    {
        if(id.mid == id2.mid)
        {

            if(id.bot == id2.bot)
            {
                return true;
            }
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
        DHT_Single_Entry tmp = Access_FileIds(i);
        if(tmp.is_set)
        {
            if(IsEqual(id, tmp.id))
            {
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



DHT_Single_Entry DHT::Access_DHT(int position)
{
    if((position >= 160*20) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    mutex_All[position].lock();
    DHT_Single_Entry tmp = DHT_ALL[position];
    mutex_All[position].unlock();

    return tmp;
}

void DHT::Write_To_DHT(DHT_Single_Entry write, int position)
{
    if((position >= 160*20) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    mutex_All[position].lock();
    DHT_ALL[position] = write;
    mutex_All[position].unlock();
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
    DHT_Single_Entry access;
    for(int i=0;i<20;i++)
    {
        access = Access_DHT(bucket*20+i);
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

    //std::cout << "log is receiving " << n << std::endl;
    int logValue = -1;
    while (n!=0) {//
        logValue++;

        n >>= 1;
    }
    //std::cout << "Log value is " << logValue << std::endl;
    return logValue;


}






int DHT::Distance(_160bitnumber id, _160bitnumber id2)
{
    unsigned long long int top_distance = id.top ^ id2.top;
    unsigned long long int mid_distance = id.mid ^ id2.mid;
    unsigned long int bot_distance = id.bot ^ id2.bot;

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

    std::cout << "Updating DHT with "  << inet_ntoa(Update.addr) <<":" << Update.port << "\n";

    int distance = Distance(Update.id, *SELF);

    for(int i=0; i<20; i++)
    {
        DHT_Single_Entry tmp = Access_DHT(distance*20+i);

        if(tmp.is_set == true)
        {
            if(tmp.id.top == Update.id.top && tmp.id.mid == Update.id.mid && tmp.id.bot == Update.id.bot)
            {
                Update.time_To_Timeout = time(0)+60*60; // 1 hour
                Write_To_DHT(Update, distance*20+i);

                return;
            }

        }

    }

    DHT::Add_Entry(Update);




}

void DHT::Set_Self_Port(unsigned short int port)
{
    SELF_PORT = port;

}
unsigned short int DHT::Get_Self_Port()
{
    return SELF_PORT;

}




void DHT::Init(){

    //This will set the SELF


    //First check if the number is already set - if so should keep probs TODO

    std::random_device rd;   // non-deterministic generator
    std::mt19937_64 gen(rd()^time(NULL)); // With this set gen() will give a psudo random 64 bit(unsigned long long) int TODO make random

    SELF->top = gen();
    SELF->mid = gen();

    SELF->bot = gen() >> 32;

    std::cout << SELF->top << " "<< SELF->mid << " "<< SELF->bot << std::endl;




    return;
}



_160bitnumber DHT::Get_SELF()
{
    return *SELF;

}


int DHT::Add_Entry(DHT_Single_Entry Entry)
{

    int distance = DHT::Distance(Entry.id, *SELF);


    for(int i=0; i<20; i++)
    {
        DHT_Single_Entry tmp = Access_DHT(distance*20+i);

        if(tmp.is_set == false)
        {
            //DHT::DHT_ALL[(distance*20)+i] = Entry;
            //DHT::DHT_ALL[(distance*20)+i].time_To_Timeout = time(0);
            //DHT::DHT_ALL[(distance*20)+i].is_set = true;
            Entry.is_set = true;
            Entry.time_To_Timeout = time(0)+60*60; //1 Hour
            Write_To_DHT(Entry, distance*20+i);
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

    DHT_Single_Entry Testing, ard;

    Testing.id.top = 8446744073709551615; //18446744073709551615 is max int
    Testing.id.mid = 999999999999999999;
    Testing.id.bot = 123123123;


    std::random_device rd;   // non-deterministic generator
    std::mt19937_64 gen(rd()^time(NULL)); // With this set gen() will give a psudo random 64 bit(unsigned long long) int TODO make random

    long long unsigned int tmp_top = gen();
    long long unsigned int tmp_mid = gen();
    long unsigned int tmp_bot = gen() >> 32;

    //tmp_top = SELF.top;
    //tmp_mid = SELF.mid;
    //tmp_bot = SELF.bot;

    Testing.id.top = SELF->top;
    Testing.id.mid = SELF->mid;
    Testing.id.bot = SELF->bot;

    tmp_top = 1;
    for(int i=0; i<64; i++)
    {
        Testing.id.top = SELF->top ^ (tmp_top);
        tmp_top = tmp_top<<1;
        std::cout << tmp_top << " is what we are sending\n";
        Update_Time(Testing);
        //Testing.id.top += 1;
        //Update_Time(Testing);

    }
    ard = Testing;
    Testing.id.top = SELF->top;
    tmp_mid = 1;
    for(int i=0; i<64; i++)
    {
        Testing.id.mid = SELF->mid ^ (tmp_mid);
        tmp_mid = tmp_mid<<1;
        //std::cout << tmp_top << " is what we are sending\n";
        Update_Time(Testing);
        //Testing.id.mid +=1;
        //Update_Time(Testing);

    }

    Testing.id.mid = SELF->mid;
    tmp_bot = 1;
    for(int i=0; i<32; i++)
    {
        Testing.id.bot = SELF->bot ^ (tmp_bot);
        tmp_bot = tmp_bot<<1;
        //std::cout << tmp_top << " is what we are sending\n";
        Update_Time(Testing);
        //Testing.id.bot += 1;
        //Update_Time(Testing);

    }
        //^^^^^^
    //This puts one entry into every k-bucket


    three_DHT asdf = Lookup(ard.id);
    for(int i=0;i<3;i++)
    {

        std::cout << "Lookup \n";
        std::cout << std::hex << asdf.entry[i].id.top <<
                             std::hex << asdf.entry[i].id.mid <<
                             std::hex << asdf.entry[i].id.bot << std::endl;

    }


    return 0;
}

void DHT::Print_DHT()
{

    for(int i=0;i<160;i++)
    {
        for(int j=0;j<20;j++)
        {
            DHT_Single_Entry tmp = Access_DHT(20*i+j);
            if(tmp.is_set)
            {
                std::cout << std::hex << tmp.id.top <<
                             std::hex << tmp.id.mid <<
                             std::hex << tmp.id.bot;
                std::cout << "    In k-bucket " << std::dec << (i) << " position " << j << " with timeout time "<<
                ctime(&tmp.time_To_Timeout ); //ctime has a \n at the end
            }
                else
            {
                //std::cout << "Causing break on " << i*20+j << std::endl;
                break;
            }


        }

    }


    return;
}
