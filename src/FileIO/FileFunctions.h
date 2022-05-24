
#ifndef __PAFT_FILEFUNCTIONS_H
#define __PAFT_FILEFUNCTIONS_H

#include "../DHT/DHT.h"

#include <iostream>

namespace paft
{
    class FileFunctions
    {
    private:

    public:
        static void makePaftlFile(_160bitnumber id, const char *fileLocation);
        static void setIdString(char *idString, _160bitnumber id);
    };

}





#endif // __PAFT_FILEFUNCTIONS_H
