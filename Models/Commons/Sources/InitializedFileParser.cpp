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
        initializedTable = nullptr;
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
    // Creating the singleton automatically, the function, getInitializedFileParserInitialization, will be 
    // done once, even though the function, parseInitializedFile(.) has been called many times
    InitializedFileParser* initialedFileParserInstance = InitializedFileParser::getInitializedFileParserInitialization();

    // Opening the specified file
    FILE* descriptor = fopen((const char*)sourcePath, "r+");
    if (descriptor == nullptr) {
        std::cerr<< "No file exists\n";
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
        length = (length > 0) ? StringImplement::obtainNoNewLineSignAtTheEnd((unsigned char**)(&linePointer)) : 0;

        if (length <= 0) {  // Empty line
            continue;
        } else if (length > 0 && linePointer[0] == '#') {  // Commented line
            continue;
        } else if (length > 0 && linePointer[0] == '[') {  // Section part
            memcpy(section, (unsigned char*)linePointer, length);
            // Verifying if the last character is ']'
            if (section[length - 1] == ']') {
                section[length - 1] = '\0';
            } else {
                section[length] = '\0';
            }
        } else {  // Key-Value part
            int sectionLength = (int)strlen((char*)section);
            if (sectionLength <= 0) {
                std::cerr << "The key-value pair has not been denoted in the section phase\n";
                return POSIXErrors::E_NOITEM;
            }

            // Accessing the key value pairs; spliting the key and value by using the character '='
            int delimiterIndex = -1;
            for (int i = 0; i < length; i++) {
                if (linePointer[i] == '=') {
                    delimiterIndex = i;
                    break;
                }
            }

            // The delimiter does not exist in the sentence.
            if (delimiterIndex == -1) {
                std::cerr << "The delimiter does not exist in the sentence\n";
                return POSIXErrors::E_NOITEM;
            }

            // Key preparing; the key will be denoted as the syntax- "$sectionName.$key";
            // for example, given the section name is "compile" and the key name is "base", the final key will be
            // equal to "compile.base"
            memcpy(key, section, sectionLength);
            key[sectionLength] = '.';
            memcpy(key + sectionLength + 1, (unsigned char*)linePointer, delimiterIndex);  // The last argument is from "delimiterIndex - 1 - 0 + 1"
            key[sectionLength + 1 + delimiterIndex] = '\0';

            // Value preparing (adjusting the value array)
            memmove(value, value + delimiterIndex + 1, length - (delimiterIndex + 1) );
            value[length - (delimiterIndex + 1)] = '\0';

            // Verifying the instance
            if (initialedFileParserInstance == nullptr) {
                std::cerr << "No InitialedFileParser instance\n";
            }
            // Putting the K-V pair into the hash table; because the value belongs to the string,
            // the size shall be included the size of '\0'
            initialedFileParserInstance->initializedTable->addElementIntoHashTable(
                (char*)key, (void*)value, (size_t)(length - (delimiterIndex + 1) + 1), HashTable::ElementType::unsignedCharType
            );
        }
    }

    // Closing the descriptor
    if (descriptor != nullptr) {
        fclose(descriptor);
    }
    return POSIXErrors::OK;
}

}  // namespace Commons