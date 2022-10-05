
#include "Meta_Files.h"
#include "Linked_List.h"
#include "SHA_256.h"
#include "../DHT/DHT.h"
#include "File_Functions.h"

#include <iostream>
#include <fstream>



using namespace paft;


Meta_Files::Meta_Files(int chunkSize, const char *fileName,long long int total_Bytes, Linked_List a, _160bitnumber fileID)
{
    id=fileID;
    Set_File_Name(fileName);
    chunk_Size = chunkSize;
    hash_List = a;
    file_Length = total_Bytes;
    Set_Output_File_Name();

}

void Meta_Files::Set_File_Name(const char* fileName)
{
    int i=0;
    bool zero=false;
    while((i<80))
    {
        if(!zero)
        {
            if(fileName[i] == '\0')
                zero = true;
            file_Name[i] = fileName[i];
        }
        else
            file_Name[i] = '\0';
        i++;
    }

}

std::string Meta_Files::Get_Output_File_Name(_160bitnumber file_id)
{
    std::string filepath;

    filepath.push_back('M');
    filepath.push_back('e');
    filepath.push_back('t');
    filepath.push_back('a');
    filepath.push_back('f');
    filepath.push_back('i');
    filepath.push_back('l');
    filepath.push_back('e');
    filepath.push_back('s');
    filepath.push_back('/');

    int counter;
    char *FILEID = (char *)&file_id;

    for(counter=0; counter<20;counter++)
    {
        unsigned char a = FILEID[counter];
        int ab = a;
        int i;
        for(i=0;i*16<ab-16;i++)
        {

        }
        unsigned int num1 = i;

        if(num1 > 9)
            filepath.push_back('a' + num1-10);
        else
            filepath.push_back('0' + num1);

        unsigned int num2 = ((unsigned int)FILEID[0+counter]) % 16;

        if(num2 > 9)
            filepath.push_back('a' + num2-10);
        else
            filepath.push_back('0' + num2);
    }

    filepath.push_back('.');
    filepath.push_back('p');
    filepath.push_back('a');
    filepath.push_back('f');
    filepath.push_back('t');
    filepath.push_back('\0');


    return filepath;
}



void Meta_Files::Set_Output_File_Name()
{

    std::string filepath = Get_Output_File_Name(id);

    for(unsigned int i=0;i<filepath.size();i++)
    {
        output_File_Name[i] = filepath[i];

    }


}



void Meta_Files::Write_File()
{

    std::ofstream metaFile;
    metaFile.open(output_File_Name, std::ios::out | std::ios::binary);

    if(!metaFile)
    {
        std::cout << "Error opening the output file!\n";
        return;
    }

    int Eight_MiB = 8000000;
    char *buf;
    buf = (char *) malloc(Eight_MiB);
    long long unsigned int bufCounter=112;

    char *FILEID = (char *)&id;


    memcpy(buf, FILEID, 20);
    memcpy(buf+20, (char *)&file_Name, 80);
    memcpy(buf+100, (char *)&file_Length, 8);
    memcpy(buf+108, (char *)&chunk_Size, 4 );

    metaFile.write(buf, 112);

    int len = hash_List.length;
    for(int i=0; i<len; i++)
    {
        //Keeping the change of string to char* on 2 seperate lines fixed a problem - keep it
        std::string strChunkHash = hash_List.Pop_Head();
        const char *chunkHash= strChunkHash.c_str();

        metaFile.write(chunkHash, 64);
        memcpy(buf+bufCounter, chunkHash, 64);
        bufCounter+=64;
    }

    metaFile.close();

    std::cout << "Download string 'download_file_on_network " << sha256(buf, bufCounter) << " " << DHT::ID_To_String(id) << "'\n";
    std::cout << "Metadata file created\n";
    free(buf);

}

void Meta_Files::Make_File(const char *input_File, const char *output_File, _160bitnumber fileID)
{



   FILE *fp = fopen(input_File, "rb"); // read mode

   if (fp == NULL)
       exit(1);




   int Eight_MiB = 8000000;

   char *buf;
   buf = (char *) malloc(Eight_MiB);


    int readBytes=0;
    long long totalBytes=0;
    Linked_List a;
    do
    {
        readBytes = fread(buf, 1, Eight_MiB, fp);
        std::string output1 = sha256(buf, readBytes);
        a.Add_Node(output1);

        totalBytes += readBytes;

    }while(readBytes == Eight_MiB);



    std::cout << "\nRead a total of " << totalBytes << " bytes\n";

    fclose(fp);
    free(buf);

    Meta_Files* mf = new Meta_Files(Eight_MiB, output_File, totalBytes, a, fileID);

    mf->Write_File();


}




std::string Meta_Files::Get_Check_Sum(int chunk, std::string metaFile)
{
    std::ifstream rf(metaFile, std::ios::out | std::ios::binary);
    if(!rf) {
        std::cout << "Cannot open file!\n";
        return "Error";
    }

    rf.seekg(112+64*chunk);

    char checksum[64];
    rf.read(checksum, 64);

    std::string myString(checksum, 64);

    return myString;
}


int Meta_Files::Get_Chunks(std::string metaFile)
{
    long metafilelen = File_Functions::Get_File_Length(metaFile);

    int fileChunks = (metafilelen - 112) / 64;

    return fileChunks;

}

