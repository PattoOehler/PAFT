#include "FileFunctions.h"
#include "../DHT/DHT_Access.h"
#include "Meta_Files.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <sys/stat.h>


using namespace paft;






void File_Functions::Load_State()
{

    Load_DHT_Entries();
    Load_File_Entries();
    Load_Local_File_Locations();


}


void File_Functions::Load_DHT_Entries()
{

    std::ifstream rf("Saves/DHTEntries.pafts", std::ios::out | std::ios::binary);
    if(!rf)
    {
      std::cout << "ERROR OPENING FILE FileFunctions::loadDHTEntries\n";
      return;
    }


    for(int i=0; i<(160*20); i++)
    {
        DHT_Single_Entry curr_Entry;
        rf.read((char *) &curr_Entry, sizeof(DHT_Single_Entry));

        DHT_Access::Write_To_DHT(curr_Entry, i);
    }


    rf.close();
    if(!rf.good()) {
        std::cout << "ERROR CLOSING FILE FileFunctions::loadDHTEntries\n";
        return;
    }



}




void File_Functions::Load_File_Entries()
{

    std::ifstream rf("Saves/DHTFileEntries.pafts", std::ios::out | std::ios::binary);
    if(!rf)
    {
      std::cout << "ERROR OPENING FILE FileFunctions::loadFileEntries\n";
      return;
    }


    for(int i=0; i<100; i++)
    {
        DHT_Single_Entry curr_Entry;
        rf.read((char *) &curr_Entry, sizeof(DHT_Single_Entry));

        DHT_Access::Write_To_File_IDs(curr_Entry, i);
    }


    rf.close();
    if(!rf.good()) {
        std::cout << "ERROR CLOSING FILE FileFunctions::loadFileEntries\n";
        return;
    }



}





void File_Functions::Load_Local_File_Locations()
{

    std::ifstream rf("Saves/DHTFileLocations.pafts", std::ios::out | std::ios::binary);
    if(!rf)
    {
      std::cout << "ERROR OPENING FILE FileFunctions::loadLocalFileLocations\n";
      return;
    }


    std::string entry;
    for(int i=0; i<100;)
    {
        char curr_Character[1];
        rf.read(curr_Character, 1);
        if(curr_Character[0] == '\0')
        {

            DHT_Access::Set_Local_File_Location(entry, i);
            entry.clear();
            i++;
        }
        else
        {
            entry.append(1,curr_Character[0]);
        }

    }


    rf.close();
    if(!rf.good()) {
        std::cout << "ERROR CLOSING FILE FileFunctions::loadLocalFileLocations\n";
        return;
    }



}








void File_Functions::Save_State()
{
    Save_DHT_Entries();
    Save_File_Entries();
    Save_Local_File_Locations();

}




void File_Functions::Save_DHT_Entries()
{

    std::ofstream wf("Saves/DHTEntries.pafts", std::ios::out | std::ios::binary);
    if(!wf) {
      std::cout << "ERROR OPENING FILE FileFunctions::saveDHTEntries\n";
      return;
    }


    for(int i=0; i<(160*20); i++)
    {
        DHT_Single_Entry curr_Entry = DHT_Access::Access_DHT(i);
        wf.write( (char *) &curr_Entry, sizeof(DHT_Single_Entry));

    }


    wf.close();
    if(!wf.good()) {
      std::cout << "ERROR OCCURED WHEN WRITING TO FILE FileFunctions::saveDHTEntries\n";
      return;
   }

}








void File_Functions::Save_File_Entries()
{

    std::ofstream wf("Saves/DHTFileEntries.pafts", std::ios::out | std::ios::binary);
    if(!wf) {
      std::cout << "ERROR OPENING FILE FileFunctions::saveFileEntries\n";
      return;
    }


    for(int i=0; i<100; i++)
    {
        DHT_Single_Entry curr_Entry = DHT_Access::Access_File_IDs(i);
        wf.write( (char *) &curr_Entry, sizeof(DHT_Single_Entry));

    }


    wf.close();
    if(!wf.good()) {
      std::cout << "ERROR OCCURED WHEN WRITING TO FILE FileFunctions::saveFileEntries\n";
      return;
   }

}






void File_Functions::Save_Local_File_Locations()
{

    std::ofstream wf("Saves/DHTFileLocations.pafts", std::ios::out | std::ios::binary);
    if(!wf) {
      std::cout << "ERROR OPENING FILE FileFunctions::saveLocalFileLocations\n";
      return;
    }


    for(int i=0; i<100; i++)
    {
        std::string localFileLocation = DHT_Access::Get_Local_File_Location(i);
        if(localFileLocation.empty())
        {
            wf.write("\0", 1);
        } else
        {
            const char *cstr = localFileLocation.c_str();
            int j;
            for(j=0; cstr[j] != '\0'; j++);

            wf.write( cstr , j+1);
        }



    }


    wf.close();
    if(!wf.good()) {
      std::cout << "ERROR OCCURED WHEN WRITING TO FILE FileFunctions::saveLocalFileLocations\n";
      return;
   }

}



void File_Functions::Allocate_File()
{
    int fileChunks = Meta_Files::getChunks("Test_Metafiles\\meta.paft");

    std::ofstream wf("Test_Metafiles\\Downloaded_File", std::ios::out | std::ios::binary);
    if(!wf) {
        std::cout << "Cannot open file!" << "\n";
        return;
    }
    int Eight_MiB = 8000000;
    char *EightMB = (char *) malloc(Eight_MiB);

    for(int i=0; i<fileChunks-1; i++)
        wf.write(EightMB, Eight_MiB);
    wf.close();
    free(EightMB);


}


void File_Functions::Allocate_File( std::string metafilename )
{
    int fileChunks = Meta_Files::getChunks(  metafilename );

    std::ofstream wf("Test_Metafiles\\Downloaded_File", std::ios::out | std::ios::binary);
    if(!wf) {
        std::cout << "Cannot open file!" << "\n";
        return;
    }
    int Eight_MiB = 8000000;
    char *EightMB = (char *) malloc(Eight_MiB);

    for(int i=0; i<fileChunks-1; i++)
        wf.write(EightMB, Eight_MiB);
    wf.close();
    free(EightMB);


}


long File_Functions::Get_File_Length(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}
