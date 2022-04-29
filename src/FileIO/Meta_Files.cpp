
#include "Meta_Files.h"
#include "linkedlist.h"
#include "sha256.h"
#include "../DHT/DHT.h"

#include <iostream>
#include <fstream>






Meta_Files::Meta_Files(int chunk_Size, const char *fileName,long long int total_Bytes, linked_list a, _160bitnumber fileID)
{
    id=fileID;
    set_File_Name(fileName);
    chunkSize = chunk_Size;
    hash_List = a;
    fileLength = total_Bytes;
    set_Output_File_Name();

}

void Meta_Files::set_File_Name(const char* fileName)
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

std::string Meta_Files::getOutput_File_Name(_160bitnumber file_id)
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



void Meta_Files::set_Output_File_Name()
{

    std::string filepath = getOutput_File_Name(id);

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
    memcpy(buf+100, (char *)&fileLength, 8);
    memcpy(buf+108, (char *)&chunkSize, 4 );

    metaFile.write(buf, 112);

    int len = hash_List.length;
    for(int i=0; i<len; i++)
    {
        //Keeping the change of string to char* on 2 seperate lines fixed a problem - keep it
        std::string strChunkHash = hash_List.pop_head();
        const char *chunkHash= strChunkHash.c_str();

        metaFile.write(chunkHash, 64);
        memcpy(buf+bufCounter, chunkHash, 64);
        bufCounter+=64;
    }

    metaFile.close();

    std::cout << "Created MetaDatafile checksum: " << sha256(buf, bufCounter) << "\n";

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
    linked_list a;
    do
    {
        readBytes = fread(buf, 1, Eight_MiB, fp);
        std::string output1 = sha256(buf, readBytes);
        a.add_node(output1);

        totalBytes += readBytes;

    }while(readBytes == Eight_MiB);



    std::cout << "Read a total of " << totalBytes << " bytes\n";

    fclose(fp);
    free(buf);

    Meta_Files* mf = new Meta_Files(Eight_MiB, output_File, totalBytes, a, fileID);

    mf->Write_File();


}

