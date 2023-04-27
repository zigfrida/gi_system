#include <iostream>
#include "CommandProcessor.h"
using namespace GIS;

int main() {

    CommandProcessor* commandprocessor1 = new CommandProcessor();
    commandprocessor1->readScript();
    return 0;
}
