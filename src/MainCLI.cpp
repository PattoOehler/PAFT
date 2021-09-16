
#include "../include/MainCLI.h"
#include "../include/Main_Client.h"

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


int MainCli::Command_Parser(char Input[], int Input_len)
{
    if(Input_len < 10)
    {
        std::cout << "Input_Len too short to operate MainClient::Command_Parser";
        return 1;
    }

    if(Input[0] == 'h')
    {
        if(Input[1] == 'e')
        {
            if(Input[2] == 'l')
            {
                if(Input[3] == 'p')
                {
                    if(Input[4]  == '\n' || Input[4]  == '\0' || Input[4]  == ' ')
                    {
                            std::cout << "Helpmessage\n";
                            std::cout << "self\n";
                            return 0;
                    }

                }
            }
        }
    }

    else if(Input[0] == 's')
    {
        if(Input[1] == 'e')
        {
            if(Input[2] == 'l')
            {
                if(Input[3] == 'f')
                {
                    if(Input[4]  == '\n' || Input[4]  == '\0' || Input[4]  == ' ')
                    {
                            MainClient Client("127.0.0.1", "1234");
                            //Client.Connect("127.0.0.1", "1234");
                            Client.GetFile("asdf");
                            return 0;
                    }

                }
            }
        }
    }
    else if(Input[0] == 'e')
    {
        if(Input[1] == 'x')
        {
            if(Input[2] == 'i')
            {
                if(Input[3] == 't')
                {
                    if(Input[4]  == '\n' || Input[4]  == '\0' || Input[4]  == ' ')
                    {
                            exit(0);
                            return 0;
                    }

                }
            }
        }
    }


    std::cout << "Unknown Command\n";
    return 1;
}
