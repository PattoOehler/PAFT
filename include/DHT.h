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
    in_addr addr;//IP Addr
    unsigned short int port =0;//Port
    _160bitnumber id;//160bit ID
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
        //Use keyword new to put on the heap
        static DHT_Single_Entry* DHT_ALL;// = new DHT_Single_Entry[160*20];
        static std::mutex* mutex_All;
        static std::mutex* mutex_FileIds;
        static DHT_Single_Entry* FileIds;
        /*
            160 is for every bit for matching (the same ID as SELF)
            20 is a server wide amount to keep that should be enough to never allow a section to go offline
            we do want to keep all 160 because there can be overlaps in nums
        */
        static _160bitnumber* SELF;// = new _160bitnumber;
        static unsigned short int SELF_PORT;

        static int Distance(_160bitnumber id, _160bitnumber id2);
        static int Add_Entry(DHT_Single_Entry Entry);
        static three_DHT Lookup_One_Bucket(_160bitnumber id, int bucket);
        static bool Compare(_160bitnumber id,_160bitnumber id2, _160bitnumber compare_To);
        static DHT_Single_Entry Access_FileIds(int position);
        static void Write_To_FileIds(DHT_Single_Entry write, int position);

    public:
        static _160bitnumber Get_SELF();
        static void Store_FileId(DHT_Single_Entry entry);
        static three_DHT Find_Value(_160bitnumber id);
        static bool IsEqual(_160bitnumber id,_160bitnumber id2);
        static void Write_To_DHT(DHT_Single_Entry write, int position);
        static DHT_Single_Entry Access_DHT(int position);
        static int Log2(unsigned long long int n);
        static void Init();

        static int Test_Add_Entry();
        static void Print_DHT();
        static void Update_Time(DHT_Single_Entry Update);
        //static void Lookup  //Should return 3 Ip addrs +
        static three_DHT Lookup(_160bitnumber id);

        static void Set_Self_Port(unsigned short int port);
        static unsigned short int Get_Self_Port();


    };

}





#endif // __PAFT_DHT_H
