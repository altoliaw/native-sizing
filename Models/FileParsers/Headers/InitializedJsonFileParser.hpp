#pragma once
/** @file InitializedJsonFileParser.hpp
 * A class for parsing the key and value pairs from the .json file; this class will be implemented
 * in a singleton pattern; for unit testing the singleton pattern in various unit tests, the unique
 * pointer shall be used. This is because the static pointer shall refer to the dynamic memory manually and
 * the memory shall be released manually; if the pointer is a normal pointer and refers to a static instance
 * in the initialization function, the dynamic memory released phase can not be implemented
 *
 * @author Nick, Liao
 * @date 2024/08/05
 * @note The file is dependent to the Models.Commons
 * @note The file is dependent to the third party software, cJson
 */
#include <stdlib.h>
#include <string.h>

#include <memory>  // For the unique pointer
#include <vector>  // To simulate the stack container
#include <string>

#include "../../../Vendors/Includes/cJSON.h"
#include "../../Commons/Headers/HashTable.hpp"
#include "../../Commons/Headers/StringImplement.hpp"

namespace FileParsers {
/**
 * Several InitializedFileParser implementation (singleton)
 */
class InitializedJsonFileParser {
   public:
    // For referring to the hash table, and the hash table will be assigned
    // by using the dynamic memory allocation
    Commons::HashTable* initializedTable;

    // A static variable for the object from the class (singleton); a unique pointer
    // shall be declared
    static std::unique_ptr<InitializedJsonFileParser> initializedFileParserPointer;

    static std::unique_ptr<InitializedJsonFileParser>& getInitializedFileParserInitialization();
    static Commons::POSIXErrors releaseInitializedFileParserInitialization();
    static Commons::POSIXErrors parseInitializedFile(const unsigned char*);
    static Commons::POSIXErrors getValueFromFileParser(const unsigned char*, unsigned char*);

    ~InitializedJsonFileParser();

   private:
    InitializedJsonFileParser();
    Commons::POSIXErrors jsonParser(cJSON*, std::vector<std::string>*);
};
}  // namespace FileParsers