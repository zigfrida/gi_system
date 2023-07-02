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
    result += this->COUNTY_NAME;
    result += "|";
    return result;
}

string GISRecord::whatIsPrint() {
    //Format example: Highland  (38d 29m 53s North, 79d 33m 23s West)
    string result = this->COUNTY_NAME + " (";

    result += convertToDMS(this->Latitude) + ", ";
    result += convertToDMS2(this->longitude) + ")";
    return result;
}

string GISRecord::convertToDMS(int value) {
    string direction = (value >= 0) ? "North" : "South";

    value = abs(value);

    int degrees = value / 10000;
    int remaining = value % 10000;

    int minutes = remaining / 100;
    int seconds = remaining % 100;

    string result = to_string(degrees) + "d "
                    + to_string(minutes) + "m "
                    + to_string(seconds) + "s "
                    + direction;

    return result;
}

string GISRecord::convertToDMS2(int value) {
    string direction = (value >= 0) ? "East" : "West";

    value = abs(value);

    int degrees = value / 10000;
    int remaining = value % 10000;

    int minutes = remaining / 100;
    int seconds = remaining % 100;

    string result = to_string(degrees) + "d "
                    + to_string(minutes) + "m "
                    + to_string(seconds) + "s "
                    + direction;

    return result;
}

