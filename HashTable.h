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

        // Hash table data structure
        struct hashTable {
            string key;
            string value; // Line offset in the file (-1 because of header)
            bool isDeleted;
        };

        hashTable* table;
        int size;
        int count;

        void resizeTable();

        // source: https://www.programmingalgorithms.com/algorithm/elf-hash/cpp/
        int elfhash(const string& key);

        int findIndex(const hashTable* table, int tableSize, const string& key);

    public:
        void resolutionFunction();
        void linearProbing();

        // Constructor
        HashTable() : size(INITIAL_SIZE), count(0) {
            table = new hashTable[size];
        }

        // Destructor for the HashTable
        ~HashTable(){
            delete[] table;
        }

        void insert(const string& key, const string& value);

        void remove(const string& key);

        string search(const string& key);

        void displayHashTable();

        void displayDebugHashTable();

        int getSize() {return size;}

        int getCount() {return count;}


    };
}


#endif //GI_SYSTEM_HASHTABLE_H
