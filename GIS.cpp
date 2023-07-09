#include <iostream>
#include "CommandProcessor.h"
#include "Logger.h"
using namespace GIS;

int main(int argc, char* argv[]) {

    CommandProcessor* commandprocessor1 = new CommandProcessor(argv[1], argv[2]);
    if (argc != 4) {
        return 0;
    } else {
        commandprocessor1->readScript();
    }
    Logger::closeLogger();
    return 0;
    return 0;
}
