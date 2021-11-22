
#include "DHT_Access.h"
#include <iostream>


using namespace paft;

DHT_Single_Entry* DHT_Access::all_Peers = new DHT_Single_Entry[160*20];
DHT_Single_Entry* DHT_Access::all_Files = new DHT_Single_Entry[100]; //Temporarily as 100, might need to change later

_160bitnumber* DHT_Access::own_ID = new _160bitnumber;
unsigned short int DHT_Access::own_Port = 1234; //Default value- should be set before accessed

std::mutex* DHT_Access::all_Peers_Mutex = new std::mutex[160*20];
std::mutex* DHT_Access::all_Files_Mutex = new std::mutex[100];




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



DHT_Single_Entry DHT_Access::Access_FileIds(int position)
{
    if((position >= 100) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    all_Files_Mutex[position].lock();
    DHT_Single_Entry tmp = all_Files[position];
    all_Files_Mutex[position].unlock();

    return tmp;
}

void DHT_Access::Write_To_FileIds(DHT_Single_Entry write, int position)
{
    if((position >= 100) | (position<0))
    {
        std::cout << "Position " << position << " is not valid!!!\n";
        throw std::invalid_argument( "Position is not valid" );
    }

    all_Files_Mutex[position].lock();
    all_Files[position] = write;
    all_Files_Mutex[position].unlock();
}

void DHT_Access::Store_FileId(DHT_Single_Entry entry)
{
    //This stores the FileId if there is less then 3 entries already for that ID
    //And does not write anything otherwise-or if its full

    //DOES NOT WORK TODO
    printf("Storing a file ID... DOES NOT CURRENTLY WORK\n\n");
    int i=0;
    int counter=0;
    int first_pos=-1;
    while((i<100) & (counter<3))
    {
        DHT_Single_Entry tmp = entry;
        if(tmp.is_set)
        {
            if(DHT::IsEqual(tmp.id, entry.id))
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

_160bitnumber DHT_Access::Get_SELF()
{
    return *own_ID;

}

void DHT_Access::Set_Self(_160bitnumber own_Id)
{
    own_ID = &own_Id;
}



void DHT_Access::Set_Self_Port(unsigned short int port)
{
    own_Port = port;

}
unsigned short int DHT_Access::Get_Self_Port()
{
    return own_Port;

}






