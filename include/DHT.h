#ifndef __PAFT_DHT_H
#define __PAFT_DHT_H

#include <stdint.h>


typedef struct
{
    //160 bits total

    unsigned long long int top=0; //64 bits -- 0     STARTS HERE
    unsigned long long int mid=0; //64 bits -- 128
    unsigned long int bot=0;      //32 bits -- 160    TAIL END


}_160bitnumber;
typedef struct
{
    uint32_t addr;//IP Addr
    unsigned short int port =0;//Port
    _160bitnumber id;//160bit ID
    bool is_set = false;

    //Need to include timestamp for timeout ~1h

} DHT_Single_Entry;


namespace paft
{
    class DHT{
    private:
        //Use keyword new to put on the heap
        static DHT_Single_Entry* DHT_ALL;// = new DHT_Single_Entry[160*20];
        /*
            160 is for every bit for matching (the same ID as SELF)
            20 is a server wide amount to keep that should be enough to never allow a section to go offline
            we do want to keep all 160 because there can be overlaps in nums
        */
        static _160bitnumber* SELF;// = new _160bitnumber;



        static int Insert_Entry(int distance, DHT_Single_Entry Entry);

    public:
        static void Init();
        static int Add_Entry(DHT_Single_Entry Entry);
        static int Test_Add_Entry();
        static void Print_DHT();

    };

}





#endif // __PAFT_DHT_H
