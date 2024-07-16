/**
 * @see InitializedFileParser.hpp
 */
#include "../Headers/InitializedFileParser.hpp"

namespace Commons {
// Initialization the nullptr to the static variable in the class
InitializedFileParser* InitializedFileParser::initializedFileParserPointer = nullptr;

/**
 * Constructor
 */
InitializedFileParser::InitializedFileParser() {
    initializedTable = nullptr;
    initializedTable = new HashTable();
}

/**
 * Destructor
 */
InitializedFileParser::~InitializedFileParser() {
    if (initializedTable != nullptr) {
        delete initializedTable;
    }
}

/**
 * The static method for initializing the singleton
 *
 * @return [InitializedFileParser*] The pointer to the "InitializedFileParser" instance
 */
InitializedFileParser* InitializedFileParser::getInitializedFileParserInitialization() {
    // A static instance created in function
    static InitializedFileParser initializedFileParser;

    // If the static pointer is nullptr, the pointer shall be referred to a static object defined in the function
    if (InitializedFileParser::initializedFileParserPointer == nullptr) {
        InitializedFileParser::initializedFileParserPointer = &initializedFileParser;
    }

    return InitializedFileParser::initializedFileParserPointer;
}

/**
 * Obtaining the setting contents from the source path
 *
 * @param sourcePath [const unsigned char*] The absolute path of the source path
 * @return [POSIXErrors] The status defined in the class "POSIXErrors"
 */
POSIXErrors InitializedFileParser::parseInitializedFile(const unsigned char* sourcePath) {
    // Opening the specified file
    FILE* descriptor = fopen((const char*)sourcePath, "r+");
    if (descriptor == nullptr) {
        return POSIXErrors::E_EXIST;
    }

    // Section & Key, and value parts
    unsigned char section[512] = {'\0'};
    unsigned char key[1024] = {'\0'};
    unsigned char value[2048] = {'\0'};
    char* linePointer = (char*)value;
    size_t lineBufferLength = sizeof(value) / sizeof(unsigned char);

    while (fgets(linePointer, sizeof(value) / sizeof(unsigned char), descriptor) != nullptr) {
        unsigned int length = strlen(linePointer);
        // Trimming the spaces
        length = (length > 0) ? StringImplement::trimLeftSpace((unsigned char**)(&linePointer)) : 0;
        length = (length > 0) ? StringImplement::trimRightSpace((unsigned char**)(&linePointer)) : 0;

        if (length <= 0) { // Empty line
            continue;
        } else if (length > 0 && linePointer[0] == '#') {// Commented line
            continue;
        } else if (length > 0 && linePointer[0] == '[') {// Section part
            memcpy(section, (unsigned char*)linePointer, length);
            section[length] ='\0';
        }else { // Key-Value part
            //TODO
        }
    }

    if (descriptor != nullptr) {
        fclose(descriptor);
    }
    return POSIXErrors::OK;
}

}  // namespace Commons