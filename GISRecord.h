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
    int latitude;
    int longitude;
    string COUNTY_NAME;
    int lineOfSet;
    string usgs_quad;
    string elev_in_ft;
    string date_created;
    //Print function used for DB append
    string dbPrint();
    string whatIsPrint();
    string whatIsAtPrint();
    string convertToDMS(int value);
    string convertToDMS2(int value);

};


#endif //GI_SYSTEM_GISRECORD_H
