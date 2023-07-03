//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_COORDINATEINDEX_H
#define GI_SYSTEM_COORDINATEINDEX_H

#include <vector>
#include "GISRecord.h"

using namespace std;

namespace GIS {
    class CoordinateIndex {
        public:
            int latitude;
            int longitude;
            vector<GISRecord> gis_records;
            vector<int> fileOffsets;

            CoordinateIndex(int lat, int lon) : latitude(lat), longitude(lon) {}
    };
}


#endif //GI_SYSTEM_COORDINATEINDEX_H
