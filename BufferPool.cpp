//
// Created by Amanda Golubics on 2023-04-26.
//

#include <fstream>
#include <iostream>
#include "BufferPool.h"
using namespace std;

vector<GISRecord> BufferPool::readDatabaseFile(string filePath) {

    // Read the vector data from the file
    std::ifstream inputFile(filePath, std::ios::in | std::ios::binary);  // Open the file for reading in binary mode
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
    }

    // Read the number of records from the file
    size_t readNumRecords;
    inputFile.read(reinterpret_cast<char*>(&readNumRecords), sizeof(readNumRecords));

    // Create a vector to store the read data
    std::vector<GISRecord> readRecords(readNumRecords);

    // Read each record from the file
    for (GISRecord& record : readRecords) {
        record.deserialize(inputFile);
    }

    inputFile.close();
    buffer1 = readRecords;
    return buffer1;
}

void BufferPool::appendToDatabase(vector<GISRecord> records1, string filePath) {
    ofstream outputFile(filePath, std::ios::out | std::ios::binary);  // Open the file for writing in binary mode
    if (!outputFile) {
        std::cerr << "Error opening file." << std::endl;
    }

    // Write the number of records to the file
    size_t numRecords = records1.size();
    outputFile.write(reinterpret_cast<const char*>(&numRecords), sizeof(numRecords));

    // Write each record to the file
    for (const GISRecord& record : records1) {
        record.serialize(outputFile);
    }

    outputFile.close();  // Close the file after writing
}
