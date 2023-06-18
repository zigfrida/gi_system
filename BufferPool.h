//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_BUFFERPOOL_H
#define GI_SYSTEM_BUFFERPOOL_H

#include <string>
#include <vector>
#include "GISRecord.h"

using namespace std;

class BufferPool {
public:
    vector<GISRecord> buffer1;
    vector<GISRecord> readDatabaseFile(string filePath);
    void appendToDatabase(vector<GISRecord> records1, string filePath);

};


#endif //GI_SYSTEM_BUFFERPOOL_H
