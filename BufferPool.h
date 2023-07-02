//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_BUFFERPOOL_H
#define GI_SYSTEM_BUFFERPOOL_H

#include <string>
#include <vector>
#include "GISRecord.h"
#include "HashTable.h"

using namespace std;

class BufferPool {
public:
    BufferPool(string dbFilePath) {     // Constructor
        this->databaseFilePath = dbFilePath;
    }
    vector<GISRecord> buffer1;
    string databaseFilePath = "";
    vector<GISRecord> readDatabaseFile(string filePath);
    int fakeHashSearch(string name, string state);
    GISRecord* whatIs(string name, string state, GIS::HashTable* nameIndex);

    string getLineAtIndex(string &filePath, size_t index);

    void insertToBuffer(GISRecord record);

    void bringToFrontOfBuffer(int index);
};


#endif //GI_SYSTEM_BUFFERPOOL_H
