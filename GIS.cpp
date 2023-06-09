#include <iostream>
#include <string>
#include "CommandProcessor.h"
#include "Logger.h"

using namespace GIS;
using namespace std;

void getUserInput(CommandProcessor* commandprocessor1) {
    string line;
    while (true) {
        cout << "Enter a command (or 'quit' to exit): ";
        getline(cin, line);

        if (line == "quit") break;

        vector<string> tokens;
        commandprocessor1->tokenizeQuotes(line,' ', tokens);
        string command = tokens[0];

        if (command == "what_is") {
            if (tokens.size() == 3) {
                string featureName = tokens[1];
                string stateAbbreviation = tokens[2];
                GISRecord* what_isThis = nullptr;
                try {
                    what_isThis = commandprocessor1->bufferPool1->whatIs(featureName, stateAbbreviation, commandprocessor1->nameIndex);
                } catch (exception e) {
                    cout << "Invalid command argument!\n" << endl;
                    cout << "------------------------------------------------------------------------------------------" << endl;
                    continue;
                }
                if (what_isThis != nullptr) {
                    cout << what_isThis->whatIsAtPrint() << endl;
                    cout << "------------------------------------------------------------------------------------------" << endl;
                } else {
                    cout << "No records match " << tokens[1] << " " << tokens[2] << endl;
                    cout << "\n------------------------------------------------------------------------------------------" << endl;
                }
            } else {
                cout << "\nInvalid number of arguments for 'what_is' command." << endl;
                cout << "Ensure Feature Names are in quotations. Example what_is \"Central Church\" VA" << endl;
                cout << "\n------------------------------------------------------------------------------------------" << endl;
            }
        } else if (command == "what_is_at") {
            if (tokens.size() == 3) {
                string featureName = tokens[1];
                string stateAbbreviation = tokens[2];
                vector<GISRecord> what_isAt;
                what_isAt.clear();
                try {
                    what_isAt = commandprocessor1->bufferPool1->whatIsAt(tokens[1], tokens[2], commandprocessor1->prquadtree);
                } catch (exception e) {
                    cout << "Invalid arguments!" << endl;
                    cout << "------------------------------------------------------------------------------------------" << endl;
                    continue;
                }
                if (!what_isAt.empty()) {
                    cout << "The following feature(s) were found at (" << tokens[1] << ", " << tokens[2] + ")\n" << endl;
                    string whatIsAtResult = "";
                    for (auto rec : what_isAt) {
                        whatIsAtResult += "\t" + rec.whatIsAtPrint() + "\n";
                    }
                    cout << whatIsAtResult << endl;
                    cout << "------------------------------------------------------------------------------------------" << endl;
                } else {
                    cout << "No feature at " << tokens[1] << " " << tokens[2] << endl;
                    cout << "------------------------------------------------------------------------------------------" << endl;
                }
            } else {
                cout << "\nInvalid number of arguments for 'what_is_at' command." << endl;
                cout << "\n------------------------------------------------------------------------------------------" << endl;
            }
        } else if (command == "what_is_in") {
            if (tokens.size() >= 5) {
                string lat1 = tokens[1];
                string lat2 = tokens[2];
                string lat3 = tokens[3];
                string lat4 = tokens[4];
                string lat5 = tokens[5];
                vector<GISRecord> what_Is_In;
                what_Is_In.clear();
                try {
                    if (tokens[1] == "-long") {
                        what_Is_In = commandprocessor1->bufferPool1->whatIsIn(tokens[2], tokens[3], tokens[4], tokens[5], "-long", commandprocessor1->prquadtree);
                    } else if (tokens[1] == "-filter") {
                        what_Is_In = commandprocessor1->bufferPool1->whatIsIn(tokens[3], tokens[4], tokens[5], tokens[6], tokens[2], commandprocessor1->prquadtree);
                    } else {
                        what_Is_In = commandprocessor1->bufferPool1->whatIsIn(tokens[1], tokens[2], tokens[3], tokens[4], "", commandprocessor1->prquadtree);
                    }
                } catch (exception e) {
                    cout << "Invalid arguments provided." << endl;
                    cout << "------------------------------------------------------------------------------------------" << endl;
                    continue;
                }
                if (!what_Is_In.empty()) {
                    if (tokens[1] == "-long") {
                        commandprocessor1->bufferPool1->whatIsInLoggerOut(what_Is_In, tokens[2], tokens[3], tokens[4], tokens[5]);
                        cout << "------------------------------------------------------------------------------------------\n" << endl;
                    } else {
                        if (tokens[1] == "-filter") {
                            cout << "The following " << to_string(what_Is_In.size()) << " feature(s) were found at (" << tokens[3] << " +/- " << tokens[5] << ", " << tokens[4] << " +/- " << tokens[6] << ")\n" << endl;
                        } else {
                            cout << "The following " << to_string(what_Is_In.size()) << " feature(s) were found at (" << tokens[1] << " +/- " << tokens[3] << ", " << tokens[2] <<  " +/- " << tokens[4] << ")\n" << endl;
                        }
                        string whatIsInResult = "";
                        for (auto rec: what_Is_In) {
                            whatIsInResult += "\t" + rec.whatIsInPrint() + "\n";
                        }
                        cout << whatIsInResult << endl;
                        cout << "------------------------------------------------------------------------------------------\n" << endl;
                    }
                } else {
                    if (tokens[1] == "-long") {
                        cout << "No feature at \"" << tokens[2] << "\" and \"" << tokens[3] << "\"";
                    } else if (tokens[1] == "-filter") {
                        cout << "No feature at \"" << tokens[3] << "\" and \"" << tokens[4] << "\"";
                    } else {
                        cout << "No feature at \"" << tokens[1] << "\" and \"" << tokens[2] << "\"";
                    }
                    cout << "------------------------------------------------------------------------------------------\n" << endl;
                }
            } else {
                cout << "Invalid number of arguments for 'what_is_in' command.\n" << endl;
                cout << "------------------------------------------------------------------------------------------" << endl;
            }
        } else if (command == "debug") {
            if (tokens.size() >= 2) {
                string debugCommand = tokens[1];
                if (debugCommand == "world") {

                } else if (debugCommand == "hash") {
                    commandprocessor1->nameIndex->displayHashTable();
                } else if (debugCommand == "quad") {
                    commandprocessor1->prquadtree->displayPRQuadtree(commandprocessor1->prquadtree->root);
                } else if (debugCommand == "pool") {
                    commandprocessor1->bufferPool1->displayPool();
                }
                cout << "------------------------------------------------------------------------------------------\n" << endl;
            } else {
                cout << "Invalid number of arguments for 'debug' command.\n" << endl;
                cout << "------------------------------------------------------------------------------------------" << endl;
            }
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
}

/**
 *
Commandline execution example
 g++ GIS.cpp CommandProcessor.cpp World.cpp HashTable.cpp BufferPool.cpp GISRecord.cpp Logger.cpp PRQuadtree.cpp -o GIS
GIS ./Files/database.txt ./Files/script01.txt ./Files/log.txt
 */
int main(int argc, char* argv[]) {

    CommandProcessor* commandprocessor = new CommandProcessor(argv[1], argv[2], argv[3]);
    if (argc != 4) {
        return 0;
    } else {
        commandprocessor->readScript();
        getUserInput(commandprocessor);
    }

    Logger::closeLogger();
    return 0;
}
