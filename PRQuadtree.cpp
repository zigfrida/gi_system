//
// Created by Amanda Golubics on 2023-06-21.
//

#include <iostream>
#include <sstream>
#include "PRQuadtree.h"
#include "World.h"
#include "Logger.h"

using namespace std;

namespace GIS {

    void PRQuadtree::insert(const CoordinateIndex& coordinate, const GISRecord& record){
        if (!isCoordinateInWorld(coordinate.latitude, coordinate.longitude)) {
            Logger::getInstance().writeLog("Error: Coordinate is outside the world boundary!");
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

    vector<int> PRQuadtree::searchOne(int latToSearch, int longToSearch) {
        vector<int> emptyy;
        emptyy.clear();
        if (!isCoordinateInWorld(latToSearch, longToSearch)) {
            return emptyy;
        }

        if(root == nullptr) {
            return emptyy;
        }
        return searchOneHelper(latToSearch, longToSearch, root);
    }

    vector<int> PRQuadtree::searchOneHelper(int latToSearch, int longToSearch, PRQuadtreeNode* node) {
            if (!node->data.empty()) { // Checking if leaf is full
                // Check if the coordinate already exists in the leaf node
                for (auto dataIndex : node->data) {
                    if (dataIndex->latitude == latToSearch && dataIndex->longitude == longToSearch) {
                        // Update the existing data entry with new file offset
                        return dataIndex->fileOffsets;
                    }
                }
            }
            int childToSeach = getQuadrant(node, latToSearch, longToSearch);
            if (node->children[childToSeach] == nullptr) {
                vector<int> empty;
                empty.clear();
                return empty;
            } else {
                return searchOneHelper(latToSearch, longToSearch, node->children[childToSeach]);
            }
    }


    vector<int> PRQuadtree::treeSearchArea(int latToSearch, int longToSearch, int latSpan, int longSpan) {
        vector<int> result;
        result.clear();
        if (!isCoordinateInWorld(latToSearch, longToSearch)) {
            return result;
        }

        if(root == nullptr) {
            return result;
        }

        treeSearchAreaHelper(latToSearch, longToSearch, latSpan, longSpan, root, result);
        return result;
    }

    void PRQuadtree::treeSearchAreaHelper(int latToSearch, int longToSearch, int latSpan, int longSpan, PRQuadtreeNode* node, vector<int>& result) {
        if (!node->data.empty()) {
            for (auto dataIndex : node->data) {
                int latitude = dataIndex->latitude;
                int longitude = dataIndex->longitude;

                if (latitude <= latToSearch + latSpan
                    && latitude >= latToSearch - latSpan
                    && longitude <= longToSearch + longSpan
                    && longitude >= longToSearch - longSpan) {
                    // The coordinate is within the search area
                    result.insert(result.end(), dataIndex->fileOffsets.begin(), dataIndex->fileOffsets.end());
                }
            }
        }



        for (int i = 0; i < 4; ++i) {
            if (node->children[i] != nullptr) {
                int nodeMaxLat = node->children[i]->maxLatitude;
                int nodeMaxLong = node->children[i]->maxLongitude;
                int nodeMinLat = node->children[i]->minLatitude;
                int nodeMinLong = node->children[i]->minLongitude;
                if (
                        isNodeInsideSearchArea(nodeMaxLat, nodeMinLat, nodeMaxLong, nodeMinLong, latToSearch, longToSearch, latSpan, longSpan)
                        )

                    treeSearchAreaHelper(latToSearch, longToSearch, latSpan, longSpan, node->children[i], result);


            }
        }
    }

    bool PRQuadtree::isNodeInsideSearchArea(int nodeMaxX, int nodeMinX, int nodeMaxY, int nodeMinY, int searchCenterX, int searchCenterY, int searchSpanX, int searchSpanY) {
        // Calculate the boundaries of the square
        int node2MinX = searchCenterX - searchSpanX;
        int node2MaxX = searchCenterX + searchSpanX;
        int node2MaxY = searchCenterY + searchSpanY;
        int node2MinY = searchCenterY - searchSpanY;

        // Check if the nodes overlap in the x-axis
        bool overlapX = (nodeMaxX >= node2MinX) && (nodeMinX <= node2MaxX);

        // Check if the nodes overlap in the y-axis
        bool overlapY = (nodeMaxY >= node2MinY) && (nodeMinY <= node2MaxY);

        // Return true if there is overlap in both x and y axes
        return overlapX && overlapY;
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

    void PRQuadtree::displayDebugPRQuadtree(GIS::PRQuadtreeNode *node, int level) {
        stringstream logMessage;
        if (node == nullptr) return; // tree is empty
        // Indentation based on the indent level
        for (int i = 0; i < level; ++i) {
            logMessage << "  ";
        }

        if (node->isLeafNode()) {
            // Print leaf node information
            for (const auto& index : node->data) {
                logMessage << "[(" << index->latitude << "," << index->longitude << "), " << index->fileOffsets.size() << "] ";
            }
        } else {
            // Print internal node information
            logMessage << "@\n";
            for (int i = 0; i < 4; ++i) {
                // Recursively display child nodes with increased indent
                displayDebugPRQuadtree(node->children[i], level + 1);
            }
        }

        if (level == 0)
            logMessage << "------------------------------------------------------------------------------------------";

        Logger::getInstance().writeLog(logMessage.str());
    }


    vector<vector<int>> PRQuadtree::quadAndData() {
        vector<vector<int>> result;
        result.clear();
        if (!isCoordinateInWorld(latToSearch, longToSearch)) {
            return result;
        }

        if(root == nullptr) {
            return result;
        }

        treeSearchAreaHelper(latToSearch, longToSearch, latSpan, longSpan, root, result);
        return result;
    }

    void PRQuadtree::quadAndDataHelper(PRQuadtreeNode* node, vector<int[]>& result) {
        if (!node->data.empty()) {
            for (auto dataIndex : node->data) {
                int latitude = dataIndex->latitude;
                int longitude = dataIndex->longitude;

                if (latitude <= latToSearch + latSpan
                    && latitude >= latToSearch - latSpan
                    && longitude <= longToSearch + longSpan
                    && longitude >= longToSearch - longSpan) {
                    // The coordinate is within the search area
                    result.insert(result.end(), dataIndex->fileOffsets.begin(), dataIndex->fileOffsets.end());
                }
            }
        }



        for (int i = 0; i < 4; ++i) {
            if (node->children[i] != nullptr) {
                int nodeMaxLat = node->children[i]->maxLatitude;
                int nodeMaxLong = node->children[i]->maxLongitude;
                int nodeMinLat = node->children[i]->minLatitude;
                int nodeMinLong = node->children[i]->minLongitude;
                if (
                        isNodeInsideSearchArea(nodeMaxLat, nodeMinLat, nodeMaxLong, nodeMinLong, latToSearch, longToSearch, latSpan, longSpan)
                        )

                    treeSearchAreaHelper(latToSearch, longToSearch, latSpan, longSpan, node->children[i], result);


            }
        }
    }
}