//
// Created by Amanda Golubics on 2023-04-26.
//

#include <fstream>
#include <iostream>
#include "BufferPool.h"
using namespace std;

/**
 * This is not a real buffer pool just for testing.
 * @param filePath
 * @return
 */
vector<GISRecord> BufferPool::readDatabaseFile(string filePath) {


}

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
        string tempp = "";
        for (GISRecord& record : records1) {
            tempp = record.dbPrint();
            outputFile << tempp << endl;
        }
    }
    outputFile.close();  // Close the file after writing
}
