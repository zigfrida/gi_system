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
    //Print function used for DB append
    string dbPrint();

};


#endif //GI_SYSTEM_GISRECORD_H
