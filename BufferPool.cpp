//
// Created by Amanda Golubics on 2023-04-26.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "BufferPool.h"
#include "Logger.h"

using namespace std;

GISRecord* BufferPool::whatIs(string name, string state, GIS::HashTable* nameIndex) {
    GISRecord* record3 = nullptr;
    record3 = new GISRecord;
    for (size_t i = 0; i < buffer1.size(); ++i) {
        if (buffer1[i].FEATURE_Name == name && buffer1[i].STATE_Abbreviation == state) {
            bringToFrontOfBuffer(i);
            return &buffer1.front();
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
        record3->lineOfSet = resultIndex;
        insertToBuffer(*record3);
        return record3;
    }

    return nullptr;
}

vector<GISRecord> BufferPool::whatIsAt(string latString, string longString, GIS::PRQuadtree* prQuadTree) {
    vector<GISRecord> whatIsAtList;
    whatIsAtList.clear();
    int latitude = GIS::World::convertStringLatLongToInt(latString);
    int longitude = GIS::World::convertStringLatLongToInt(longString);
    for (size_t i = 0; i < buffer1.size(); ++i) {
        if (buffer1[i].latitude == latitude && buffer1[i].longitude == longitude) {
            whatIsAtList.insert(whatIsAtList.begin(), buffer1[i]);
            bringToFrontOfBuffer(i);
        }
    }

    vector<int> resultIndexList = prQuadTree->searchOne(latitude, longitude);
    bool alreadyAdded = false;
    for (int resultIndex : resultIndexList) {
        alreadyAdded = false;
        for (const GISRecord& recc : whatIsAtList) {
            if (recc.lineOfSet == resultIndex) {
                alreadyAdded = true;
            }
        }
        if (resultIndex > 0 && !alreadyAdded) {
            GISRecord* record3;
            record3 = nullptr;
            record3 = new GISRecord();
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
            record3->lineOfSet = resultIndex;
            insertToBuffer(*record3);
            whatIsAtList.insert(whatIsAtList.begin(), *record3);
        }
    }

    return whatIsAtList;
}

vector<GISRecord> BufferPool::whatIsIn(string latString, string longString, string latSpanString, string longSpanString, string filter, GIS::PRQuadtree* prQuadTree) {
    vector<GISRecord> whatIsInList;
    whatIsInList.clear();
    int latitude = GIS::World::convertStringLatLongToInt(latString);
    int longitude = GIS::World::convertStringLatLongToInt(longString);
    int latSpan = stoi(latSpanString);
    int longSpan = stoi(longSpanString);

    bool isAll = (filter == "" || filter == "-long");

    for (size_t i = 0; i < buffer1.size(); ++i) {
        if (buffer1[i].latitude <= latitude + latSpan
        && buffer1[i].latitude >= latitude - latSpan
        && buffer1[i].longitude <= longitude + longSpan
        && buffer1[i].longitude >= longitude - longSpan
        && (isAll || featureClassType(buffer1[i].FEATURE_CLASS) == filter )) {
            whatIsInList.insert(whatIsInList.begin(), buffer1[i]);
            bringToFrontOfBuffer(i);
        }
    }

    vector<int> resultIndexList;
    resultIndexList.clear();
    resultIndexList = prQuadTree->treeSearchArea(latitude, longitude, latSpan, longSpan);
    bool alreadyAdded = false;
    for (int resultIndex : resultIndexList) {
        alreadyAdded = false;
        for (const GISRecord& recc : whatIsInList) {
            if (recc.lineOfSet == resultIndex) {
                alreadyAdded = true;
            }
        }
        if (resultIndex > 0 && !alreadyAdded) {
            GISRecord* record3;
            record3 = nullptr;
            record3 = new GISRecord();
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
            record3->elev_in_ft = featureInfo[7];
            record3->date_created = featureInfo[8];
            record3->lineOfSet = resultIndex;
            if (isAll || featureClassType(record3->FEATURE_CLASS) == filter ) {
                insertToBuffer(*record3);
                whatIsInList.insert(whatIsInList.begin(), *record3);
            }
        }
    }

    return whatIsInList;
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
 * This function simulates a hashtable search function. Replace it with real hash search function later.
 */
vector<int> BufferPool::fakeTreeSearchArea(int latitude, int longitude, int latSpan, int longSpan) {
    vector<GISRecord> allRecords = readDatabaseFile(databaseFilePath);
    vector<int> recordsInArea;
    recordsInArea.clear();
    for (size_t i = 0; i < allRecords.size(); ++i) {
        GISRecord& record = allRecords[i];
        if (record.latitude <= latitude + latSpan
            && record.latitude >= latitude - latSpan
            && record.longitude <= longitude + longSpan
            && record.longitude >= longitude - longSpan) {
            recordsInArea.insert(recordsInArea.begin(), i);
        }
    }
    return recordsInArea;
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
        int lineCount = 0;

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
                tempRec.COUNTY_NAME = featureInfo[6];
                tempRec.elev_in_ft = featureInfo[7];
                tempRec.date_created = featureInfo[8];
                tempRec.lineOfSet = lineCount;
                dbRecords.push_back(tempRec);

                lineCount++;
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
        return;
    }

    GISRecord rec = buffer1[index];
    buffer1.erase(buffer1.begin() + index);
    buffer1.insert(buffer1.begin(), rec);
}

void BufferPool::displayPool() {
    cout << "MRU" << endl;
    for(auto feature : buffer1) {
        cout << "\t" << feature.lineOfSet << ": " << feature.FEATURE_ID << "|" << feature.FEATURE_Name << "|" << feature.FEATURE_CLASS << "|" << feature.STATE_Abbreviation << "|" << feature.COUNTY_NAME << "|" << feature.latitude << "|" << feature.longitude << "|" << endl;
    }
    cout << "LRU" << endl;
}

void BufferPool::displayDebugPool() {
    stringstream logMessage;
    logMessage << "MRU" << endl;
    for(auto feature : buffer1) {
        logMessage << "\t" << feature.lineOfSet << ": " << feature.FEATURE_ID << "|" << feature.FEATURE_Name << "|" << feature.FEATURE_CLASS << "|" << feature.STATE_Abbreviation << "|" << feature.COUNTY_NAME << "|" << feature.latitude << "|" << feature.longitude << "|" << endl;
    }
    logMessage << "LRU" << endl;
    logMessage << "------------------------------------------------------------------------------------------";
    GIS::Logger::getInstance().writeLog(logMessage.str());
}

string BufferPool::featureClassType(string featureClass) {
    if (
            featureClass == "Airport" ||
            featureClass == "Bridge" ||
                    featureClass == "Building" ||
                    featureClass == "Church" ||
                    featureClass == "Dam" ||
                    featureClass == "Hospital" ||
                    featureClass == "Levee" ||
                    featureClass == "Park" ||
                    featureClass == "Post Office" ||
                    featureClass == "School" ||
                    featureClass == "Tower" ||
                    featureClass == "Tunnel"
            ) {
        return "structure";
    }

    if (
            featureClass == "Falls" ||
            featureClass == "Glacier" ||
            featureClass == "Gut" ||
            featureClass == "Harbor" ||
            featureClass == "Lake" ||
            featureClass == "Rapids" ||
            featureClass == "Reservoir"||
            featureClass == "Sea"||
            featureClass == "Spring"||
            featureClass == "Stream"||
            featureClass == "Swamp"||
            featureClass == "Well"

            ) {
        return "water";
    }

    if (
            featureClass == "Populated Place"
            ) {
        return "pop";
    }
    return "";
}

void BufferPool::whatIsInLogger(vector<GISRecord> records, string cord1, string cord2, string span1, string span2) {
    stringstream logMessage;

    logMessage << "\tThe following " << records.size() << " feature(s) were found in " << cord1 << " +/- " << span1 << ", " << cord2 << " +/- " << span2  << endl;

    for (auto rec : records) {
        logMessage << "\n\tFeature ID   :" << rec.FEATURE_ID << endl;
        logMessage << "\tFeature Name :" << rec.FEATURE_Name << endl;
        logMessage << "\tFeature Cat  :" << rec.FEATURE_CLASS << endl;
        logMessage << "\tState        :" << rec.STATE_Abbreviation << endl;
        logMessage << "\tCounty       :" << rec.COUNTY_NAME << endl;
        logMessage << "\tLongitude    :" << rec.longitude << endl;
        logMessage << "\tLatitude     :" << rec.latitude << endl;
        logMessage << "\tElev in ft   :" << rec.elev_in_ft << endl;
        logMessage << "\tDate created :" << rec.date_created << endl;
    }

    logMessage << endl;
    logMessage << "------------------------------------------------------------------------------------------";
    GIS::Logger::getInstance().writeLog(logMessage.str());

}

void BufferPool::whatIsInLoggerOut(vector<GISRecord> records, string cord1, string cord2, string span1, string span2) {

    cout << "The following " << records.size() << " feature(s) were found in " << cord1 << " +/- " << span1 << ", " << cord2 << " +/- " << span2  << endl;

    for (auto rec : records) {
        cout << "\n\tFeature ID   :" << rec.FEATURE_ID << endl;
        cout << "\tFeature Name :" << rec.FEATURE_Name << endl;
        cout << "\tFeature Cat  :" << rec.FEATURE_CLASS << endl;
        cout << "\tState        :" << rec.STATE_Abbreviation << endl;
        cout << "\tCounty       :" << rec.COUNTY_NAME << endl;
        cout << "\tLongitude    :" << rec.longitude << endl;
        cout << "\tLatitude     :" << rec.latitude << endl;
        cout << "\tElev in ft   :" << rec.elev_in_ft << endl;
        cout << "\tDate created :" << rec.date_created << endl;
    }

    cout << endl;

}
