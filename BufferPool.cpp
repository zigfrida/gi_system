//
// Created by Amanda Golubics on 2023-04-26.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "BufferPool.h"
using namespace std;

/**
 * Function to append record to the database.txt
 * @param records1
 * @param filePath
 */
void BufferPool::appendToDatabase(vector<GISRecord> records1, string filePath) {
    ofstream outputFile(filePath, std::ios::out | std::ios::app);  // Open the file for writing in binary mode
    if (!outputFile) {
        std::cerr << "Error opening file." << std::endl;
    } else {
        databaseFilePath = filePath;
        string tempp = "";
        for (GISRecord& record : records1) {
            tempp = record.dbPrint();
            outputFile << tempp << endl;
        }
    }
    outputFile.close();  // Close the file after writing
}

GISRecord BufferPool::whatIs(string name, string state) {
    GISRecord record3;
    for (size_t i = 0; i < buffer1.size(); ++i) {
        record3 = buffer1[i];
        if (record3.FEATURE_Name == name && record3.STATE_Abbreviation == state) {
            return record3;
        }
    }

    int resultIndex = fakeHashSearch(name, state);
    if (resultIndex > 0) {
        string line = getLineAtIndex(databaseFilePath, resultIndex);
        string feature;
        vector<string> featureInfo;
        istringstream iss(line);
        while (getline(iss, feature, '|')) {
            featureInfo.push_back(feature);
        }

        record3.FEATURE_ID = stoi(featureInfo[0]);
        record3.FEATURE_Name = featureInfo[1];
        record3.FEATURE_CLASS = featureInfo[2];
        record3.Latitude = stoi(featureInfo[4]);
        record3.longitude = stoi(featureInfo[5]);
        record3.STATE_Abbreviation = featureInfo[3];
        return record3;
    }

    return record3;
}

/**
 * This function simulates a hashtable search function. Replace it with real hash search function later.
 */
int BufferPool::fakeHashSearch(string name, string state) {
    vector<GISRecord> allRecords = readDatabaseFile(databaseFilePath);
    for (size_t i = 0; i < allRecords.size(); ++i) {
        const GISRecord& record = allRecords[i];
        if (record.FEATURE_Name == name && record.STATE_Abbreviation == state) {
            return i;  // Return the index of the first matching record
        }
    }
    return -1;
}

/**
 * Reads the entire database.txt.
 * This function is only for testing.
 * Should not be used for search or print.
 */
vector<GISRecord> BufferPool::readDatabaseFile(string filePath) {
    ifstream source(filePath);
    vector<GISRecord> dbRecords;
    if (!source.is_open()) {
        cout << "Error opening source file" << endl;
    } else {
        string line;

        while (getline(source, line)) {
            istringstream iss(line);
            string feature;
            vector<string> featureInfo;

                while (getline(iss, feature, '|')) {
                    featureInfo.push_back(feature);
                }

                GISRecord tempRec;
                tempRec.FEATURE_ID = stoi(featureInfo[0]);
                tempRec.FEATURE_Name = featureInfo[1];
                tempRec.FEATURE_CLASS = featureInfo[2];
                tempRec.Latitude = stoi(featureInfo[4]);
                tempRec.longitude = stoi(featureInfo[5]);
                tempRec.STATE_Abbreviation = featureInfo[3];
                dbRecords.push_back(tempRec);
        }
        source.close();
    }
    return dbRecords;
}

string BufferPool::getLineAtIndex(string& filename, size_t index) {
    ifstream inputFile(filename);
    string line;

    for (size_t i = 0; i < index; ++i) {
        if (!std::getline(inputFile, line)) {
            // If the file ends before reaching the desired line, return an empty string
            return "";
        }
    }

    std::getline(inputFile, line); // Retrieve the desired line

    inputFile.close();
    return line;
}
