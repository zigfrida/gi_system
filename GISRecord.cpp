//
// Created by Amanda Golubics on 2023-04-26.
//

#include "GISRecord.h"

string GISRecord::dbPrint() {
    string result = "";
    result += to_string(this->FEATURE_ID);
    result += "|";
    result += this->FEATURE_Name;
    result += "|";
    result += this->FEATURE_CLASS;
    result += "|";
    result += this->STATE_Abbreviation;
    result += "|";
    result += to_string(this->Latitude) ;
    result += "|";
    result += to_string(this->longitude);
    result += "|";
    return result;
}
