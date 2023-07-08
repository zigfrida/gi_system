
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
        scriptFile = "../Files/script01.txt";
        dbFile = "../Files/database.txt";
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
//        ifstream source("../Files/VA_Monterey.txt");

        if (!source.is_open()) {
            Logger::getInstance().writeLog("Import failed. Import file " + recordFile + " does not exist!\n");
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
                        CoordinateIndex* newCordIndex = new CoordinateIndex(tempRec.latitude, tempRec.longitude);
                        newCordIndex->fileOffsets.push_back(lineOffSet);
                        prquadtree->insert(*newCordIndex, tempRec);
                        lineOffSet++;
                    }
                }
                if (firstLine) firstLine = false;
            }

            appendToDatabase(dbRecords, databaseFile);
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
            Logger::getInstance().writeLog("Database file does not exist and creating file at designated path did not work\n");
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
        ifstream ScriptFile1(scriptFile);

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
                        Logger::getInstance().writeCommandCount(myText);
                        importCommand(concatenated[1], dbFile);
                    } else if (command == "what_is") {
                        Logger::getInstance().writeCommandCount(myText);
                        GISRecord* what_isThis;
                        what_isThis = nullptr;
                        try {
                            what_isThis = bufferPool1->whatIs(concatenated[1], concatenated[2], nameIndex);
                        } catch (exception e) {
                            Logger::getInstance().writeLog("Invalid command argument!");
                            continue;
                        }

                        if (what_isThis != nullptr) {
                            Logger::getInstance().writeLog("\t" + what_isThis->whatIsPrint());
                            Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");

                        } else {
                            Logger::getInstance().writeLog("\tNo records match \""+ concatenated[1] + "\" and \""+ concatenated[2] + "\"");
                            Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
                        }
                    }  else if (command=="what_is_at") {
                        Logger::getInstance().writeCommandCount(myText);
                        vector<GISRecord> what_isAt;
                        what_isAt.clear();
                        try {
                            what_isAt = bufferPool1->whatIsAt(concatenated[1], concatenated[2], prquadtree);
                        } catch (exception e) {
                            Logger::getInstance().writeLog("Invalid command argument!");
                            continue;
                        }

                        if (!what_isAt.empty()) {
                            Logger::getInstance().writeLog("\tThe following feature(s) were found at (" + concatenated[1] + ", " + concatenated[2] + ")");
                            string whatIsAtResult = "";
                            for (auto rec : what_isAt) {
                                whatIsAtResult += "\t\t" + rec.whatIsAtPrint();
                            }
                            Logger::getInstance().writeLog(whatIsAtResult);
                            Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
                        } else {
                            Logger::getInstance().writeLog("No feature at \""+ concatenated[1] + "\" and \""+ concatenated[2] + "\"");
                            Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
                        }
                    } else if (command=="what_is_in") {
                        Logger::getInstance().writeCommandCount(myText);
                        vector<GISRecord> what_Is_In;
                        what_Is_In.clear();
                        try {
                            if (concatenated[1] == "-long") {
                                what_Is_In = bufferPool1->whatIsIn(concatenated[2], concatenated[3], concatenated[4], concatenated[5], "-long", prquadtree);
                            } else if (concatenated[1] == "-filter") {
                                what_Is_In = bufferPool1->whatIsIn(concatenated[3], concatenated[4], concatenated[5], concatenated[6], concatenated[2], prquadtree);
                            } else {
                                what_Is_In = bufferPool1->whatIsIn(concatenated[1], concatenated[2], concatenated[3], concatenated[4], "", prquadtree);
                            }


                        } catch (exception e) {
                            Logger::getInstance().writeLog("Invalid command argument!");
                            continue;
                        }
                        if (!what_Is_In.empty()) {
                            Logger::getInstance().writeLog("\tThe following feature(s) were found at (" + concatenated[1] + ", " + concatenated[2] + ")");
                            string whatIsInResult = "";
                            for (auto rec : what_Is_In) {
                                whatIsInResult += "\t\t" + rec.whatIsAtPrint() + "\n";
                            }
                            Logger::getInstance().writeLog(whatIsInResult);
                            Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
                        } else {

                            if (concatenated[1] == "-long") {
                                Logger::getInstance().writeLog("\tNo feature at \""+ concatenated[2] + "\" and \""+ concatenated[3] + "\"");
                            } else if (concatenated[1] == "-filter") {
                                Logger::getInstance().writeLog("\tNo feature at \""+ concatenated[3] + "\" and \""+ concatenated[4] + "\"");
                            } else {
                                Logger::getInstance().writeLog("\tNo feature at \""+ concatenated[1] + "\" and \""+ concatenated[2] + "\"");
                            }
                            Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
                        }
                    } else if (command == "debug") {
                        string debugCommand = concatenated[1];
                        if (debugCommand == "world") {
                            Logger::getInstance().writeCommandCount(myText);

                        } else if (debugCommand == "hash") {
                            Logger::getInstance().writeCommandCount(myText);
                            nameIndex->displayDebugHashTable();
                        } else if (debugCommand == "quad") {
                            Logger::getInstance().writeCommandCount(myText);
                            prquadtree->displayDebugPRQuadtree(prquadtree->root);
                        } else if (debugCommand == "pool") {
                            Logger::getInstance().writeCommandCount(myText);
                            bufferPool1->displayDebugPool();
                        }
                    } else if (command == "quit") {
                        Logger::getInstance().quitCommand();
                        return 0;
                    }

                }
            }
        }

        ScriptFile1.close();
        return 0;
    }

} // GIS

