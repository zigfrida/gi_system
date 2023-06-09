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

            PRQuadtree(int westLat, int eastLat, int southLong, int northLong) : root(nullptr), worldMinLatitude(southLong), worldMaxLatitude(northLong), worldMaxLongitude(eastLat), worldMinLongitude(westLat) {
            }

            ~PRQuadtree() {
                destroyTree(root);
            }

            void insert(const CoordinateIndex& coordinate, const GISRecord& record);
            void displayPRQuadtree(PRQuadtreeNode* node, int level = 0);
            void displayDebugPRQuadtree(PRQuadtreeNode* node, int level = 0);
            vector<int> searchOne(int latToSearch, int longToSearch);
            vector<int> searchOneHelper(int latToSearch, int longToSearch, PRQuadtreeNode* node);
            vector<int> treeSearchArea(int latitude, int longitude, int latSpan, int longSpan);
            void treeSearchAreaHelper(int latToSearch, int longToSearch, int latSpan, int longSpan, PRQuadtreeNode* node, vector<int>& result);
            bool isNodeInsideSearchArea(int nodeMaxX, int nodeMinX, int nodeMaxY, int nodeMinY, int searchCenterX, int searchCenterY, int searchSpanX, int searchSpanY);
            int** quadAndData();
            void quadAndDataHelper(PRQuadtreeNode* node, int**& result);
            void convertCoordinates(int x, int y, int& convertedX, int& convertedY);


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
