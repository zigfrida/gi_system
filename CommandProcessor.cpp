
// Created by dldms on 4/26/2023.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "CommandProcessor.h"
#include "World.h"
#include "HashTable.h"
#include "GISRecord.h"
#include "Logger.h"

using namespace std;

namespace GIS {

    CommandProcessor::CommandProcessor(){
        bufferPool1 = new BufferPool("../Files/database.txt");
    }

    void CommandProcessor::tokenize(std::string const &str, const char delim,
                  std::vector<std::string> &out)
    {
        // construct a stream from the string
        std::stringstream ss(str);

        std::string s;
        while (std::getline(ss, s, delim)) {
            out.push_back(s);
        }
    }

    void CommandProcessor::importCommand(string const &recordFile, string const &databaseFile) {
        nameIndex = new HashTable();
        vector<GISRecord> dbRecords;
        ifstream source(recordFile); // Source file
//        ofstream database; // Destination file
//        database.open("../Files/database.txt", ios::out | ios::in);

        int lineOffSet = 0;

        if (!source.is_open()) {
            cout << "Error opening source file" << endl;
        } else {
            string line;
            bool firstLine = true;

            while (getline(source, line)) {
                istringstream iss(line);
                string feature;
                vector<string> featureInfo;

                if (!firstLine) {
                    while (getline(iss, feature, '|')) {
                        featureInfo.push_back(feature);
                    }

                    if (world1.isItInWorldBoundary(featureInfo[7], featureInfo[8])) {
                        string key = featureInfo[1] + " " + featureInfo[3]; // Key is the concatenation of feature name and state abbreviation
                        string value = to_string(lineOffSet + 1); // line the feature was found in the import file
                        GISRecord tempRec;
                        tempRec.FEATURE_ID = stoi(featureInfo[0]);
                        tempRec.FEATURE_Name = featureInfo[1];
                        tempRec.FEATURE_CLASS = featureInfo[2];
                        tempRec.Latitude = World::convertStringLatLongToInt(featureInfo[7]);
                        tempRec.longitude = World::convertStringLatLongToInt(featureInfo[8]);
                        tempRec.STATE_Abbreviation = featureInfo[3];
                        dbRecords.push_back(tempRec);
                        nameIndex->insert(key, value);
                        lineOffSet++;
                    }
                }
                if (firstLine) firstLine = false;
            }

            bufferPool1->appendToDatabase(dbRecords, databaseFile);

//            nameIndex->displayHashTable(); // Visualization purposes
            source.close();
//            database.close();
        }
    }



    int CommandProcessor::readScript()
    {
        string myText;
        ifstream ScriptFile1("../Files/script01.txt");
        string file = "../Files/VA_Monterey.txt"; // + concatenated[1];
        string dbFile = "../Files/database.txt";
        int commandCounter = 0;

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
                        world1.createWorld(concatenated[1], concatenated[2], concatenated[3], concatenated[4]);
                        commandCounter++;
                    } else if (command == "import") {
                        stringstream logMessage;
                        logMessage << "Command " << commandCounter << ": " << myText << endl;
                        Logger::getInstance().writeLog(logMessage.str());

                        // Uncomment lines to run import command
                        cout << "Import Command, file name: " << concatenated[1] << endl;

                        importCommand(file, dbFile);
                    } else if (command=="what_is") {
                        GISRecord what_isThis = bufferPool1->whatIs(concatenated[1], concatenated[2]);
                    }
                } else {
                    Logger::getInstance().writeLog(myText);
                }

            }
        }
        ScriptFile1.close();
        return 0;
    }

} // GIS

