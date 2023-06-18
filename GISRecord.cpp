//
// Created by Amanda Golubics on 2023-04-26.
//

#include "GISRecord.h"

void GISRecord::serialize(ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&FEATURE_ID), sizeof(FEATURE_ID));
    writeString(file, FEATURE_Name);
    writeString(file, FEATURE_CLASS);
    writeString(file, STATE_Abbreviation);
    file.write(reinterpret_cast<const char*>(&Latitude), sizeof(Latitude));
    file.write(reinterpret_cast<const char*>(&longitude), sizeof(longitude));
}

void GISRecord::writeString(ofstream &file, const string &str) const {
    size_t length = str.length();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(str.data(), length);
}

std::string GISRecord::readString(ifstream &file) const {
    size_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    std::string str(length, '\0');
    file.read(const_cast<char *>(str.data()), length);
    return str;
}

void GISRecord::deserialize(ifstream &file) {
    file.read(reinterpret_cast<char*>(&FEATURE_ID), sizeof(FEATURE_ID));
    FEATURE_Name = readString(file);
    FEATURE_CLASS = readString(file);
    STATE_Abbreviation = readString(file);
    file.read(reinterpret_cast<char*>(&Latitude), sizeof(Latitude));
    file.read(reinterpret_cast<char*>(&longitude), sizeof(longitude));
}
