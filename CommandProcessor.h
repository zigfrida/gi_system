

#ifndef GI_SYSTEM_COMMANDPROCESSOR_H
#define GI_SYSTEM_COMMANDPROCESSOR_H

#include "World.h"
#include "BufferPool.h"
#include "PRQuadtree.h"
#include "HashTable.h"
#include <vector>
using namespace std;
namespace GIS {

    class CommandProcessor {
        public:
            World world1;
            BufferPool* bufferPool1;
            HashTable* nameIndex;
            PRQuadtree* prquadtree;
            int lineOffSet;
            string scriptFile;
            string dbFile;
            CommandProcessor(string dbArg, string scriptArg);
            int readScript();
            void tokenize(string const &str, const char delim,
                vector<string> &out);
            void importCommand(string const &recordFile, string const &databaseFile);
            void appendToDatabase(vector<GISRecord> records1, string filePath);

    };

} // GIS

#endif //GI_SYSTEM_COMMANDPROCESSOR_H
