//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_GISRECORD_H
#define GI_SYSTEM_GISRECORD_H

#include <string>
#include <fstream>
using namespace std;

class GISRecord {
public:
    int FEATURE_ID;
    string FEATURE_Name;
    string FEATURE_CLASS;
    string STATE_Abbreviation;
    int Latitude;
    int longitude;
    // Serialization function to write GISRecord to a binary file
    void serialize(std::ofstream& file) const;

    // Deserialization function to read GISRecord from a binary file
    void deserialize(std::ifstream& file);

    // Helper function to write a string to the binary file
    void writeString(std::ofstream& file, const std::string& str) const;

    // Helper function to read a string from the binary file
    std::string readString(std::ifstream& file) const;
};


#endif //GI_SYSTEM_GISRECORD_H
