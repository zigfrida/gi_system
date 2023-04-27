//
// Created by dldms on 4/26/2023.
//
#include <iostream>
#include <fstream>
#include "CommandProcessor.h"
using namespace std;

namespace GIS {

    CommandProcessor::CommandProcessor()
    {

    }

    int CommandProcessor::readScript()
    {
        string myText;
        ifstream ScriptFile1("../script01.txt");

        // Use a while loop together with the getline() function to read the file line by line
        while (getline (ScriptFile1, myText)) {
            // Output the text from the file
            cout << myText;
        }
        ScriptFile1.close();
        return 0;
    }

} // GIS