//
// Created by Amanda Golubics on 2023-06-21.
//

#ifndef GI_SYSTEM_PRQUADTREE_H
#define GI_SYSTEM_PRQUADTREE_H

#include <iostream>
#include <vector>
#include "PRQuadtreeNode.h"
#include "CoordinateIndex.h"
#include "GISRecord.h"

using namespace std;

namespace GIS {

    class PRQuadtree {
        public:
            PRQuadtreeNode* root;
            int worldMinLatitude;
            int worldMaxLatitude;
            int worldMinLongitude;
            int worldMaxLongitude;

            PRQuadtree(int westLat, int eastLat, int southLong, int northLong) : root(nullptr), worldMinLatitude(westLat), worldMaxLatitude(eastLat), worldMaxLongitude(northLong), worldMinLongitude(southLong) {
                cout << "PRQuadtree created" << endl;
            }

            ~PRQuadtree() {
                destroyTree(root);
            }

            void insert(const CoordinateIndex& coordinate, const GISRecord& record);
            void displayPRQuadtree(PRQuadtreeNode* node, int level = 0);

        private:
            void destroyTree(PRQuadtreeNode* node);
            void insertHelper( PRQuadtreeNode* node, const CoordinateIndex& coordinate);
            void partitionLeaf(PRQuadtreeNode* node);
            int getQuadrant(PRQuadtreeNode *node, int latitude, int longitude) const;
            void createChildNode(PRQuadtreeNode* parent, int quadrant);
            bool isCoordinateInWorld(int latitude, int longitude);

    };
}


#endif //GI_SYSTEM_PRQUADTREE_H
