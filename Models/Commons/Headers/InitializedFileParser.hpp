#pragma once
/** @file InitializedFileParser.hpp
 * A class for parsing the key and value pairs from the .ini file
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

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

    // A static variable for the object from the class (singleton)
    static InitializedFileParser* initializedFileParserPointer;

    static InitializedFileParser* getInitializedFileParserInitialization();
    static POSIXErrors parseInitializedFile(const unsigned char*);
    static POSIXErrors getValueFromFileParser(const unsigned char*, unsigned char*);


    ~InitializedFileParser();
   private:
    InitializedFileParser();
};
}  // namespace Commons