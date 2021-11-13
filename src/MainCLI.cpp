
#include "../include/MainCLI.h"
#include "../include/Main_Client.h"
#include "../include/DHT.h"

#include <iostream>

#define INPUT_BUFFER_LENGTH 100


using namespace paft;

int MainCli::Start_Client()
{
    std::cout << "Type help for a list of commands\n";


    char buffer[INPUT_BUFFER_LENGTH];
    for(;;)
    {
        std::cin.getline(buffer, INPUT_BUFFER_LENGTH);
        std::cin.clear();

        //std::cout << buffer ;
        MainCli::Command_Parser(buffer, INPUT_BUFFER_LENGTH);

    }

    //Should Not get to here
    exit(1);
}

bool MainCli::String_Compare(char* string1, const char* string2)
{
    for(int i = 0;;i++)
    {

        if(string1[i]  == '\n' || string1[i]  == '\0' || string1[i]  == ' ')
        {
            if(string2[i]  == '\n' || string2[i]  == '\0' || string2[i]  == ' ')
            {
                return true;

            }
            else
            {
                return false;

            }

        }
        if(string1[i] == string2[i])
            continue;
        else
            return false;




    }



    //return false;
}



int MainCli::Command_Parser(char Input[], int Input_len)
{
    if(Input_len < 10)
    {
        std::cout << "Input_Len too short to operate MainClient::Command_Parser";
        return 1;
    }


    if(String_Compare(Input, "help"))
    {
        std::cout << "Helpmessage\n\n";
        std::cout << "help          -- Receive this message\n";
        std::cout << "self_get_file -- Get a file from 127.0.0.1 \n";
        std::cout << "self_ping     -- Ping self \n";
        std::cout << "print_dht     -- Print the dht\n";
        std::cout << "exit          -- Exit the program\n";
        std::cout << "test_dht      -- pings the DHT at position 159*20\n";
        std::cout << "self_find_peer-- finds the closest 3 peers to a random id in own DHT\n";
        return 0;
    }




    else if(String_Compare(Input, "self_get_file"))
    {
        MainClient Client("127.0.0.1", "1234");
        Client.GetFile("asdf");
        return 0;

    }

    else if(String_Compare(Input, "self_ping"))
    {
        MainClient Client("127.0.0.1", "1234");
        Client.Ping();
        return 0;

    }
    else if(String_Compare(Input, "print_dht"))
    {
        DHT::Print_DHT();
        return 0;

    }
    else if(String_Compare(Input, "test_dht"))
    {
        DHT_Single_Entry a = DHT::Access_DHT(159*20);
        if(a.is_set)
        {   MainClient Client(a.addr, a.port);
            Client.Ping();
        }
        else
            printf("The DHT at position 159*20 is not set\n");


        return 0;
    }

    else if(String_Compare(Input, "self_find_peer"))
    {
        printf("Not implemented yet");

        return 0;
    }

    else if(String_Compare(Input, "exit"))
    {
        exit(0);
        return 0;
    }


    std::cout << "Unknown Command\n";
    return 1;
}
