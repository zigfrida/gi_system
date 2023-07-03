//
// Created by dldms on 4/26/2023.
//

#include <iostream>
#include <sstream>
#include "World.h"
#include "Logger.h"
using namespace std;
namespace GIS {
    void World::createWorld(string westLong, string eastLong, string southLat, string northLat) {
        this->westLong = convertStringLatLongToInt(westLong);
        this->eastLong = convertStringLatLongToInt(eastLong);
        this->southLat = convertStringLatLongToInt(southLat);
        this->northLat = convertStringLatLongToInt(northLat);

        writeWorldToLog();
    }

    int World::convertStringLatLongToInt(string lat) {
        int result = 0;
        result = stoi(lat.substr(0, lat.length() - 1));
        if (lat[lat.length() - 1] == 'W' || lat[lat.length() - 1] == 'S') {
            result = result * (-1);
            return result;
        } else {
            return result;
        }
    }

    bool World::isItInWorldBoundary(string lat, string longitude) {
        int latitude = World::convertStringLatLongToInt(lat);
        int longi = World::convertStringLatLongToInt(longitude);

        if (latitude > this->northLat || latitude < this->southLat) {
            return false;
        }

        if (longi < this->westLong || longi > this->eastLong) {
            return false;
        }

        return true;
    }

    void World::writeWorldToLog() {
        stringstream logMessage, upSentence, middleSentence, lastSentence;
        logMessage << "world\t" << this->westLong << "\t" << this->eastLong << endl;
        Logger::getInstance().writeLog(logMessage.str());
        Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
        Logger::getInstance().writeLog("Latitude/longitude values in index entries are shown as signed integers, in total seconds.");
        Logger::getInstance().writeLog("------------------------------------------------------------------------------------------");
        Logger::getInstance().writeLog("\t\t\t\t\t\tWorld boundaries are set to:");
        upSentence << "\t\t\t\t\t\t           " <<  this->northLat;
        Logger::getInstance().writeLog(upSentence.str());
        middleSentence << "\t\t\t\t\t\t" << this->westLong << "              " << this->eastLong;
        Logger::getInstance().writeLog(middleSentence.str());
        lastSentence << "\t\t\t\t\t\t           " << this->southLat << endl;
        Logger::getInstance().writeLog(lastSentence.str());
    }
}
