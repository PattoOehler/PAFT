#ifndef META_FILES
#define META_FILES

#include <iostream>

#include "Linked_List.h"
#include "../DHT/DHT.h"

class Meta_Files
{
public:
    Linked_List hash_List;

    _160bitnumber id;

    char file_Name[80];
    char output_File_Name[60];
    long long int file_Length; //In bytes
    long long int chunk_Size;




    Meta_Files(int chunk_Size, const char *fileName, long long int total_Bytes, Linked_List a, _160bitnumber fileID);
    void Write_File();

    static void Make_File(const char *input_File, const char *output_File, _160bitnumber fileID);

    static std::string Get_Output_File_Name(_160bitnumber id);


    static std::string Get_Check_Sum(int chunk, std::string metaFile);
    static int Get_Chunks(std::string metaFile);

private:
    void Set_File_Name(const char *fileName);
    void Set_Output_File_Name();
};


#endif // META_FILES
