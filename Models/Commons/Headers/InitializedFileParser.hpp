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
 * Several string implementation (singleton)
 */
class InitializedFileParser {
   public:
    // For referring to the hash table
    Commons::HashTable* initializedTable;

    // A static variable for the object from the class (singleton)
    static InitializedFileParser* initializedFileParserPointer;

    static InitializedFileParser* getInitializedFileParserInitialization();
    static char parseInitialFile(const char*);
    static void* getValueFromFileParser(const char*);

    ~InitializedFileParser();

   private:
    InitializedFileParser();
};
}  // namespace Commons