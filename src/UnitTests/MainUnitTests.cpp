
#include "MainUnitTests.h"


#include "DHT/DHT_TESTS.h"

using namespace paft;

void MainUnitTests::Run_All_Manual_Tests()
{



}


void MainUnitTests::Run_All_Auto_Tests()
{
    DHT_TESTS::DHT__Is_Equal();
    DHT_TESTS::DHT__Compare();
    DHT_TESTS::DHT__Random_ID();
    DHT_TESTS::DHT__ID_To_String();
    std::cout << "All tests have been ran\n";
}
