#pragma once
/** @file HashTable.hpp
 * The class of the hash table
 *
 * @author Nick, Liao
 * @date 2024/05/24
 */
#include <string.h>

#include "./POSIXErrors.hpp"

namespace Commons {
/**
 * The hash table (Some customers have old platforms; that implies users
 * can not use container in C++)
 */
class HashTable {
   public:
    /**
     * The element for the hash table
     */
    struct element {
        // The column name
        char* columnName;
        // The value of the column name
        char* value;
        // The next address of the element for linking with the hash table
        element* nextInHashTable;

        // The next address of the element in queuing for
        // the situation when all elements will be removed
        element* nextInQueue;
        // The next address of the element in queuing for
        // the situation when all elements will be removed (easily maintain the structure in the queue)
        element* previousInQueue;
		element(char* columnName, char* value);
		~element();
    };

    // The size of the size of the hash table
    unsigned int hashTableSize;
    // The pointer to the hash table (array), and each element in the table is an element
    element** hashTable;
    // A queue for reserving all elements in the hash table;
    // this is a mechanism for fast releasing memory
    element* queue;
    // The address of the last element in the queue
    element* lastElement;

    HashTable(int = 97);
    virtual ~HashTable();
    virtual char getValueByName(char*, char**);
    virtual POSIXErrors removeElementByName(char*);
    virtual POSIXErrors addElementIntoHashTable(char*, char*);

   protected:
    virtual unsigned int getHashIndex(char*);
};

}  // namespace Commons