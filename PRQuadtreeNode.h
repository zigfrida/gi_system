//
// Created by Amanda Golubics on 2023-06-21.
//

#ifndef GI_SYSTEM_PRQUADTREENODE_H
#define GI_SYSTEM_PRQUADTREENODE_H

#include <vector>
#include "CoordinateIndex.h"

namespace GIS {
    class PRQuadtreeNode {
        static const int MAX_BUCKET_SIZE = 4;

    public:
        int minLatitude;
        int maxLatitude;
        int minLongitude;
        int maxLongitude;
        PRQuadtreeNode* children[4];                 // A node can only have 4 children independent of bucket size
        vector<CoordinateIndex*> data;                // If the node is a leaf, it contains data

        PRQuadtreeNode(int minLat, int maxLat, int minLong, int maxLong) : minLatitude(minLat), maxLatitude(maxLat), minLongitude(minLong), maxLongitude(maxLong) {
            for (int i = 0; i < MAX_BUCKET_SIZE; ++i) {
                children[i] = nullptr;
            }
        }

        ~PRQuadtreeNode() {
            for (int i = 0; i < 4; ++i) {
                delete children[i];
            }

            for (auto dataPoint :  data) {
                delete dataPoint;
            }
        }

        bool isLeafNode() const {
            return children[0] == nullptr;  // A node is a leaf if  it does not have any children
        }

        bool isFull() const {
            return data.size() == MAX_BUCKET_SIZE;
        }
    };
}


#endif //GI_SYSTEM_PRQUADTREENODE_H
