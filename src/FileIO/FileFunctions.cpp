#include "FileFunctions.h"

#include <iostream>
#include <fstream>


using namespace paft;

void FileFunctions::makePaftlFile(_160bitnumber id, const char *fileLocation)
{
    char paftlLocation[57];


    paftlLocation[0] = 'M';
    paftlLocation[1] = 'e';
    paftlLocation[2] = 't';
    paftlLocation[3] = 'a';
    paftlLocation[4] = 'f';
    paftlLocation[5] = 'i';
    paftlLocation[6] = 'l';
    paftlLocation[7] = 'e';
    paftlLocation[8] = 's';
    paftlLocation[9] = '/';


    setIdString(paftlLocation, id);

    paftlLocation[50] = '.';
    paftlLocation[51] = 'p';
    paftlLocation[52] = 'a';
    paftlLocation[53] = 'f';
    paftlLocation[54] = 't';
    paftlLocation[55] = 'l';
    paftlLocation[56] = '\0';

    std::cout << paftlLocation << "\n";

    std::ofstream myfile;
    myfile.open (paftlLocation);
    myfile << fileLocation;
    myfile.close();

    return;
}


void FileFunctions::setIdString(char *idString, _160bitnumber id)
{

    char *FILEID = (char *)&id;

    int offset = 10;

    for(int counter=0; counter<20;counter++)
    {
        unsigned char a = FILEID[counter];
        int ab = a;
        int i;
        for(i=0;i*16<ab-16;i++)
        {

        }
        unsigned int num1 = i;

        if(num1 > 9)
            idString[counter*2+offset] = 'a' + num1-10 ;
        else
            idString[counter*2+offset] = '0' + num1;

        unsigned int num2 = ((unsigned int)FILEID[0+counter]) % 16;

        if(num2 > 9)
            idString[counter*2 + 1+offset] = 'a' + num2-10;
        else
            idString[counter*2 + 1+offset] = '0' + num2;
    }


}
