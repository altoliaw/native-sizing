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
	fopen()

	return POSIXErrors::OK;
}

}  // namespace Commons