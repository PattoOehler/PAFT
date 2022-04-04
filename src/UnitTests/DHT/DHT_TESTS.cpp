
#include "DHT_TESTS.h"
using namespace paft;

void DHT_TESTS::Manual_DHT__Print_ID()
{
    std::cout << "\nDHT_TESTS::Manual_DHT__Print_ID()\n\n";

    _160bitnumber printID;
    char *bits = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\xFF\xFE\xFD";

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
