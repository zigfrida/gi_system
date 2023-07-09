//
// Created by dldms on 4/26/2023.
//

#ifndef GI_SYSTEM_WORLD_H
#define GI_SYSTEM_WORLD_H

#include <string>
#include "PRQuadtree.h"

using namespace std;

namespace GIS {
    class World {
    public:
        int westLong;
        int eastLong;
        int southLat;
        int northLat;
        void createWorld(string westLong, string eastLong, string southLat, string northLat);
        static int convertStringLatLongToInt(string lat);
        bool isItInWorldBoundary(string lat, string longitude);
        void writeWorldToLog(string westLong, string eastLong, string southLat, string northLat);
        void debugWorldToLog(GIS::PRQuadtree* prQuadTree);
    };
}



#endif //GI_SYSTEM_WORLD_H
