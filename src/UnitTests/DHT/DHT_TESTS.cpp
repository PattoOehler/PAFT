
#include "DHT_TESTS.h"
using namespace paft;

void DHT_TESTS::Manual_DHT__Print_ID()
{
    std::cout << "\nDHT_TESTS::Manual_DHT__Print_ID()\n\n";

    _160bitnumber printID;
    const char *bits = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\xFF\xFE\xFD";

    memcpy((char *)&printID,bits,20);


    std::cout << "000102030405060708090A0B0C0D0E0F10FFFEFD <- Expected\n";
    DHT::Print_ID(printID);
    std::cout << " <- returned\n";


    std::cout << "\nDHT_TESTS::Manual_DHT__Print_ID()\n";
}



void DHT_TESTS::DHT__IsEqual()
{
    _160bitnumber num = DHT::Random_ID();

    if(!DHT::IsEqual(num, num))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }

    _160bitnumber num2 = num;

    num2.bot += 1;
    if(DHT::IsEqual(num, num2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }

    num2.bot -= 1;
    num2.mid += 1;
    if(DHT::IsEqual(num, num2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::IsEqual is not performing as expected\n";
    }

    num2.mid -= 1;
    num2.top += 1;
    if(DHT::IsEqual(num, num2))
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

    if(DHT::IsEqual(id, id2))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::IsEqual(id2, id3))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::IsEqual(id3, id4))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::IsEqual(id4, id5))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::IsEqual(id2, id4))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::IsEqual(id2, id5))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
    if(DHT::IsEqual(id3, id5))
    {
        std::cout << "\n\nUNIT TEST ERROR!!!\n\n";
        std::cout << "DHT::Random_ID() Is not producing randomIDs\n";
    }
}

