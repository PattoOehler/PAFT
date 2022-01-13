#ifndef META_FILES
#define META_FILES

#include <iostream>

#include "linkedlist.h"
#include "../DHT/DHT.h"

class Meta_Files
{
public:
    linked_list hash_List;

    char FILEID_o[21] = "\xfa\xff\xbb\xcc\xdd\xee\x00\xaa\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13"; // 20 byte File ID
    _160bitnumber id;

    char file_Name[80];
    char output_File_Name[60];
    long long int fileLength; //In bytes
    long long int chunkSize;




    Meta_Files(int chunk_Size, const char *fileName, long long int total_Bytes, linked_list a, _160bitnumber fileID);
    void Write_File();
    static void Make_File(const char *input_File, const char *output_File, _160bitnumber fileID);


private:
    void set_File_Name(const char *fileName);
    void set_Output_File_Name();
};


#endif // META_FILES
