
#include "CLI_Functions.h"
#include "Main_Client.h"
#include "../DHT/DHT_Access.h"
#include "Major_Functions.h"

#include <iostream>

using namespace paft;

void CLI_Functions::Help_Command()
{
    std::cout << "\n\n\n\nHelpmessage\n\n";
    std::cout << "help                                                -- Receive this message\n";
    std::cout << "self_ping                                           -- Ping self \n";
    std::cout << "print_dht                                           -- Print the dht\n";
    std::cout << "test_dht                                            -- pings the DHT at position 159*20\n";
    std::cout << "self_find_random_peer                               -- finds the closest 3 peers to a random id in own DHT\n";
    std::cout << "self_find_random_file                               -- finds the closest 3 peers/files to a random id in own DHT\n";
    std::cout << "self_store_random_file                              -- sends store file RPC to self\n";
    std::cout << "print_files                                         -- prints the stored files\n";
    std::cout << "make_meta_file                                      -- makes a metadata file\n";
    std::cout << "self_find_random_node_network                       -- Recursively find closest 3 peers in network\n";
    std::cout << "store_file_on_network [C:\\File_Location] [Filename]-- stores a file on the network\n";
    std::cout << "download_file_on_network [SHA-CHECKSUM] [FileID]    -- downloads a file from the network\n";
    std::cout << "store_file_net_and_get_meta_back                    -- For Testing\n";
    std::cout << "store_file_net_and_get_chunk_back                   -- For Testing\n";
    std::cout << "manual_tests                                        -- Runs all of the manual tests\n";
    std::cout << "print_self                                          -- For Testing\n";
    std::cout << "exit                                                -- Exit the program\n";
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
    _160bitnumber self = DHT_Access::Get_SELF_ID();
    std::cout << "SELF: ";
    DHT::Print_ID(self);
    std::cout << "\n";
}




void CLI_Functions::Upload_File_Network(char input[], int length)
{
    char file_Location[150];
    char file_Name[80];

    int file_Location_Counter=0;
    int file_Name_Counter=0;

    file_Location[0] = '\0';
    file_Name[0] = '\0';

    bool is_Command=true;
    bool is_File_Location=true;
    bool is_File_Name=true;

    for(int i=0; i<length; i++)
    {
        if(is_Command)
        {
            if(input[i] == ' ')
                is_Command=false;
        }
        else
        {
            if(is_File_Location)
            {
                if(input[i] != ' ')
                {
                    file_Location[file_Location_Counter] = input[i];
                    file_Location_Counter++;
                }
                else
                {
                    is_File_Location=false;
                }
            }
            else
            {
                if(is_File_Name)
                {
                    if((input[i] != ' ') | (input[i] == '\0') | (input[i] == '\n'))
                    {
                        file_Name[file_Name_Counter] = input[i];
                        file_Name_Counter++;
                    }
                    else
                    {
                        is_File_Name=false;
                    }
                }
                else
                {
                    break;
                }
            }

        }

    }


    if(file_Location[0] == '\0')
    {
        std::cout << "file_Location[0] == \\0 running default values";
        Major_Functions::Upload_File_To_Network("F:\\Ubuntu\\ISOs\\MAC\\snowlepard.dmg", "ISO.paft");
    }
    else
    {
        Major_Functions::Upload_File_To_Network(file_Location, file_Name );

    }

}


void CLI_Functions::Downlaod_File_Network(char input[], int length)
{
    char checksum[150];
    char fileID[80];

    int checksum_Counter=0;
    int fileID_Counter=0;

    checksum[0] = '\0';
    fileID[0] = '\0';

    bool is_Command=true;
    bool is_File_Location=true;
    bool is_File_Name=true;

    for(int i=0; i<length; i++)
    {
        if(is_Command)
        {
            if(input[i] == ' ')
                is_Command=false;
        }
        else
        {
            if(is_File_Location)
            {
                if(input[i] != ' ')
                {
                    checksum[checksum_Counter] = input[i];
                    checksum_Counter++;
                }
                else
                {
                    is_File_Location=false;
                }
            }
            else
            {
                if(is_File_Name)
                {
                    if((input[i] != ' ') | (input[i] == '\0') | (input[i] == '\n'))
                    {
                        fileID[fileID_Counter] = input[i];
                        fileID_Counter++;
                    }
                    else
                    {
                        is_File_Name=false;
                    }
                }
                else
                {
                    break;
                }
            }

        }

    }


    if(checksum[0] == '\0')
    {
        std::cout << "Error no parameters given\n";
    }
    else
    {

        //Major_Functions::Find_File_On_Network( fileID );

        //Needs to download after this

    }

}















