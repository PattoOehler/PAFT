
#include "CLI_Functions.h"
#include "Main_Client.h"
#include "../DHT/DHT_Access.h"
#include "Major_Functions.h"

#include <iostream>

using namespace paft;

void CLI_Functions::Help_Command()
{
    std::cout << "\n\n\n\nHelpmessage\n\n";
    std::cout << "help                  -- Receive this message\n";
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
    std::cout << "manual_tests          -- Runs all of the manual tests\n";
    std::cout << "print_self            -- For Testing\n";
    std::cout << "\n\n";

}

void CLI_Functions::Self_Ping_Command()
{
    MainClient Client("127.0.0.1", "1234");
    Client.Ping();

}


void CLI_Functions::Test_DHT_Command()
{
    DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
    if(a.is_set)
    {
        DHT::Add_Entry_All_Buckets();
    }
    else
    {
        std::cout << "The DHT at position 159*20 is not set\n";
    }
}

void CLI_Functions::Store_File_Net_And_Get_Meta_Back_Command()
{

    DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
    if(!a.is_set)
    {
        printf("The DHT at position 159*20 is not set\n");
        return;
    }

    _160bitnumber ID = DHT::Random_ID();
    Major_Functions::Upload_File_To_Network("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", ID);



    Major_Functions::getMetaDataFile(ID, "asdf", a);


    std::cout << "Done!!!\n";

}



void CLI_Functions::Store_File_Net_And_Get_Chunk_Back_Command()
{

    DHT_Single_Entry a = DHT_Access::Access_DHT(159*20);
    if(!a.is_set)
    {
        printf("The DHT at position 159*20 is not set\n");
        return;
    }

    _160bitnumber ID = DHT::Random_ID();
    Major_Functions::Upload_File_To_Network("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft", ID);



    Major_Functions::getFileChunk(ID, "ExpectedChecksum", a,1);


    std::cout << "Done!!!\n";

}

void CLI_Functions::Print_Self_Command()
{
    _160bitnumber self = DHT_Access::Get_SELF();
    std::cout << "SELF: ";
    DHT::Print_ID(self);
    std::cout << "\n";
}




























