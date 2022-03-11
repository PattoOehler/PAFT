
#include "MainCLI.h"
#include "Main_Client.h"
#include "../DHT/DHT.h"
#include "../DHT/DHT_Access.h"
#include "../FILEIO/Meta_Files.h"
#include "Major_Functions.h"

#include <iostream>
#include <random>

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


        MainCli::Command_Parser(buffer, INPUT_BUFFER_LENGTH);

    }
}

int MainCli::Command_Parser(char Input[], int Input_len)
{


    if(String_Compare(Input, "help"))
    {
        std::cout << "\n\n\n\nHelpmessage\n\n";
        std::cout << "help                  -- Receive this message\n";
        std::cout << "self_get_file         --  \n";
        std::cout << "self_ping             -- Ping self \n";
        std::cout << "print_dht             -- Print the dht\n";
        std::cout << "exit                  -- Exit the program\n";
        std::cout << "test_dht              -- pings the DHT at position 159*20\n";
        std::cout << "self_find_random_peer -- finds the closest 3 peers to a random id in own DHT\n";
        std::cout << "self_find_random_file -- finds the closest 3 peers/files to a random id in own DHT\n";
        std::cout << "self_store_random_file-- sends store file RPC to self\n";
        std::cout << "print_files           -- prints the stored files\n";
        std::cout << "make_meta_file        -- makes a metadata file\n";
        std::cout << "self_find_random_node_network  -- Testing\n";
        std::cout << "store_file_on_network -- stores a file on the network\n";
        std::cout << "store_file_net_and_get_meta_back -- For Testing\n";
        std::cout << "store_file_net_and_get_chunk_back -- For Testing\n";
        std::cout << "\n\n";
        return 0;
    }




    else if(String_Compare(Input, "self_get_file"))
    {

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
        DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
        if(a.is_set)
        {
            DHT::Test_Add_Entry();
        }
        else
            printf("The DHT at position 159*20 is not set\n");


        return 0;
    }

    else if(String_Compare(Input, "self_find_random_peer"))
    {
        Self_Find_Random_Node();

        return 0;
    }

    else if(String_Compare(Input, "self_find_random_file"))
    {
        Self_Find_Random_File();


        return 0;
    }
    else if(String_Compare(Input, "self_store_random_file"))
    {
        Self_Store_Random_File();


        return 0;
    }
    else if(String_Compare(Input, "print_files"))
    {
        DHT::Print_Files();
        return 0;


    }
    else if(String_Compare(Input, "make_meta_file"))
    {

        Meta_Files::Make_File("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", DHT::Random_ID());


        return 0;

    }
    else if(String_Compare(Input, "self_find_random_node_network"))
    {
        Self_Find_Random_Node_Network();



        return 0;

    }

    else if(String_Compare(Input, "store_file_on_network"))
    {
        Major_Functions::Upload_File_To_Network("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft");


        return 0;

    }
    else if(String_Compare(Input, "store_file_net_and_get_meta_back"))
    {
        DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
        if(!a.is_set)
        {
            printf("The DHT at position 159*20 is not set\n");
            return 0;
        }

        _160bitnumber ID = DHT::Random_ID();
        Major_Functions::Upload_File_To_Network("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", ID);


        std::cout << "Made it to getMetaDataFile\n";

        Major_Functions::getMetaDataFile(ID, "asdf", a);


        std::cout << "Done!!!\n";

        return 0;

    }
    else if(String_Compare(Input, "store_file_net_and_get_chunk_back"))
    {
        DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
        if(!a.is_set)
        {
            printf("The DHT at position 159*20 is not set\n");
            return 0;
        }

        _160bitnumber ID = DHT::Random_ID();
        Major_Functions::Upload_File_To_Network("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", ID);



        Major_Functions::getFileChunk(ID, "ExpectedChecksum", a,1);


        std::cout << "Done!!!\n";

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



    //return false;
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




