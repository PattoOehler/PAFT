
#include "CLI_Functions.h"
#include "Main_Client.h"
#include "../DHT/DHT_Access.h"
#include "Major_Functions.h"
#include "../DHT/DHT.h"
#include "../FileIO/File_Functions.h"
#include "../FileIO/Meta_Files.h"
#include "../DHT/DHT_Search.h"

#include <iostream>

using namespace paft;

void CLI_Functions::Help_Command()
{
    std::cout << "\n\n\n\nHelpmessage\n\n";
    std::cout << "help                                                -- Receive this message\n";
    std::cout << "self_ping                                           -- Ping self \n";
    std::cout << "ping [ip] [port]                                    -- Ping\n";
    std::cout << "print_dht                                           -- Print the dht\n";
    std::cout << "print_files                                         -- prints the stored files\n";
    std::cout << "store_file_on_network [C:\\File_Location] [Filename] -- stores a file on the network\n";
    std::cout << "download_file_on_network [SHA-CHECKSUM] [FileID]    -- downloads a file from the network\n";
    std::cout << "save_state                                          -- Saves the DHT\n";
    std::cout << "load_state                                          -- Load in the DHT from the previous save\n";
    std::cout << "exit                                                -- Exit the program\n\n";
    std::cout << "testing_help                                        -- Show advanced/testing commands ";
    std::cout << "\n\n";

}


void CLI_Functions::Testing_Help_Command()
{
    std::cout << "\n\n";
    std::cout << "self_download_file_on_network [SHA-CHECKSUM] [FileID]-- downloads a file from yourself\n";
    std::cout << "store_file_net_and_get_chunk_back                    -- For Testing\n";
    std::cout << "manual_tests                                         -- Runs all of the manual tests\n";
    std::cout << "print_self                                           -- For Testing\n";
    std::cout << "make_meta_file                                       -- makes a metadata file\n";
    std::cout << "self_find_random_node_network                        -- Recursively find closest 3 peers in network\n";
    std::cout << "self_find_random_peer                                -- finds the closest 3 peers to a random id in own DHT\n";
    std::cout << "self_find_random_file                                -- finds the closest 3 peers/files to a random id in own DHT\n";
    std::cout << "self_store_random_file                               -- sends store file RPC to self\n";
    std::cout << "test_dht                                             -- puts SELF in each bucket of the DHT\n";
    std::cout << "print_local_files                                    -- Prints all the local files\n";
    std::cout << "download_file_onion_1 [SHA-CHECKSUM] [FileID]        -- Downloads a file using another peer as a proxy\n";
    std::cout << "download_file_onion_2 [SHA-CHECKSUM] [FileID]        -- Downloads a file using another 2 peers as a proxy\n";
    std::cout << "\n";
}


void CLI_Functions::Download_File_Onion_1(char input[], int length)
{
    int *positions;
    positions = Arguement_Positions( input, length);
    if(*(positions+0) == 0 || *(positions+1) == 0)
    {
        std::cout << "Error: not enough parameters\n";
        return;
    }
    //(input + *(positions+0), input + *(positions+1) ) how to call the arguments
    _160bitnumber fileID = Char_To_160bit(input + *(positions+1));
    three_DHT net_Closest = Major_Functions::Find_File_On_Network(fileID);
    ChunkResponce b;
    three_DHT peers;
    if( DHT::Is_Equal( net_Closest.entry[0].id , fileID) )
    {
        b.chunkID = -1;
        b.fileID = fileID;
        b.sendToAddr = net_Closest.entry[0].addr; //Should work TODO test
        b.sendToPort = net_Closest.entry[0].port;

        peers = DHT_Search::Lookup(fileID);
        if( memcmp((char *)&peers.entry[0].addr, (char *)&b.sendToAddr, 4) == 0)
        {
            if(peers.entry[1].is_set)
                peers.entry[0] = peers.entry[1];
            else
            {
                std::cout << "Error in Download_File_Onion\n";
                return;
            }
        }
        int error_Check = Major_Functions::Get_Metadata_File_Proxy(input + *(positions+0), peers.entry[0], b);
        if(error_Check == 0)
            std::cout << "Got the metadata file\n";
        else
        {
            std::cout << "Metadata file had an unexpected error\n";
            return;
        }
    }
    else
    {
        std::cout << "Expected " << DHT::ID_To_String(fileID) << " got -> " << DHT::ID_To_String(net_Closest.entry[0].id) << "RETURNING\n\n";
        return;
    }


    std::string output_File_Name = Meta_Files::Get_Output_File_Name( fileID );
    //Allocate the amount of space necessary for the file
    File_Functions::Allocate_File( output_File_Name );


    //Get the file by chunks
    int fileChunks = Meta_Files::Get_Chunks( output_File_Name );
    for(int i=0; i<fileChunks; i++ )
    {
        std::string metaCheckSum = Meta_Files::Get_Check_Sum(i, output_File_Name);
        b.chunkID = i;
        Major_Functions::Get_File_Chunk_Proxy(metaCheckSum, peers.entry[0], b );
        std::cout << "Got FileChunk " << i << "\n";
    }

    std::cout << "Finished Downloading the file now uploading\n";
    Major_Functions::Upload_To_Network("Test_Metafiles/Downloaded_File", fileID);


}





