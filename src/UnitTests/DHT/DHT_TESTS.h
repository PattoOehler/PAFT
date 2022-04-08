
#ifndef __PAFT_DHT_TESTS_H
#define __PAFT_DHT_TESTS_H

#include "../../DHT/DHT.h"

#include <iostream>

namespace paft
{
    class DHT_TESTS{
        public:


        // MANUAL TESTS
        static void Manual_DHT__Print_ID();


        // AUTOMATIC TESTS
        static void DHT__IsEqual();
        static void DHT__Compare();
        static void DHT__Random_ID();

    };

}



#endif // __PAFT_DHT_TESTS_H
