//
// Created by Amanda Golubics on 2023-04-26.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "BufferPool.h"
using namespace std;

GISRecord* BufferPool::whatIs(string name, string state, GIS::HashTable* nameIndex) {
    GISRecord* record3 = nullptr;
    record3 = new GISRecord;
    for (size_t i = 0; i < buffer1.size(); ++i) {
        if (buffer1[i].FEATURE_Name == name && buffer1[i].STATE_Abbreviation == state) {
            bringToFrontOfBuffer(i);
            return &buffer1[i];
        }
    }

    int resultIndex = stoi(nameIndex->search(name + " " + state));
    if (resultIndex > 0) {
        string line = getLineAtIndex(databaseFilePath, resultIndex);
        string feature;
        vector<string> featureInfo;
        istringstream iss(line);
        while (getline(iss, feature, '|')) {
            featureInfo.push_back(feature);
        }

        record3->FEATURE_ID = stoi(featureInfo[0]);
        record3->FEATURE_Name = featureInfo[1];
        record3->FEATURE_CLASS = featureInfo[2];
        record3->latitude = stoi(featureInfo[4]);
        record3->longitude = stoi(featureInfo[5]);
        record3->STATE_Abbreviation = featureInfo[3];
        record3->COUNTY_NAME = featureInfo[6];
        insertToBuffer(*record3);
        return record3;
    }

    return nullptr;
}

GISRecord* BufferPool::whatIsAt(string latString, string longString, GIS::PRQuadtree* prQuadTree) {
    GISRecord* record3 = nullptr;
    record3 = new GISRecord();
    int latitude = GIS::World::convertStringLatLongToInt(latString);
    int longitude = GIS::World::convertStringLatLongToInt(longString);
    for (size_t i = 0; i < buffer1.size(); ++i) {
        if (buffer1[i].latitude == latitude && buffer1[i].longitude == longitude) {
            bringToFrontOfBuffer(i);
            return &buffer1[i];
        }
    }

    int resultIndex = fakeTreeSearch(latitude, longitude);
    if (resultIndex > 0) {
        string line = getLineAtIndex(databaseFilePath, resultIndex);
        string feature;
        vector<string> featureInfo;
        istringstream iss(line);
        while (getline(iss, feature, '|')) {
            featureInfo.push_back(feature);
        }

        record3->FEATURE_ID = stoi(featureInfo[0]);
        record3->FEATURE_Name = featureInfo[1];
        record3->FEATURE_CLASS = featureInfo[2];
        record3->latitude = stoi(featureInfo[4]);
        record3->longitude = stoi(featureInfo[5]);
        record3->STATE_Abbreviation = featureInfo[3];
        record3->COUNTY_NAME = featureInfo[6];
        insertToBuffer(*record3);
        return record3;
    }

    return nullptr;
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
 * This function simulates a hashtable search function. Replace it with real hash search function later.
 */
int BufferPool::fakeTreeSearch(int latitude, int longitude) {
    vector<GISRecord> allRecords = readDatabaseFile(databaseFilePath);
    for (size_t i = 0; i < allRecords.size(); ++i) {
        const GISRecord& record = allRecords[i];
        if (record.longitude == longitude && record.latitude == latitude) {
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
                tempRec.latitude = stoi(featureInfo[4]);
                tempRec.longitude = stoi(featureInfo[5]);
                tempRec.STATE_Abbreviation = featureInfo[3];
                dbRecords.push_back(tempRec);
        }
        source.close();
    }
    return dbRecords;
}

/**
 * This function finds a specific record at index in the database.txt
 * @param filename
 * @param index
 * @return
 */
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

void BufferPool::insertToBuffer(GISRecord record) {
    if (buffer1.empty()) {
        buffer1.push_back(record);
    } else if (buffer1.size() >= 15) {
        buffer1.insert(buffer1.begin(), record);
        buffer1.pop_back();
    } else {
        buffer1.insert(buffer1.begin(), record);
    }
}

void BufferPool::bringToFrontOfBuffer(int index) {
    if (index >= buffer1.size()) {
        std::cout << "Invalid index." << std::endl;
        return;
    }

    GISRecord rec = buffer1[index];
    buffer1.erase(buffer1.begin() + index);
    buffer1.insert(buffer1.begin(), rec);
}
