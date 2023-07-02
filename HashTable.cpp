//
// Created by Amanda Golubics on 2023-06-14.
//

#include <iostream>
#include "HashTable.h"

using namespace std;

namespace GIS {

    void HashTable::resizeTable() {
        int newSize = size * 2;
        hashTable* newTable = new hashTable[newSize];
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

    int HashTable::findIndex(const GIS::HashTable::hashTable *table, int tableSize, const std::string &key) {
        int index = elfhash(key);
        int i = 1;
        while (!table[index].key.empty() && table[index].key != key) {
            index = (index + i * i) % tableSize;
            i++;
        }
        return index;
    }

    void HashTable::insert(const std::string &key, const std::string &value) {
        if ((float)count / size >= MAX_THRESHOLD) {
            cout << "Table is full, resizing." << endl;
            resizeTable();
        }

        int index = findIndex(table, size, key);
        int stepSize = 1;
        int n;

        // If index is not empty and not marked as deleted, a collision has occurred
        while(!table[index].key.empty() && !table[index].isDeleted) {
            // Collision occurred
            n++;
            index = (index + stepSize) % size;
            stepSize = (((n * n) + n)/2); // Computes the step size per instructions
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isDeleted = false;
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
}