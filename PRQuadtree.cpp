//
// Created by Amanda Golubics on 2023-06-21.
//

#include <iostream>
#include "PRQuadtree.h"
#include "World.h"

using namespace std;

namespace GIS {

    void PRQuadtree::insert(const CoordinateIndex& coordinate, const GISRecord& record){
        if (!isCoordinateInWorld(coordinate.latitude, coordinate.longitude)) {
            cout << "Coordinate Lat " << coordinate.latitude << endl;
            cout << "World max Lat " <<  worldMaxLatitude << endl;
            cout << "World min Lat " << worldMinLatitude << endl;
            cout << endl;
            cout << "Coordinate Long " << coordinate.longitude << endl;
            cout << "World max Long " << worldMaxLongitude << endl;
            cout << "World min Long " <<worldMinLongitude << endl;
            cout << endl;
            cout << "Error: Coordinate is outside the world boundary!" << endl;
            return;
        }

        if(root == nullptr) {
            root = new PRQuadtreeNode(worldMinLatitude, worldMaxLatitude, worldMinLongitude, worldMaxLongitude);
        }
        insertHelper(root, coordinate);
    }

    void PRQuadtree::destroyTree(PRQuadtreeNode* node) {
        if (node != nullptr) {
            for (int i = 0; i < 4; ++i) {
                destroyTree(node->children[i]);
            }
            delete node;
        }
    }

    void PRQuadtree::insertHelper(PRQuadtreeNode* node, const CoordinateIndex& coordinate){
        if (node->isLeafNode()) { // Check if the current node is a leaf node
            if (!node->isFull()) { // Checking if leaf is full
                // Check if the coordinate already exists in the leaf node
                for (auto dataIndex : node->data) {
                    if (dataIndex->latitude == coordinate.latitude && dataIndex->longitude == coordinate.longitude) {
                        // Update the existing data entry with new file offset
                        dataIndex->fileOffsets.insert(dataIndex->fileOffsets.end(), coordinate.fileOffsets.begin(), coordinate.fileOffsets.end());
                        return;
                    }
                }

                // Create a new Coordinate Index and add it to the leaf node
                CoordinateIndex* newIndex = new CoordinateIndex(coordinate.latitude, coordinate.longitude);
                newIndex->fileOffsets = coordinate.fileOffsets;
                node->data.push_back(newIndex);
            } else { // Leaf node is full, need to partition first
                partitionLeaf(node);
                insertHelper(node, coordinate); // Leaf is partitioned, now insert
            }
        } else { // Current node is not a leaf
            int quadrant = getQuadrant(node, coordinate.latitude, coordinate.longitude); // Check which quadrant the coordinate should be inserted

            if(node->children[quadrant] == nullptr) { // Check if child node for the specified quadrant exists
                createChildNode(node, quadrant); // Create child node if it does not exist
            }

            insertHelper(node->children[quadrant], coordinate);
        }
    }

    void PRQuadtree::partitionLeaf(PRQuadtreeNode *node) {
        for (int i = 0; i < 4; ++i) {
            createChildNode(node, i);
        }

        // Re-insert data records into the appropriate child nodes
        for (auto dataPoint : node->data) {
            int quadrant = getQuadrant(node, dataPoint->latitude, dataPoint->longitude);
            insertHelper(node->children[quadrant], *dataPoint);
            delete dataPoint;
        }
        node->data.clear();
    }

    void PRQuadtree::createChildNode(PRQuadtreeNode* parent, int quadrant) {
        int midLatitude = (parent->maxLatitude + parent->minLatitude) / 2;
        int midLongitude = (parent->maxLongitude + parent->minLongitude) / 2;

        switch (quadrant) {
            case 0: {
                parent->children[quadrant] =
                        new PRQuadtreeNode(parent->minLatitude, midLatitude + 1, parent->minLongitude, midLongitude + 1); // Added 1 to make sure boundaries between regions are included
                break;
            }
            case 1: {
                parent->children[quadrant] =
                        new PRQuadtreeNode(parent->minLatitude, midLatitude + 1, midLongitude, parent->maxLongitude + 1);
                break;
            }
            case 2: {
                parent->children[quadrant] =
                        new PRQuadtreeNode(midLatitude, parent->maxLatitude + 1, parent->minLongitude, midLongitude + 1);
                break;
            }
            case 3: {
                parent->children[quadrant] =
                        new PRQuadtreeNode(midLatitude, parent->maxLatitude + 1, midLongitude, parent->maxLongitude + 1);
                break;
            }
        }
    }

    bool PRQuadtree::isCoordinateInWorld(int latitude, int longitude) {
        return latitude >= worldMinLatitude && latitude <= worldMaxLatitude &&
               longitude >= worldMinLongitude && longitude <= worldMaxLongitude;
    }

    // Determines the quadrant (the child node) in which a given coordinate should be inserted or located in the PR Quadtree
    int PRQuadtree::getQuadrant(PRQuadtreeNode *node, int latitude, int longitude) const {
        double latMid = (node->minLatitude + node->maxLatitude) / 2.0;
        double lonMid = (node->minLongitude + node->maxLongitude) / 2.0;

        if (latitude <= latMid) {
            if (longitude <= lonMid) {
                return 0; // Southwest - Latitude and Longitude are less than or equal to midpoint values
            } else {
                return 1; // Southeast - Latitude is less than or equal to the midpoint value, but longitude is greater
            }
        } else {
            if (longitude <= lonMid) {
                return 2; // Northwest - Latitude is greater than midpoint value, but Longitude is less than or equal to midpoint
            } else {
                return 3; // Northeast - Latitude and Longitude are greater than or equal to midpoint values
            }
        }

    }

    void PRQuadtree::displayPRQuadtree(GIS::PRQuadtreeNode *node, int level) {
        if (node == nullptr) return; // tree is empty
        // Indentation based on the indent level
        for (int i = 0; i < level; ++i) {
            cout << "  ";
        }

        if (node->isLeafNode()) {
            // Print leaf node information
            for (const auto& index : node->data) {
                cout << "[(" << index->latitude << "," << index->longitude << "), " << index->fileOffsets.size() << "] ";
            }
            cout << endl;
        } else {
            // Print internal node information
            cout << "@" << endl;
            for (int i = 0; i < 4; ++i) {
                // Recursively display child nodes with increased indent
                displayPRQuadtree(node->children[i], level + 1);
            }
        }
    }

}