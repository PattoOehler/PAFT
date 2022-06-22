
#ifndef __PAFT_DHT_TESTS_H
#define __PAFT_DHT_TESTS_H

#include "../../DHT/DHT.h"

#include <iostream>

namespace paft
{
    class DHT_TESTS{
        public:


        // MANUAL TESTS



        // AUTOMATIC TESTS
        static void DHT__ID_To_String();
        static void DHT__Is_Equal();
        static void DHT__Compare();
        static void DHT__Random_ID();

    };

}



#endif // __PAFT_DHT_TESTS_H
