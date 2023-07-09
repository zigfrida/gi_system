//
// Created by dldms on 4/26/2023.
//

#include <iostream>
#include <sstream>
#include "World.h"
#include "Logger.h"
#include "PRQuadtree.h"

using namespace std;
namespace GIS {
    void World::createWorld(string westLong, string eastLong, string southLat, string northLat) {
        this->westLong = convertStringLatLongToInt(westLong);
        this->eastLong = convertStringLatLongToInt(eastLong);
        this->southLat = convertStringLatLongToInt(southLat);
        this->northLat = convertStringLatLongToInt(northLat);

        writeWorldToLog(westLong, eastLong, southLat, northLat);
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

    void World::writeWorldToLog(string westLong, string eastLong, string southLat, string northLat) {
        stringstream logMessage;
        logMessage << endl << "world\t" << westLong << "\t" << eastLong << "\t" << southLat << "\t" << northLat << endl;
        logMessage << "------------------------------------------------------------------------------------------" << endl;
        logMessage << "Latitude/longitude values in index entries are shown as signed integers, in total seconds." << endl;
        logMessage << "------------------------------------------------------------------------------------------" << endl;
        logMessage << "\t\t\t\t\t\tWorld boundaries are set to:" << endl;
        logMessage << "\t\t\t\t\t\t           " <<  this->northLat << endl;
        logMessage << "\t\t\t\t\t\t" << this->westLong << "              " << this->eastLong << endl;
        logMessage << "\t\t\t\t\t\t           " << this->southLat << endl;
        logMessage << "------------------------------------------------------------------------------------------";
        Logger::getInstance().writeLog(logMessage.str());
    }

    void World::debugWorldToLog(GIS::PRQuadtree* prQuadTree) {
        stringstream logMessage;
        logMessage << "+--------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
        int** matrixWorld = prQuadTree->quadAndData();
        string row = "";
        for (int i = 0; i < 40; i++) {
            row ="|";
            for(int j = 0; j < 140; j++) {

                if (matrixWorld[i][j] > 0) {
                    row += to_string(matrixWorld[i][j]);
                } else {
                    row += " ";
                }
            }
            row +="|\n";
            logMessage << row;
        }

        logMessage << "+--------------------------------------------------------------------------------------------------------------------------------------------+" << endl;
        logMessage << "------------------------------------------------------------------------------------------";
        Logger::getInstance().writeLog(logMessage.str());
    }
}
