

#ifndef GI_SYSTEM_COMMANDPROCESSOR_H
#define GI_SYSTEM_COMMANDPROCESSOR_H

#include "World.h"
#include <vector>
using namespace std;
namespace GIS {

    class CommandProcessor {
        public:
            World world1;
            CommandProcessor();
            int readScript();
            void tokenize(string const &str, const char delim,
                vector<string> &out);
            void importCommand(string const &recordFile, string const &databaseFile);
    };

} // GIS

#endif //GI_SYSTEM_COMMANDPROCESSOR_H
