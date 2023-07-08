//
// Created by Amanda Golubics on 2023-06-14.
//

#include <iostream>
#include <sstream>
#include "HashTable.h"
#include "Logger.h"

using namespace std;

namespace GIS {

    void HashTable::resizeTable() {
        int newSize = size * 2;
        NameIndex* newTable = new NameIndex[newSize];
        for (int i = 0; i < size; ++i) {
            if (!table[i].key.empty() && !table[i].isDeleted) {
                int index = findIndex(newTable, newSize, table[i].key);
                newTable[index] = table[i];
            }
        }
        delete[] table;
        table = newTable;
        size = newSize;
    }

    int HashTable::elfhash(const std::string &key) {
        unsigned long hash = 0;
        for (char c : key) {
            hash = (hash << 4) + c;
            unsigned long x = hash & 0xF0000000L;
            if (x != 0)
                hash ^= x >> 24;
            hash &= ~x;
        }
        return hash % size;
    }

    int HashTable::findIndex(const NameIndex *table, int tableSize, const string &key) {
        int index = elfhash(key);
        int i = 1;
        while (!table[index].key.empty() && table[index].key != key) {
            index = (index + i * i) % tableSize;
            i++;
        }
        return index;
    }

    int HashTable::resolveCollision(int index, int stepSize) {
        int n = 1;
        int originalIndex = index;

        // Execute at least once since insert already checks once
        do {
            index = (originalIndex + (stepSize * n) + ((n * n + n) / 2) % size);
            n++;
        } while (!table[index].key.empty() && !table[index].isDeleted);
        // If index is not empty and not marked as deleted, a collision has occurred

        return index;
    }

    void HashTable::insert(const string &key, const string &value) {
        if ((float)count / size >= MAX_THRESHOLD) {
            Logger::getInstance().writeLog("Table is full, resizing");
            resizeTable();
        }

        int index = findIndex(table, size, key);
        int stepSize = 1;

        // If index is not empty and not marked as deleted, a collision has occurred
        if (!table[index].key.empty() && !table[index].isDeleted) {
            index = resolveCollision(index, stepSize);
        }

        table[index] = NameIndex(key, value);
        count++;
    }

    void HashTable::remove(const std::string &key) {
        int index = findIndex(table, size, key);
        if (!table[index].key.empty() && !table[index].isDeleted) {
            table[index].key = "";
            table[index].value = "";
            table[index].isDeleted = true;
            count--;
        }
    }

    string HashTable::search(const std::string &key) {
        int index = findIndex(table, size, key);
        if (!table[index].key.empty() && !table[index].isDeleted) {
            return table[index].value;
        }
        return "-1";
    }

    void HashTable::displayHashTable() {
        for (int i = 0; i < size; ++i) {
            if (!table[i].key.empty() && !table[i].isDeleted) {
                cout << "Key: " << table[i].key << " Value: " << table[i].value << endl;
                cout << "---------------------------------------" << endl;
            }
        }
    }

    void HashTable::displayDebugHashTable() {
        stringstream logMessage;
        logMessage << "Format of display is \n"
                      "Slot number: data record" << endl;
        logMessage << "Current table size is " << size << endl;
        logMessage << "Number of elements in table is " << count << endl << endl;
        for (int i = 0; i < size; i++) {
            if (!table[i].key.empty() && !table[i].isDeleted) {
                logMessage << "\t" << i << ": [" << table[i].key << ", [" << table[i].value << "]]" << endl;
            }
        }
        logMessage << "------------------------------------------------------------------------------------------";
        Logger::getInstance().writeLog(logMessage.str());
    }
}