#include <iostream>
#include "CommandProcessor.h"
#include "Logger.h"
using namespace GIS;

/**
 *
Commandline execution example
 g++ GIS.cpp CommandProcessor.cpp World.cpp HashTable.cpp BufferPool.cpp GISRecord.cpp Logger.cpp PRQuadtree.cpp -o GIS
GIS ./Files/database.txt ./Files/script01.txt ./Files/log.txt
 */

int main(int argc, char* argv[]) {

    CommandProcessor* commandprocessor1 = new CommandProcessor(argv[1], argv[2], argv[3]);
    if (argc != 4) {
        return 0;
    } else {
        commandprocessor1->readScript();
    }
    Logger::closeLogger();
    return 0;
    return 0;
}
