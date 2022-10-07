
#include "Main_CLI.h"
#include "Main_Client.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "../FILEIO/Meta_Files.h"
#include "Major_Functions.h"
#include "../UnitTests/MainUnitTests.h"
#include "CLI_Functions.h"
#include "../FILEIO/File_Functions.h"
#include "../DHT/Print_DHT.h"

#include <iostream>
#include <random>

#define INPUT_BUFFER_LENGTH 1000


using namespace paft;

int Main_CLI::Start_Client()
{
    std::cout << "Type help for a list of commands\n";


    char command[INPUT_BUFFER_LENGTH];
    for(;;)
    {
        std::cin.getline(command, INPUT_BUFFER_LENGTH);
        std::cin.clear();

        int i;
        for(i=0;i<INPUT_BUFFER_LENGTH;i++)
            if(command[i] == '\0')
                break;
        Main_CLI::Command_Parser(command, i);

    }
}

int Main_CLI::Command_Parser(char Input[], int Input_len)
{
    if(String_Compare(Input, "help"))
        CLI_Functions::Help_Command();

    else if(String_Compare(Input, "testing_help"))
        CLI_Functions::Testing_Help_Command();

    else if(String_Compare(Input, "self_ping"))
        CLI_Functions::Self_Ping_Command();

    else if(String_Compare(Input, "ping"))
        CLI_Functions::Ping(Input, Input_len);

    else if(String_Compare(Input, "print_dht"))
        Print_DHT::Print_Main_DHT();

    else if(String_Compare(Input, "print_local_files"))
        Print_DHT::Print_File_Locations();

    else if(String_Compare(Input, "test_dht"))
        CLI_Functions::Test_DHT_Command();

    else if(String_Compare(Input, "self_find_random_peer"))
        Self_Find_Random_Node();

    else if(String_Compare(Input, "self_find_random_file"))
        Self_Find_Random_File();

    else if(String_Compare(Input, "self_store_random_file"))
        Self_Store_Random_File();

    else if(String_Compare(Input, "print_files"))
        Print_DHT::Print_Files();

    else if(String_Compare(Input, "make_meta_file"))
        Meta_Files::Make_File("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", DHT::Random_ID());

    else if(String_Compare(Input, "self_find_random_node_network"))
        Self_Find_Random_Node_Network();

    else if(String_Compare(Input, "store_file_on_network"))
        CLI_Functions::Upload_File_Network(Input, Input_len);

    else if(String_Compare(Input, "download_file_on_network"))
        CLI_Functions::Downlaod_File_Network(Input, Input_len);

    else if(String_Compare(Input, "self_download_file_on_network"))
        CLI_Functions::Self_Downlaod_File_Network(Input, Input_len);

    else if(String_Compare(Input, "store_file_net_and_get_chunk_back"))
        CLI_Functions::Store_File_Net_And_Get_Chunk_Back_Command();

    else if(String_Compare(Input, "manual_tests"))
        MainUnitTests::Run_All_Manual_Tests();

    else if(String_Compare(Input, "print_self"))
        CLI_Functions::Print_Self_Command();

    else if(String_Compare(Input, "load_state"))
        File_Functions::Load_State();

    else if(String_Compare(Input, "save_state"))
        File_Functions::Save_State();

    else if(String_Compare(Input, "download_file_onion_1"))
        CLI_Functions::Download_File_Onion_1(Input, Input_len);

    else if(String_Compare(Input, "download_file_onion_2"))
        CLI_Functions::Download_File_Onion_2(Input, Input_len);

    else if(String_Compare(Input, "upload_file_onion"))
        CLI_Functions::Download_File_Onion_2(Input, Input_len);

    else if(String_Compare(Input, "exit"))
        exit(0);

    else
       std::cout << "Unknown Command\n";



    return 0;
}


bool Main_CLI::String_Compare(char* string1, const char* string2)
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


void Main_CLI::Self_Find_Random_Node()
{
    _160bitnumber Testing = DHT::Random_ID();




    Main_Client Client("127.0.0.1", "1234");
    Client.Find_Node(Testing);




}

void Main_CLI::Self_Find_Random_Node_Network()
{
    _160bitnumber Testing = DHT::Random_ID();


    Major_Functions::Three_Closest_Peers_In_Network(Testing);



}

void Main_CLI::Self_Find_Random_File()
{

    _160bitnumber Testing = DHT::Random_ID();


    Main_Client Client("127.0.0.1", "1234");

    Client.Find_File(Testing);



}

void Main_CLI::Self_Store_Random_File()
{

    _160bitnumber Testing = DHT::Random_ID();

    DHT_Single_Entry file;

    file = DHT_Access::Access_DHT(159*20);
    file.id = Testing;


    Main_Client Client("127.0.0.1", "1234");

    Client.Store_File(file);



}




