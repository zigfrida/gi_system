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
    result += to_string(this->latitude) ;
    result += "|";
    result += to_string(this->longitude);
    result += "|";
    result += this->COUNTY_NAME;
    result += "|";
    result += this->elev_in_ft;
    result += "|";
    result += this->date_created;
    result += "|";
    return result;
}

string GISRecord::whatIsPrint() {
    //Format example: Highland  (38d 29m 53s North, 79d 33m 23s West)
    string result = to_string(this->lineOfSet) + ": " + this->COUNTY_NAME + " (";

    result += convertToDMS(this->latitude) + ", ";
    result += convertToDMS2(this->longitude) + ")";
    return result;
}

string GISRecord::whatIsAtPrint() {
    //Format example: Highland  (38d 29m 53s North, 79d 33m 23s West)
    string result = "";
    result += "" + to_string(this->lineOfSet) + ":  ";
    result += "\"" + this->FEATURE_Name + "\" \"";
    result += this->COUNTY_NAME + "\" \"";
    result += this->STATE_Abbreviation + "\"";
    return result;
}

string GISRecord::whatIsInPrint() {
    /**
     * Format example:
     * 9:  "Elk Run"  "VA"  "(38d 29m 36s North, 79d 31m 53s West)
     */
    string result = "";
    result += "" + to_string(this->lineOfSet) + ":  ";
    result += "\"" + this->FEATURE_Name + "\" \"";
    result += this->COUNTY_NAME + "\" \"";
    result += this->STATE_Abbreviation + "\"";
    return result;
}

string GISRecord::whatIsInPrintLong() {
    //Format example: Highland  (38d 29m 53s North, 79d 33m 23s West)
    /**
     *   Feature ID   : 1482110
          Feature Name : Buck Hill
          Feature Cat  : Summit
          State        : VA
          County       : Highland
          Longitude    : 79d 33m 58s West
          Latitude     : 38d 19m 2s North
          Elev in ft   : 3291
          USGS Quad    : Monterey SE
          Date created : 09/28/1979
    */
    string result = "    /**\n";
    result += "        Feature ID   : " + to_string(this->lineOfSet) + "\n";
    result += "        Feature Name : " + this->FEATURE_Name + "\n";
    result += "        Feature Class : " + this->FEATURE_CLASS + "\n";
    result += "        State        : " + this->STATE_Abbreviation + "\n";
    result += "        County       : " + this->COUNTY_NAME + "\n";
    result += "        Longitude    : " + this->convertToDMS2(this->longitude) + " \n";
    result += "        Latitude     : " + this->convertToDMS(this->latitude) + " \n";
    result += "        Line Offset  : " + to_string(this->lineOfSet) + "\n";
    result += "     \n";
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