void CLI_Functions::Download_File_Onion_2(char input[], int length)
{
    int *positions;
    positions = Arguement_Positions( input, length);
    if(*(positions+0) == 0 || *(positions+1) == 0)
    {
        std::cout << "Error: not enough parameters\n";
        return;
    }
    //(input + *(positions+0), input + *(positions+1) ) how to call the arguments
    std::cout << "Download_File_Onion 1\n";
    _160bitnumber fileID = Char_To_160bit(input + *(positions+1));
    three_DHT net_Closest = Major_Functions::Find_File_On_Network(fileID);
    ChunkResponce b;
    std::cout << "Download_File_Onion 2\n";
    three_DHT peers;
    if( DHT::Is_Equal( net_Closest.entry[0].id , fileID) )
    {
        std::cout << "Download_File_Onion 3\n";
        b.chunkID = -1;
        b.fileID = fileID;
        b.sendToAddr = net_Closest.entry[0].addr; //Should work TODO test
        b.sendToPort = net_Closest.entry[0].port;
        std::cout << "Download_File_Onion 4\n";

        peers = DHT_Search::Lookup(fileID);
        std::cout << "Download_File_Onion 5\n";
        if( memcmp((char *)&peers.entry[0].addr, (char *)&b.sendToAddr, 4) == 0)
        {
            std::cout << "Download_File_Onion 6-1\n";
            if(peers.entry[1].is_set)
                peers.entry[0] = peers.entry[1];
            else
            {
                std::cout << "Error 1 in Download_File_Onion 2\n";
                return;
            }
        }
        if( memcmp((char *)&peers.entry[1].addr, (char *)&b.sendToAddr, 4) == 0)
        {
            std::cout << "Download_File_Onion 6-2\n";
            if(peers.entry[2].is_set)
                peers.entry[1] = peers.entry[2];
            else
            {
                std::cout << "Error 2 in Download_File_Onion 2\n";
                return;
            }
        }
        std::cout << "Download File Onion 7\n";

        int error_Check = Major_Functions::Get_Metadata_File_Proxy_2(input + *(positions+0), peers.entry[0],peers.entry[1], b);
        std::cout << "Download File Onion 8\n";
        if(error_Check == 0)
            std::cout << "Got the metadata file\n";
        else
        {
            std::cout << "Metadata file had an unexpected error\n";
            return;
        }
    }
    else
    {
        std::cout << "Expected " << DHT::ID_To_String(fileID) << " got -> " << DHT::ID_To_String(net_Closest.entry[0].id) << "RETURNING\n\n";
        return;
    }

    /*
    std::string output_File_Name = Meta_Files::Get_Output_File_Name( fileID );
    //Allocate the amount of space necessary for the file
    File_Functions::Allocate_File( output_File_Name );


    //Get the file by chunks
    int fileChunks = Meta_Files::Get_Chunks( output_File_Name );
    for(int i=0; i<fileChunks; i++ )
    {
        std::string metaCheckSum = Meta_Files::Get_Check_Sum(i, output_File_Name);
        b.chunkID = i;
        Major_Functions::Get_File_Chunk_Proxy(metaCheckSum, peers.entry[0], b );
        std::cout << "Got FileChunk " << i << "\n";
    }

    std::cout << "Finished Downloading the file now uploading\n";
    Major_Functions::Upload_To_Network("Test_Metafiles/Downloaded_File", fileID);
    */

}











void CLI_Functions::Self_Ping_Command()
{
    Main_Client client("127.0.0.1", "1234");
    client.Ping();

}


void CLI_Functions::Test_DHT_Command()
{
    DHT_Single_Entry a = DHT_Access::Get_Self();
    if(a.is_set)
    {
        DHT::Add_Entry_All_Buckets();
    }
    else
    {
        std::cout << "DHT_Access::SELF is not set\n";
    }
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



    Major_Functions::Get_File_Chunk(ID, "ExpectedChecksum", a,1);


    std::cout << "Done!!!\n";

}

void CLI_Functions::Print_Self_Command()
{
    _160bitnumber self = DHT_Access::Get_Self_ID();
    std::cout << "SELF: " << DHT::ID_To_String(self) << "\n";
}




void CLI_Functions::Upload_File_Network(char input[], int length)
{

    int *positions;
    positions = Arguement_Positions( input, length);
    if(*(positions+0) == 0 || *(positions+1) == 0)
    {
        std::cout << "Error: not enough parameters\n";
        return;
    }

    Major_Functions::Upload_File_To_Network(input + *(positions+0), input + *(positions+1) );



}


