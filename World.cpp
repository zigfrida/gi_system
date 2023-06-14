//
// Created by dldms on 4/26/2023.
//

#include <iostream>
#include "World.h"
using namespace std;
namespace GIS {
    void World::createWorld(string westLong, string eastLong, string southLat, string northLat) {
        this->westLong = convertStringLatLongToInt(westLong);
        this->eastLong = convertStringLatLongToInt(eastLong);
        this->southLat = convertStringLatLongToInt(southLat);
        this->northLat = convertStringLatLongToInt(northLat);

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
}
