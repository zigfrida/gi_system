//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_GISRECORD_H
#define GI_SYSTEM_GISRECORD_H

#include <string>

using namespace std;

class GISRecord {
public:
    int FEATURE_ID;
    string FEATURE_Name;
    string FEATURE_CLASS;
    string STATE_Abbreviation;
    int Latitude;
    int longitude;
    //FEATURE_ID|FEATURE_NAME|FEATURE_CLASS|STATE_ALPHA|STATE_NUMERIC|COUNTY_NAME|COUNTY_NUMERIC|PRIMARY_LAT_DMS|PRIM_LONG_DMS|PRIM_LAT_DEC|PRIM_LONG_DEC|SOURCE_LAT_DMS|SOURCE_LONG_DMS|SOURCE_LAT_DEC|SOURCE_LONG_DEC|ELEV_IN_M|ELEV_IN_FT|MAP_NAME|DATE_CREATED|DATE_EDITED
    //1481240|Allegheny Mountain Trail|Trail|VA|51|Bath|017|375754N|0800225W|37.9651236|-80.0403391|||||891|2923|Rucker Gap|09/28/1979|
};


#endif //GI_SYSTEM_GISRECORD_H
