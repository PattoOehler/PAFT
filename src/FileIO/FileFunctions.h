
#ifndef __PAFT_FILEFUNCTIONS_H
#define __PAFT_FILEFUNCTIONS_H

#include "../DHT/DHT.h"

#include <iostream>
#include <vector>
#include <string>

namespace paft
{
    class FileFunctions
    {
    private:

    public:
        static void loadState();
        static void saveState();

        static void saveDHTEntries();
        static void saveFileEntries();
        static void saveLocalFileLocations();

        static void loadDHTEntries();
        static void loadFileEntries();
        static void loadLocalFileLocations();

        static std::vector<std::string> get_all_files_names_within_folder(std::string folder);
    };

}





#endif // __PAFT_FILEFUNCTIONS_H
