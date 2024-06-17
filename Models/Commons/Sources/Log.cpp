/**
 * @see Log.hpp
 */
#include "../Headers/Log.hpp"

namespace Commons {
// The definition of the static variable of the class
HashTable* Log::logTablePointer = nullptr;

/**
 * The function is the first method when using static methods in this class; all
 * variables in the class will be initialized; the function has been called by other static functions
 * in this class; as a result, users do not call the function in the codes
 */
void Log::getHashTableInitialization() {
    static HashTable hashTable;  // The constructor will be executed automatically
    if (Log::logTablePointer == nullptr) {
        Log::logTablePointer = &hashTable;  // Static singleton
    }
}

/**
 * Setting the log information into the hash table; that implies the log path will be set in 
 * the hash table
 *
 * @param value [void*] The log file name (char*) and shall be cast into the void* because of a general solution
 * @param sizeOfMemory [size_t] The size of the memory of the value above
 * @param type [HashTable::ElementType] The type of the value above; the default value is "HashTable::ElementType::charStarType"
 * @param abbreviatedLogName [const char*] The abbreviation of the log name; the default value is "default"
 * @param isReassignment [char] The flag for determining if the value shall be reassigned when the element has existed in the hash table;
 * when the value is equal to 0x0, the value will not be reassigned in the element when the element exists;
 * when the value is equal to 0x1, the value will be reassigned in the element when the element exists;
 * the default value is 0x1
 * @return [char] The flag if the function works well; if the function is okay, the value is 0x1; otherwise 0x0 (error occurs)
 */
char Log::setLogTableInformation(void* value, size_t sizeOfMemory, HashTable::ElementType type, const char* abbreviatedLogName, char isReassignment) {
    // Initialization automatically if the pointer is equal to nullptr
    Log::getHashTableInitialization();

    char isSuccess = 0x0;
    char isExisted = 0x0;
    void* tmpValue = nullptr;  // The address which refers to the value object

    HashTable::ElementType tmpType = HashTable::ElementType::charStarType;
    size_t tmpMemorySize = 0;

    // Verifying if the existence of the element
    isExisted = (Log::logTablePointer)->getValueByName((char*)abbreviatedLogName, &tmpValue, &tmpMemorySize, &tmpType);
    if (isExisted == 0x0) {  // The element does not exist, calling the function, HashTable::addElementIntoHashTable(.)
        (Log::logTablePointer)->addElementIntoHashTable((char*)abbreviatedLogName, value, sizeOfMemory);
        isSuccess = 0x1;
    } else {                            // When the element exists, ...
        if (isReassignment == 0x1) {    // The situation where the element exists and the flag is 0x1
            if (tmpValue == nullptr) {  // The element exists and the value is nullptr.
                // "calloc" method
                tmpValue = calloc(1, sizeOfMemory);
            } else {
                // "realloc" method
                tmpValue = realloc(tmpValue, sizeOfMemory);
            }
            // When the dynamic allocation has been success, the log table has been set
            if (tmpValue != nullptr) {
                isSuccess = 0x1;
                memcpy(tmpValue, value, sizeOfMemory);
            }
        }
    }
    return isSuccess;
}

/**
 *
 */
void Log::executeLog(std::ostream&, const char*, POSIXSysLog = POSIXSysLog::Debug, const char* = "default") {

}

/**
 * Obtaining the log information from the hash table; that implies the log path
 * 
 * @param value [void**] The address of the pointer of the value (FILE*)
 * @param sizeOfMemory [size_t*] The size of the memory of the value above
 * @param type [HashTable::ElementType*] The address of the HashTable::ElementType and the type implies the 
 * original type of the value
 * @param abbreviatedLogName [const char*] The key in the hash table, the default value is "default"
 * @return [char] The successful flag; when the flag is 0x1, the function is okay; otherwise, the function error occurs
 */
char Log::getLogTableInformation(void** value, size_t* sizeOfMemory,  HashTable::ElementType* type, const char* abbreviatedLogName) {
    char isExisted = 0x0;

    // Verifying if the existence of the element
    isExisted = (Log::logTablePointer)->getValueByName((char*)abbreviatedLogName, value, sizeOfMemory, type);
    return isExisted;
}

}  // namespace Commons