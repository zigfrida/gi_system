//
// Created by Amanda Golubics on 2023-06-14.
//

#ifndef GI_SYSTEM_HASHTABLE_H
#define GI_SYSTEM_HASHTABLE_H

using namespace std;

namespace GIS {
    class HashTable {
        const int INITIAL_SIZE = 1024;
        const float MAX_THRESHOLD = 0.7;
    private:

        class NameIndex {
            public:
                string key;
                string value; // Line offset in the file (-1 because of header)
                bool isDeleted;
                NameIndex(const string& key = "", const string& value = "") : key(key), value(value), isDeleted(false) {}
        };

        NameIndex* table;
        int size;
        int count;
        int longestProbeSequence;

        void resizeTable();

        // source: https://www.programmingalgorithms.com/algorithm/elf-hash/cpp/
        int elfhash(const string& key);

        int findIndex(const NameIndex* table, int tableSize, const string& key);

    public:

        // Constructor
        HashTable() : size(INITIAL_SIZE), count(0) {
            table = new NameIndex[size]();
        }

        // Destructor for the HashTable
        ~HashTable(){
            delete[] table;
        }

        int resolveCollision(int index, int stepSize);

        void insert(const string& key, const string& value);

        void remove(const string& key);

        string search(const string& key);

        void displayHashTable();

        void displayDebugHashTable();

        int getSize() {return size;}

        int getCount() {return count;}

        int getLongestProbSequence() const {return longestProbeSequence;}
    };
}


#endif //GI_SYSTEM_HASHTABLE_H
