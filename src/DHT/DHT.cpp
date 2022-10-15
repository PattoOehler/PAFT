#include "DHT.h"
#include "DHT_Access.h"
#include "DHT_Updater.h"
#include "DHT_Search.h"

#include <random>
#include <iostream>
#include <time.h>
#include <bits/stdc++.h>
#include <mutex>
#include <ws2tcpip.h>
#include <thread>
#include <string>

#include <Wincrypt.h>
#include <windows.h>
//#pragma comment(lib, "advapi32.lib")

using namespace paft;


std::mt19937_64 DHT::gen(time(nullptr));






_160bitnumber DHT::Random_ID()
{
    _160bitnumber randomID;

    HCRYPTPROV hProvider = 0;

	if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    {
        std::cout << "MASSIVE ERROR IN DHT::RANDOM_ID\n\n\n";
        return randomID;
    }


	if (!::CryptGenRandom(hProvider, 20, (BYTE *)&randomID))
	{
		::CryptReleaseContext(hProvider, 0);
		std::cout << "MASSIVE ERROR IN DHT::RANDOM_ID\n\n\n";
		return randomID;
	}



	if (!::CryptReleaseContext(hProvider, 0))
    {
        std::cout << "MASSIVE ERROR IN DHT::RANDOM_ID\n\n\n";
        return randomID;
    }


    return randomID;

    /* OLD -- DELETE LATER BUT WORKING FUNCTION

    _160bitnumber random_ID;

    //Produces pseudo-random Numbers based upon startup time.
    //TODO make random numbers cryptographically secure
    random_ID.top = gen();
    random_ID.mid = gen();
    random_ID.bot = gen() >> 32;

    return random_ID;
    */
}

bool DHT::Is_Equal(_160bitnumber id,_160bitnumber id2)
{
    if( (id.top == id2.top) && (id.mid == id2.mid) && (id.bot == id2.bot) )
        return true;

    return false;
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


int DHT::Distance(_160bitnumber id, _160bitnumber id2)
{
    unsigned long long int top_distance = id.top ^ id2.top;
    unsigned long long int mid_distance = id.mid ^ id2.mid;
    unsigned long int bot_distance = id.bot ^ id2.bot;


    if(top_distance != 0)
        return Log_Base_2(top_distance);

    else
    {
        if(mid_distance != 0)
            return 64+Log_Base_2(mid_distance);

        else
        {
            if(bot_distance != 0)
                return 128+Log_Base_2((unsigned long long int)bot_distance);
            else
                return  159;
        }
    }


}


void DHT::Update_Time(DHT_Single_Entry Update)
{
    if( Is_Equal(Update.id, DHT_Access::Get_Self_ID() ) )
       return;

    int distance = Distance(Update.id, DHT_Access::Get_Self_ID());

    for(int i=0; i<20; i++)
    {
        DHT_Single_Entry tmp = DHT_Access::Access_DHT(distance*20+i);

        if(tmp.is_set == true)
        {

            if(Is_Equal(tmp.id, Update.id))
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

    _160bitnumber ownID = DHT::Random_ID();

    std::cout << "SELF: " << DHT::ID_To_String(ownID) << "\n";

    DHT_Single_Entry self = DHT_Access::Get_Self();
    self.id = ownID;
    self.is_set = true;
    DHT_Access::Set_Self(self);


    return;

}



int DHT::Add_Entry(DHT_Single_Entry Entry)
{

    int distance = DHT::Distance(Entry.id, DHT_Access::Get_Self_ID());


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



std::string DHT::ID_To_String(_160bitnumber id)
{
    std::string returningID;

    unsigned char *pointer =  (unsigned char *) &id;
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
            returningID.push_back(currentLetter);
        }

    }
    return returningID;

}


std::string DHT::IP_To_String(in_addr addr_In)
{
    std::string returningID;

    unsigned char *pointer =  (unsigned char *) &addr_In;
    for(int i=0; i<4; i++)
    {
        int counter =0;
        while(pointer[i] != 0)
        {
            pointer[i] --;
            counter++;
        }

        returningID += std::to_string(counter);

        if(i!=3)
            returningID.push_back('.');

    }
    return returningID;

}





int DHT::Add_Entry_All_Buckets()
{

    DHT_Single_Entry Testing;


    DHT_Single_Entry a = DHT_Access::Get_Self();
    if(!a.is_set)
    {
        std::cout << "DHT_Access::Get_Self() IS NOT SET DHT::Add_Entry_All_Buckets cannot run!\n\n";
        return -1;
    }

    Testing.addr = a.addr;
    Testing.port = a.port;


    long long unsigned int tmp_top;
    long long unsigned int tmp_mid;
    long unsigned int tmp_bot;


    _160bitnumber SELF = DHT_Access::Get_Self_ID();
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





int DHT::Log_Base_2(unsigned long long int n)
{
    if (n == 0)
        return 0;


    int logValue = -1;
    while (n!=0)
    {
        logValue++;
        n >>= 1;
    }
    return logValue;

}


