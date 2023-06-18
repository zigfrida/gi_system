
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

using namespace std;

namespace GIS {

    CommandProcessor::CommandProcessor(){}

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
        HashTable* hashTable = new HashTable();
        vector<GISRecord> dbBuffer;
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
                        dbBuffer.push_back(tempRec);
                        hashTable->insert(key, value);
                        lineOffSet++;
                    }
                }
                if (firstLine) firstLine = false;
            }

            bufferPool1.appendToDatabase(dbBuffer, databaseFile);

//            hashTable->displayHashTable(); // Visualization purposes
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
                    } else if (command == "import") {
                        // Uncomment lines to run import command
                        cout << "Import Command, file name: " << concatenated[1] << endl;

                        importCommand(file, dbFile);
                        bufferPool1.readDatabaseFile(dbFile);

                    }

                }

            }
        }
        ScriptFile1.close();
        return 0;
    }

} // GIS

