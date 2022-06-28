
#include "DHT_TESTS.h"
using namespace paft;

void DHT_TESTS::DHT__ID_To_String()
{
    _160bitnumber printID;
    const char *bits = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\xFF\xFE\xFD";

    memcpy((char *)&printID,bits,20);

    std::string expected = "000102030405060708090A0B0C0D0E0F10FFFEFD";
    std::string received = DHT::ID_To_String(printID);

    if(expected != received)
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::ID_To_String is not performing as expected";
    }
}



void DHT_TESTS::DHT__Is_Equal()
{
    _160bitnumber num = DHT::Random_ID();

    if(!DHT::Is_Equal(num, num))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }

    _160bitnumber num2 = num;

    num2.bot += 1;
    if(DHT::Is_Equal(num, num2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }

    num2.bot -= 1;
    num2.mid += 1;
    if(DHT::Is_Equal(num, num2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }

    num2.mid -= 1;
    num2.top += 1;
    if(DHT::Is_Equal(num, num2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }
}


void DHT_TESTS::DHT__Compare()
{

    _160bitnumber num = DHT::Random_ID();
    _160bitnumber num2 = num;
    num2.bot += 1;

    if(!DHT::Compare(num,num2, num))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Compare is not performing as expected\n";
    }

    if(DHT::Compare(num2,num, num))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Compare is not performing as expected2\n";
    }

    if(DHT::Compare(num,num, num))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Compare is not performing as expected2\n";
    }

    if(DHT::Compare(num2,num2, num))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Compare is not performing as expected2\n";
    }

}


void DHT_TESTS::DHT__Random_ID()
{

    _160bitnumber id = DHT::Random_ID();
    _160bitnumber id2 = DHT::Random_ID();
    _160bitnumber id3 = DHT::Random_ID();
    _160bitnumber id4 = DHT::Random_ID();
    _160bitnumber id5 = DHT::Random_ID();

    if(DHT::Is_Equal(id, id2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::Is_Equal(id2, id3))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::Is_Equal(id3, id4))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::Is_Equal(id4, id5))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::Is_Equal(id2, id4))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::Is_Equal(id2, id5))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::Is_Equal(id3, id5))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
}


void DHT_TESTS::DHT__Log_Base_2()
{
    unsigned long long int num=0;

    if(DHT::Log_Base_2(num) != 0)
        std::cout << "DHT::Log_Base_2 Not working as expected\n";

    num=1;
    if(DHT::Log_Base_2(num) != 0)
        std::cout << "DHT::Log_Base_2 Not working as expected\n";

    num=400;
    if(DHT::Log_Base_2(num) != 8)
        std::cout << "DHT::Log_Base_2 Not working as expected\n";

    num=100000000000;
    if(DHT::Log_Base_2(num) != 36)
        std::cout << "DHT::Log_Base_2 Not working as expected\n";

    num=-1;
    if(DHT::Log_Base_2(num) != 63)
        std::cout << "DHT::Log_Base_2 Not working as expected\n";


}





