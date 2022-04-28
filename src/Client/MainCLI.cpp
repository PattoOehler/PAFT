
#include "MainCLI.h"
#include "Main_Client.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "../FILEIO/Meta_Files.h"
#include "Major_Functions.h"
#include "../UnitTests/MainUnitTests.h"
#include "CLI_Functions.h"

#include <iostream>
#include <random>

#define INPUT_BUFFER_LENGTH 100


using namespace paft;

int MainCli::Start_Client()
{
    std::cout << "Type help for a list of commands\n";


    char command[INPUT_BUFFER_LENGTH];
    for(;;)
    {
        std::cin.getline(command, INPUT_BUFFER_LENGTH);
        std::cin.clear();


        MainCli::Command_Parser(command, INPUT_BUFFER_LENGTH);

    }
}

int MainCli::Command_Parser(char Input[], int Input_len)
{
    if(String_Compare(Input, "help"))
        CLI_Functions::Help_Command();

    else if(String_Compare(Input, "self_ping"))
        CLI_Functions::Self_Ping_Command();

    else if(String_Compare(Input, "print_dht"))
        DHT::Print_DHT();

    else if(String_Compare(Input, "test_dht"))
        CLI_Functions::Test_DHT_Command();

    else if(String_Compare(Input, "self_find_random_peer"))
        Self_Find_Random_Node();

    else if(String_Compare(Input, "self_find_random_file"))
        Self_Find_Random_File();

    else if(String_Compare(Input, "self_store_random_file"))
        Self_Store_Random_File();

    else if(String_Compare(Input, "print_files"))
        DHT::Print_Files();

    else if(String_Compare(Input, "make_meta_file"))
        Meta_Files::Make_File("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", DHT::Random_ID());

    else if(String_Compare(Input, "self_find_random_node_network"))
        Self_Find_Random_Node_Network();

    else if(String_Compare(Input, "store_file_on_network"))
        CLI_Functions::Upload_File_Network(Input, Input_len);

    else if(String_Compare(Input, "store_file_net_and_get_meta_back"))
        CLI_Functions::Store_File_Net_And_Get_Meta_Back_Command();

    else if(String_Compare(Input, "store_file_net_and_get_chunk_back"))
        CLI_Functions::Store_File_Net_And_Get_Chunk_Back_Command();

    else if(String_Compare(Input, "manual_tests"))
        MainUnitTests::Run_All_Manual_Tests();

    else if(String_Compare(Input, "print_self"))
        CLI_Functions::Print_Self_Command();

    else if(String_Compare(Input, "exit"))
        exit(0);

    else
       std::cout << "Unknown Command\n";



    return 0;
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
        if(string2[i]  == '\n' || string2[i]  == '\0' || string2[i]  == ' ')
        {
            return false;

        }
        if(string1[i] == string2[i])
            continue;
        else
            return false;

    }
}


void MainCli::Self_Find_Random_Node()
{
    _160bitnumber Testing = DHT::Random_ID();




    MainClient Client("127.0.0.1", "1234");
    Client.Find_Node(Testing);




}

void MainCli::Self_Find_Random_Node_Network()
{
    _160bitnumber Testing = DHT::Random_ID();


    Major_Functions::Three_Closest_Peers_In_Network(Testing);



}

void MainCli::Self_Find_Random_File()
{

    _160bitnumber Testing = DHT::Random_ID();


    MainClient Client("127.0.0.1", "1234");

    Client.Find_File(Testing);



}

void MainCli::Self_Store_Random_File()
{

    _160bitnumber Testing = DHT::Random_ID();

    DHT_Single_Entry file;

    file = DHT_Access::Access_DHT(159*20);
    file.id = Testing;


    MainClient Client("127.0.0.1", "1234");

    Client.Store_File(file);



}