void CLI_Functions::Downlaod_File_Network(char input[], int length)
{

    int *positions;
    positions = Arguement_Positions( input, length);
    if(*(positions+0) == 0 || *(positions+1) == 0)
    {
        std::cout << "Error: not enough parameters\n";
        return;
    }
    //(input + *(positions+0), input + *(positions+1) ) how to call the arguments

    _160bitnumber fileID = Char_To_160bit(input + *(positions+1));
    three_DHT net_Closest = Major_Functions::Find_File_On_Network(fileID);


    if( DHT::Is_Equal( net_Closest.entry[0].id , fileID) )
    {
        int error_Check = Major_Functions::Get_Metadata_File(fileID, input + *(positions+0), net_Closest.entry[0]);
        if(error_Check == 0)
            std::cout << "Got the metadata file\n";
        else
        {
            std::cout << "Metadata file had an unexpected error\n";
            return;
        }
    }
    else
    {
        std::cout << "Expected " << DHT::ID_To_String(fileID) << " got -> " << DHT::ID_To_String(net_Closest.entry[0].id) << "RETURNING\n\n";
        return;
    }


    std::string output_File_Name = Meta_Files::Get_Output_File_Name( fileID );
    //Allocate the amount of space necessary for the file
    File_Functions::Allocate_File( output_File_Name );


    //Get the file by chunks
    int fileChunks = Meta_Files::Get_Chunks( output_File_Name );
    for(int i=0; i<fileChunks; i++ )
    {
        std::string metaCheckSum = Meta_Files::Get_Check_Sum(i, output_File_Name);

        Major_Functions::Get_File_Chunk(fileID, metaCheckSum, net_Closest.entry[0], i );
    }

    std::cout << "Finished Downloading the file now uploading\n";
    Major_Functions::Upload_To_Network("Test_Metafiles/Downloaded_File", fileID);

}



void CLI_Functions::Self_Downlaod_File_Network(char input[], int length)
{

    int *positions;
    positions = Arguement_Positions( input, length);
    if(*(positions+0) == 0 || *(positions+1) == 0)
    {
        std::cout << "Error: not enough parameters\n";
        return;
    }
    //(input + *(positions+0), input + *(positions+1) ) how to call the arguemnts

    _160bitnumber file_ID = Char_To_160bit(input + *(positions+1));
    three_DHT net_Closest = Major_Functions::Find_File_On_Network(file_ID);


    if( DHT::Is_Equal( net_Closest.entry[0].id , file_ID) )
    {
        int error_Check = Major_Functions::Get_Metadata_File_Self(file_ID, input + *(positions+0), net_Closest.entry[0]);
        if(error_Check == 0)
            std::cout << "Got the metadata file\n";
        else
        {
            std::cout << "Metadata file had an unexpected error\n";
            return;
        }
    }
    else
    {
        std::cout << "Expected " << DHT::ID_To_String(file_ID) << " got -> " << DHT::ID_To_String(net_Closest.entry[0].id) << "RETURNING\n\n";
        return;
    }



    //Allocate the amount of space necessary for the file
    File_Functions::Allocate_File();


    //Get the file by chunks
    int fileChunks = Meta_Files::Get_Chunks("Test_Metafiles\\meta.paft");
    for(int i=0; i<fileChunks; i++ )
    {
        std::string metaCheckSum = Meta_Files::Get_Check_Sum(i, "Test_Metafiles\\meta.paft");

        Major_Functions::Get_File_Chunk(file_ID, metaCheckSum, net_Closest.entry[0], i );
    }

    std::cout << "Finished Downloading the file\n";

}






_160bitnumber CLI_Functions::Char_To_160bit(char *input)
{
    _160bitnumber returning;
    for(int i=0; i<40; i+= 2)
    {
        unsigned char bytenum=0;
        if(input[i] <= '9' && input[i] >= '0')
            bytenum += (input[i] - '0') * 16;
        else
            bytenum += ((input[i] - 'A')+10) * 16;

        if(input[i+1] <= '9' && input[i+1] >= '0')
            bytenum += (input[i+1] - '0');
        else
            bytenum += (input[i+1] - 'A') + 10;

        memcpy((char*)&returning+(i/2),(char*)&bytenum,1);



    }

    return returning;
}






void CLI_Functions::Ping(char input[], int length)
{
    int *positions;
    positions = Arguement_Positions( input, length);

    if(*(positions+0) == 0 || *(positions+1) == 0)
    {
        std::cout << "Error: not enough parameters\n";
        return;
    }

    Main_Client Client(input + *(positions+0), input + *(positions+1));
    Client.Ping();

}




int* CLI_Functions::Arguement_Positions(char input[], int length)
{
    static int positions[10] = {0,0,0,0,0,0,0,0,0,0};
    int positionsCounter=0;
    for(int i=0;i<length;i++)
    {
        if(input[i] == ' ')
        {
            positions[positionsCounter] = i+1;
            positionsCounter++;
            input[i] = '\0';
        }

    }
    return positions;

}









