
#ifndef __PAFT_MAINCLI_H
#define __PAFT_MAINCLI_H



namespace paft
{
    class MainCli{
    public:
        static int Start_Client();
        static void Self_Find_Random_Node();
        static void Self_Find_Random_File();

    private:
        static int Command_Parser(char *Input, int a);
        static bool String_Compare(char* string1, const char* string2);
    };

}





#endif // __PAFT_MAINCLI_H
