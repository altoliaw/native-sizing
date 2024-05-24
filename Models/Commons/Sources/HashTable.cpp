/**
 * @see HashTable.hpp
 */
#include "../Headers/HashTable.hpp"

namespace Commons {

/**
 * Constructor
 *
 * @param size [int] The size of the hashtable, the default value is 97 (a prime);
 * if possible, the value of the size shall be set as a prime
 */
HashTable::HashTable(int size) {
    hashTableSize = size;
    queue = lastElement = nullptr;
    hashTable = new element* [hashTableSize] {};
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // Releasing all objects from the queue
    element* current = nullptr;

    // Traversal all objects in the queued linked list
    for (; queue != nullptr;) {
        current = queue;
        queue = queue->nextInQueue;
        // Releasing the element's memory
        elementDestructor(current);
        delete current;
        current = nullptr;
    }
    queue = nullptr;
    lastElement = nullptr;

    // Because all elements have been removed from queue, the hash table can be removed directly
    if (hashTable != nullptr) {
        delete[] hashTable;
    }
    hashTable = nullptr;
    hashTableSize = 0;
}

/**
 * Obtaining the result from the hash table
 *
 * @param columnName [char*] The column name
 * @return [char*] The value; if the name does not in the hash table, the nullptr will be returned
 */
char* HashTable::getValueByName(char* columnName) {
    char* result = nullptr;
    unsigned int index = getHashIndex(columnName);
    element* current = hashTable[index];
    for (; current != nullptr;) {
        if (strcmp(current->columnName, columnName) != 0) {  // If the two string are equal, ...
            current = current->nextInHashTable;
        } else {
            result = current->value;
            break;
        }
    }

    return result;
}

/**
 *
 * @param columnName [char*] The name of the column
 * @return [POSIXErrors] The success/fail value*
 */
POSIXErrors HashTable::removeElementByName(char* columnName) {
    element* removedItem = nullptr;

    unsigned int index = getHashIndex(columnName);
    element* current = hashTable[index];
    element* previous = nullptr;
    for (; current != nullptr;) {
        if (strcmp(current->columnName, columnName) != 0) {  // If the two string are equal, ...
            previous = current;
            current = current->nextInHashTable;
        } else {
            removedItem = current;
            break;
        }
    }

    // If there exists nothing in the hash table with linked lists, ...
    if (removedItem == nullptr) {
        return POSIXErrors::E_NOITEM;
    } else {  // Hitting the element
        // Maintaining the linked list from the hash table (single direction)
        if (previous == nullptr && removedItem->nextInHashTable == nullptr) {  // There is only one element in the linked list
                                                                               // and the one is the removedItem
            hashTable[index] = nullptr;
        } else if (previous == nullptr && removedItem->nextInHashTable != nullptr) {  // The first element is the removedItem
                                                                                      // in the linked list.
            hashTable[index] = removedItem->nextInHashTable;
        } else if (previous != nullptr && removedItem->nextInHashTable == nullptr) {  // The last element is the removedItem.
                                                                                      // inn the linked list
        } else {
            previous->nextInHashTable = removedItem->nextInHashTable;
        }
    }

    // Maintaining the queue (bi-direction)
    previous = removedItem->previousInQueue;
    element* next = removedItem->nextInQueue;
    if (previous == nullptr) {
        if (next == nullptr) {
            // In the linked listed, there exist an element, namely removedItem.
        } else {  // The previousInQueue of the next element shall be modified as nullptr
            next->previousInQueue = nullptr;
        }
    } else {  // If there exist an element before the removedItem, ...
        if (next == nullptr) {
            // The nextInQueue of the previous element shall be modified as nullptr
            previous->nextInQueue = nullptr;
        } else {
            // The nextInQueue of the previous element shall be modified as the next element and
            // the previousInQueue of the next element shall be modified as the previous element
            previous->nextInQueue = next;
            next->previousInQueue = previous;
        }
    }

	// Deleting the removedItem
	elementDestructor(removedItem);
    return POSIXErrors::OK;
}

/**
 * Adding an element, and putting the element into the hash table with linked lists
 *
 * @param columnName [char*] The name of the column
 * @param value [char*] The value of the column
 * @return [POSIXErrors] The success/fail value
 */
POSIXErrors HashTable::addElementIntoHashTable(char* columnName, char* value) {
    // Creating an element instance by using dynamic memory allocation
    element* instance = new element{columnName, value, nullptr, nullptr, nullptr};
    if (instance == nullptr) {
        return POSIXErrors::E_NOMEM;
    }
    elementConstructor(instance);  // Initialization

    // Registering the element into the queue
    if (queue == nullptr) {
        queue = lastElement = instance;
    } else {
        lastElement->nextInQueue = instance;
        instance->previousInQueue = lastElement;
        lastElement = instance;
    }

    // Setting the element into the hash table
    unsigned int index = getHashIndex(instance->columnName);
    element* current = nullptr;
    if (hashTable[index] != nullptr) {
        current = hashTable[index];

        // Looping to the end element of the linked list
        for (; current != nullptr;) {
            current = current->nextInHashTable;
        }
        // Linking the new element and the last element in the linked list
        current->nextInHashTable = instance;
    } else {
        hashTable[index] = instance;
    }

    return POSIXErrors::OK;
}

/**
 * Element constructor
 *
 * @param object [element*] The address of the element object
 */
void HashTable::elementConstructor(element* object) {
    object->columnName = nullptr;
    object->nextInHashTable = nullptr;
    object->nextInQueue = nullptr;
    object->previousInQueue = nullptr;
    object->value = nullptr;
}

/**
 * Element destructor
 *
 * @param object [element*] The address of the element object
 */
void HashTable::elementDestructor(element* object) {
    // Removing the memories for the "char" arrays
    if (object->columnName != nullptr) {
        delete[] object->columnName;
        object->columnName = nullptr;
    }
    if (object->value != nullptr) {
        delete[] object->value;
        object->value = nullptr;
    }
    object->nextInQueue = nullptr;
    object->previousInQueue = nullptr;
    object->nextInHashTable = nullptr;
}

/**
 * Obtaining the index from the name
 *
 * @param columnName [char*] The starting address of the name string
 * @return [unsigned int] The index for tha hash table
 */
unsigned int HashTable::getHashIndex(char* columnName) {
    unsigned int index = 0;
    for (unsigned int i = 0; i < strlen(columnName); i++) {
        index += (unsigned int)columnName[i];
    }
    index %= hashTableSize;
    return index;
}

};  // namespace Commons