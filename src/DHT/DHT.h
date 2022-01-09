#ifndef __PAFT_DHT_H
#define __PAFT_DHT_H

#include <stdint.h>
#include <mutex>
#include <winsock.h>



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
        static bool Compare(_160bitnumber id,_160bitnumber id2, _160bitnumber compare_To);

    public:
        static three_DHT Find_Value(_160bitnumber id);
        static bool IsEqual(_160bitnumber id,_160bitnumber id2);


        static int Log2(unsigned long long int n);
        static void Init();

        static int Test_Add_Entry();
        static void Print_DHT();
        static void Update_Time(DHT_Single_Entry Update);
        static three_DHT Lookup(_160bitnumber id);
        static void Print_Files();

        static _160bitnumber Random_ID();




    };

}





#endif // __PAFT_DHT_H
