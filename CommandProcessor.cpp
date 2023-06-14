
// Created by dldms on 4/26/2023.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "CommandProcessor.h"
#include "World.h"

using namespace std;

namespace GIS {

    CommandProcessor::CommandProcessor()
    {

    }

    void tokenize(std::string const &str, const char delim,
                  std::vector<std::string> &out)
    {
        // construct a stream from the string
        std::stringstream ss(str);

        std::string s;
        while (std::getline(ss, s, delim)) {
            out.push_back(s);
        }
    }



    int CommandProcessor::readScript()
    {
        string myText;
        ifstream ScriptFile1("../script01.txt");

        // Use a while loop together with the getline() function to read the file line by line
        while (getline (ScriptFile1, myText)) {
            // Output the text from the file
            string command = "";
            if (myText.length() > 0) {
                if (myText[0] != ';') {
                    vector<string> concatenated;
                    tokenize(myText, '\t', concatenated);
                    command = concatenated[0];
                    if (command == "world") {
                        //run world
                        World* world1 = new World();
                        world1->createWorld(concatenated[1], concatenated[2], concatenated[3], concatenated[4]);
                    }
                }

            }
        }
        ScriptFile1.close();
        return 0;
    }

} // GIS

