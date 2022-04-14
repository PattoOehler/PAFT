
#include "DHT_Lookup.h"
#include <iostream>

#define Lookup_Length 20



using namespace paft;

three_DHT *DHT_Lookup::three_DHT_For_Lookups = new three_DHT[Lookup_Length];
std::mutex *DHT_Lookup::three_DHT_For_Lookups_Mutex = new std::mutex[Lookup_Length];

bool *DHT_Lookup::three_DHT_Is_Available = new bool[Lookup_Length];
std::mutex *DHT_Lookup::is_Available_Mutex = new std::mutex[Lookup_Length];





int DHT_Lookup::Get_Available_Three_DHT()
{
    int i=0;
    for(;i<Lookup_Length;i++)
    {
        if(Access_To_Three_DHT_Is_Available(i))
        {
            Write_To_Three_DHT_Is_Available(false, i);
            return i;
        }


    }

    return -1;


}






void DHT_Lookup::Write_To_Three_DHT_Is_Available(bool write, int position)
{
    if((position >= Lookup_Length) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    is_Available_Mutex[position].lock();
    three_DHT_Is_Available[position] = write;
    is_Available_Mutex[position].unlock();

}

bool DHT_Lookup::Access_To_Three_DHT_Is_Available(int position)
{
    if((position >= Lookup_Length) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    is_Available_Mutex[position].lock();
    bool tmp = three_DHT_Is_Available[position];
    is_Available_Mutex[position].unlock();

    return tmp;

}



three_DHT DHT_Lookup::Access_Three_DHT(int position)
{
    if((position >= Lookup_Length) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }


    three_DHT_For_Lookups_Mutex[position].lock();
    three_DHT tmp = three_DHT_For_Lookups[position];
    three_DHT_For_Lookups_Mutex[position].unlock();


    return tmp;
}

three_DHT DHT_Lookup::Access_Three_DHT_And_Lock(int position)
{
    if((position >= Lookup_Length) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }


    three_DHT_For_Lookups_Mutex[position].lock();
    three_DHT tmp = three_DHT_For_Lookups[position];

    return tmp;
}

void DHT_Lookup::Unlock(int position)
{

    three_DHT_For_Lookups_Mutex[position].unlock();
}



void DHT_Lookup::Write_To_Three_DHT(three_DHT write, int position)
{
    if((position >= Lookup_Length) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    three_DHT_For_Lookups_Mutex[position].lock();
    three_DHT_For_Lookups[position] = write;
    three_DHT_For_Lookups_Mutex[position].unlock();
}


void DHT_Lookup::Write_To_Locked_Three_DHT(three_DHT write, int position)
{
    if((position >= Lookup_Length) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    three_DHT_For_Lookups[position] = write;
}
