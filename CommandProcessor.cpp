
// Created by dldms on 4/26/2023.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "CommandProcessor.h"
#include "World.h"
#include "HashTable.h"
#include "PRQuadtree.h"
#include "GISRecord.h"
#include "Logger.h"

using namespace std;

namespace GIS {

    CommandProcessor::CommandProcessor(){
        bufferPool1 = new BufferPool("../Files/database.txt");
        lineOffSet = 0;
        nameIndex = new HashTable();
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
        vector<GISRecord> dbRecords;
        ifstream source(recordFile); // Source file
//        ofstream database; // Destination file
//        database.open("../Files/database.txt", ios::out | ios::in);



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
                        string value = to_string(lineOffSet); // line the feature was found in the import file
                        GISRecord tempRec;
                        tempRec.FEATURE_ID = stoi(featureInfo[0]);
                        tempRec.FEATURE_Name = featureInfo[1];
                        tempRec.FEATURE_CLASS = featureInfo[2];
                        tempRec.latitude = World::convertStringLatLongToInt(featureInfo[7]);
                        tempRec.longitude = World::convertStringLatLongToInt(featureInfo[8]);
                        tempRec.STATE_Abbreviation = featureInfo[3];
                        tempRec.COUNTY_NAME = featureInfo[5];
                        dbRecords.push_back(tempRec);
                        nameIndex->insert(key, value);
                        lineOffSet++;
                        CoordinateIndex* newCordIndex = new CoordinateIndex(tempRec.latitude, tempRec.longitude);
                        newCordIndex->fileOffsets.push_back(lineOffSet);

                        prquadtree->insert(*newCordIndex, tempRec);
                    }
                }
                if (firstLine) firstLine = false;
            }

            appendToDatabase(dbRecords, databaseFile);
//            nameIndex->displayHashTable(); // Visualization purposes
            source.close();
        }
    }

    /**
     * Function to append record to the database.txt
     * @param records1
     * @param filePath
     */
    void CommandProcessor::appendToDatabase(vector<GISRecord> records1, string filePath) {
        ofstream outputFile(filePath, std::ios::out | std::ios::app);  // Open the file for writing in binary mode
        if (!outputFile) {
            std::cerr << "Error opening file." << std::endl;
        } else {
            string tempp = "";
            for (GISRecord& record : records1) {
                tempp = record.dbPrint();
                outputFile << tempp << endl;
            }
        }
        outputFile.close();  // Close the file after writing
    }



    int CommandProcessor::readScript()
    {
        string myText;
        ifstream ScriptFile1("../Files/script01.txt");
        string file = "../Files/VA_Monterey.txt"; // + concatenated[1];
        string dbFile = "../Files/database.txt";
        int commandCounter = 1;

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
                        //run world            west              east            south             north
                        world1.createWorld(concatenated[1], concatenated[2], concatenated[3], concatenated[4]);
                        prquadtree = new PRQuadtree(world1.westLong, world1.eastLong, world1.southLat, world1.northLat); // Initialize PRQuadtree with world boundaries
                    } else if (command == "import") {
                        stringstream logMessage;
                        logMessage << "Command " << commandCounter << ": " << myText << endl;
                        Logger::getInstance().writeLog(logMessage.str());

                        // Uncomment lines to run import command
                        cout << "Import Command, file name: " << concatenated[1] << endl;

                        importCommand(file, dbFile);
                        commandCounter++;
                    } else if (command=="what_is") {
                        Logger::getInstance().writeLog(myText);
                        GISRecord* what_isThis = bufferPool1->whatIs(concatenated[1], concatenated[2], nameIndex);
                        if (what_isThis != nullptr) {
                            Logger::getInstance().writeLog(what_isThis->whatIsPrint());
                        } else {
                            Logger::getInstance().writeLog("No records match \""+ concatenated[1] + "\" and \""+ concatenated[2] + "\"");
                        }
                    }
                } else {
                    Logger::getInstance().writeLog(myText);
                }

            }
        }

        cout << "Printing tree: " << endl;
        prquadtree->displayPRQuadtree(prquadtree->root);

        ScriptFile1.close();
        return 0;
    }

} // GIS

