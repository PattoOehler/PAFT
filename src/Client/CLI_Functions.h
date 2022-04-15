

#ifndef __PAFT_CLI_FUNCTIONS_H
#define __PAFT_CLI_FUNCTIONS_H



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

    };

}





#endif // __PAFT_CLI_FUNCTIONS_H
