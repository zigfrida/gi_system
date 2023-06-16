#include <iostream>
#include "CommandProcessor.h"
#include "Logger.h"
using namespace GIS;

int main() {

    CommandProcessor* commandprocessor1 = new CommandProcessor();
    commandprocessor1->readScript();

    Logger::closeLogger();
    return 0;
}
