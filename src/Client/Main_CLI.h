
#ifndef __PAFT_MAINCLI_H
#define __PAFT_MAINCLI_H



namespace paft
{
    class Main_CLI{
    public:
        static int Start_Client();
        static void Self_Find_Random_Node();
        static void Self_Find_Random_File();
        static void Self_Store_Random_File();
        static void Self_Find_Random_Node_Network();

    private:
        static int Command_Parser(char *Input, int a);
        static bool String_Compare(char* string1, const char* string2);
    };

}





#endif // __PAFT_MAINCLI_H
