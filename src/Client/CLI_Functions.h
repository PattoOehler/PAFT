

#ifndef __PAFT_CLI_FUNCTIONS_H
#define __PAFT_CLI_FUNCTIONS_H


using namespace std;

namespace paft
{
    class CLI_Functions{
    public:
        static void Help_Command();
        static void Self_Ping_Command();
        static void Test_DHT_Command();
        static void Store_File_Net_And_Get_Meta_Back_Command();
        static void Store_File_Net_And_Get_Chunk_Back_Command();
        static void Print_Self_Command();
        static void Upload_File_Network(char input[], int length);
        static void Downlaod_File_Network(char input[], int length);
        static void Ping(char input[], int length);
        static int* Arguement_Positions(char input[], int length);
    };

}





#endif // __PAFT_CLI_FUNCTIONS_H
