#pragma once
/** @file InitializedFileParser.hpp
 * A class for parsing the key and value pairs from the .ini file; this class will be implemented 
 * in a singleton pattern; for unit testing the singleton pattern in various unit tests, the unique
 * pointer shall be used. This is because the static pointer shall refer to the dynamic memory manually and
 * the memory shall be released manually; if the pointer is a normal pointer and refers to a static instance
 * in the initialization function, the dynamic memory released phase can not be implemented
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */
#include <memory> // For the unique pointer

#include "./HashTable.hpp"
#include "./StringImplement.hpp"

namespace Commons {
/**
 * Several InitializedFileParser implementation (singleton)
 */
class InitializedFileParser {
   public:
    // For referring to the hash table, and the hash table will assigned 
    // by using the dynamic memory allocation
    Commons::HashTable* initializedTable;

    // A static variable for the object from the class (singleton); a unique pointer
    // shall be declared
    static InitializedFileParser* initializedFileParserPointer;

    static InitializedFileParser* getInitializedFileParserInitialization();
    static POSIXErrors parseInitializedFile(const unsigned char*);
    static POSIXErrors getValueFromFileParser(const unsigned char*, unsigned char*);


    ~InitializedFileParser();
   private:
    InitializedFileParser();
};
}  // namespace Commons