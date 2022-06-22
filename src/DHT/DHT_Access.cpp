
#include "DHT_Access.h"
#include <iostream>

#define DHT_File_Len 100

using namespace paft;

DHT_Single_Entry* DHT_Access::all_Peers = new DHT_Single_Entry[160*20];
DHT_Single_Entry* DHT_Access::all_Files = new DHT_Single_Entry[DHT_File_Len]; //Temporarily as 100, might need to change later
std::string *DHT_Access::local_File_Locations = new std::string[DHT_File_Len];


DHT_Single_Entry DHT_Access::self;

std::mutex DHT_Access::own_ID_Mutex;
std::mutex* DHT_Access::all_Peers_Mutex = new std::mutex[160*20];
std::mutex* DHT_Access::all_Files_Mutex = new std::mutex[DHT_File_Len];







std::string DHT_Access::Get_Local_File_Location(int position)
{
    return local_File_Locations[position];

}

void DHT_Access::Set_Local_File_Location(std::string local_File_Location, int position)
{
    local_File_Locations[position] = local_File_Location;

}


int DHT_Access::Find_Stored_File(_160bitnumber entry)
{
    for(int i=0; i<DHT_File_Len; i++)
    {
        DHT_Single_Entry a = Access_File_IDs(i);
        if(a.is_set)
        {
            if(DHT::Is_Equal(a.id, entry))
            {


              std::string localFileLocation = Get_Local_File_Location(i);
              if(!localFileLocation.empty())
                return i;

            }

        }

    }

    return -1;

}


DHT_Single_Entry DHT_Access::Access_DHT(int position)
{
    if((position >= 160*20) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    all_Peers_Mutex[position].lock();
    DHT_Single_Entry tmp = all_Peers[position];
    all_Peers_Mutex[position].unlock();

    return tmp;
}

void DHT_Access::Write_To_DHT(DHT_Single_Entry write, int position)
{
    if((position >= 160*20) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    all_Peers_Mutex[position].lock();
    all_Peers[position] = write;
    all_Peers_Mutex[position].unlock();
}



DHT_Single_Entry DHT_Access::Access_File_IDs(int position)
{
    if((position >= DHT_File_Len) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    all_Files_Mutex[position].lock();
    DHT_Single_Entry tmp = all_Files[position];
    all_Files_Mutex[position].unlock();

    return tmp;
}

void DHT_Access::Write_To_File_IDs(DHT_Single_Entry write, int position)
{
    if((position >= DHT_File_Len) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    all_Files_Mutex[position].lock();
    all_Files[position] = write;
    all_Files_Mutex[position].unlock();
}

int DHT_Access::Store_File_ID(DHT_Single_Entry entry)
{

    printf("Storing a file ID...\n\n");

    for(int i=0;i<DHT_File_Len;i++)
    {
        DHT_Single_Entry tmp_File = Access_File_IDs(i);
        if(!tmp_File.is_set)
        {
            entry.is_set = true;
            entry.time_To_Timeout = time(0)+60*60; // 1 hour
            Write_To_File_IDs(entry, i);
            return i;
        }

    }
    return -1;

}





_160bitnumber DHT_Access::Get_Self_ID()
{
    own_ID_Mutex.lock();
    _160bitnumber returning = self.id;
    own_ID_Mutex.unlock();
    return returning;

}

void DHT_Access::Set_Self_ID(_160bitnumber id)
{
    own_ID_Mutex.lock();
    self.id = id;
    own_ID_Mutex.unlock();


}





DHT_Single_Entry DHT_Access::Get_Self()
{
    own_ID_Mutex.lock();
    DHT_Single_Entry returning = self;
    own_ID_Mutex.unlock();
    return returning;

}

void DHT_Access::Set_Self(DHT_Single_Entry entry)
{
    own_ID_Mutex.lock();
    self = entry;
    own_ID_Mutex.unlock();
}



unsigned short int DHT_Access::Get_Self_Port()
{
    own_ID_Mutex.lock();
    unsigned short int own_Port = self.port;
    own_ID_Mutex.unlock();
    return own_Port;

}

void DHT_Access::Set_Self_Port(unsigned short int port)
{
    own_ID_Mutex.lock();
    self.port = port;
    own_ID_Mutex.unlock();

}

void DHT_Access::Set_Self_Address(const char *ip)
{
    unsigned long addr = inet_addr(ip);

    in_addr tmp;
    memcpy((char *)&tmp, (char *)&addr, 4);

    if(addr != INADDR_NONE)
    {
        own_ID_Mutex.lock();
        self.addr = tmp;
        own_ID_Mutex.unlock();
    }
    else
    {
        std::cout << "ERROR SETTING SELF ADDRESS IN DHT_Access::Set_Self_Address\n";

    }
}





