#ifndef __PAFT_DHT_H
#define __PAFT_DHT_H

#include <stdint.h>
#include <mutex>
#include <winsock.h>
#include <random>
#include <ctime>


typedef struct
{
    //160 bits total

    unsigned long long int top=0; //64 bits -- 0     STARTS HERE
    unsigned long long int mid=0; //64 bits -- 128
    unsigned long int bot=0;      //32 bits -- 160    TAIL END


}_160bitnumber;
typedef struct
{
    in_addr addr;
    unsigned short int port =0;
    _160bitnumber id;
    bool is_set = false;
    time_t time_To_Timeout;
    //Timeout ~1h


} DHT_Single_Entry;


typedef struct
{
    DHT_Single_Entry entry[3];


} three_DHT;


namespace paft
{
    class DHT{
    private:
        static int Distance(_160bitnumber id, _160bitnumber id2);
        static int Add_Entry(DHT_Single_Entry Entry);
        static three_DHT Lookup_One_Bucket(_160bitnumber id, int bucket);




    public:
        static std::mt19937_64 gen;

        static three_DHT Find_Value(_160bitnumber id);
        static bool Is_Equal(_160bitnumber id,_160bitnumber id2);


        static int Log_Base_2(unsigned long long int n);
        static void Init();

        static int Add_Entry_All_Buckets();
        static void Print_DHT();
        static void Update_Time(DHT_Single_Entry Update);
        static three_DHT Lookup(_160bitnumber id);
        static void Print_Files();

        static _160bitnumber Random_ID();

        static bool Compare(_160bitnumber id,_160bitnumber id2, _160bitnumber compare_To);

        static std::string ID_To_String(_160bitnumber id_to_print);

        static DHT_Single_Entry Next_Closest_In_Bucket(int bucket, _160bitnumber id, _160bitnumber previous_id);


        static void Print_File_Locations();
    };

}





#endif // __PAFT_DHT_H
