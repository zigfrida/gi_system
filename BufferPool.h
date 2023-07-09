//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_BUFFERPOOL_H
#define GI_SYSTEM_BUFFERPOOL_H

#include <string>
#include <vector>
#include "GISRecord.h"
#include "HashTable.h"
#include "CoordinateIndex.h"
#include "World.h"
#include "PRQuadtree.h"

using namespace std;

class BufferPool {
public:
    BufferPool(string dbFilePath) {     // Constructor
        this->databaseFilePath = dbFilePath;
        /**
         * Clear things inside the database text file
         */
        ofstream outputFile22(databaseFilePath, std::ios::out);  // Open the file for writing in binary mode
        outputFile22 << "";
        outputFile22.close();
    }
    vector<GISRecord> buffer1;
    string databaseFilePath = "";
    vector<GISRecord> readDatabaseFile(string filePath);
    int fakeHashSearch(string name, string state);
    int fakeTreeSearch(int latitude, int longitude);
    vector<int> fakeTreeSearchArea(int latitude, int longitude, int latSpan, int longSpan);
    GISRecord* whatIs(string name, string state, GIS::HashTable* nameIndex);
    vector<GISRecord> whatIsAt(string latString, string longString, GIS::PRQuadtree* prQuadTree);
    vector<GISRecord> whatIsIn(string latString, string longString, string latSpanString, string longSpanString, string filter, GIS::PRQuadtree* prQuadTree);
    void whatIsInLogger(vector<GISRecord> records, string cord1, string cord2, string span1, string span2);
    void whatIsInLoggerOut(vector<GISRecord> records, string cord1, string cord2, string span1, string span2);
    string getLineAtIndex(string &filePath, size_t index);

    void insertToBuffer(GISRecord record);

    void bringToFrontOfBuffer(int index);
    void displayPool();
    void displayDebugPool();
    string featureClassType(string featureClass);
};


#endif //GI_SYSTEM_BUFFERPOOL_H
