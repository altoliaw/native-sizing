/**
 * @see InitializedJsonFileParser.hpp
 */
#include "../Headers/InitializedJsonFileParser.hpp"

namespace FileParsers {
// Initialization as nullptr to the static variable defined as the unique pointer in the class
std::unique_ptr<InitializedJsonFileParser> InitializedJsonFileParser::initializedFileParserPointer = nullptr;

/**
 * Constructor
 */
InitializedJsonFileParser::InitializedJsonFileParser() {
    initializedTable = nullptr;
    initializedTable = new Commons::HashTable();
}

/**
 * Destructor
 */
InitializedJsonFileParser::~InitializedJsonFileParser() {
    if (initializedTable != nullptr) {
        delete initializedTable;
        initializedTable = nullptr;
    }
}

/**
 * The static method for initializing the singleton
 *
 * @return [std::unique_ptr<InitializedJsonFileParser>&] The reference to the "InitializedFileParser" instance
 */
std::unique_ptr<InitializedJsonFileParser>& InitializedJsonFileParser::getInitializedFileParserInitialization() {
    // If the static pointer is nullptr, the pointer shall be referred to a static object defined in the function
    if (InitializedJsonFileParser::initializedFileParserPointer == nullptr) {
        InitializedJsonFileParser::initializedFileParserPointer.reset(new InitializedJsonFileParser());
    }

    return InitializedJsonFileParser::initializedFileParserPointer;
}

/**
 * The caller for releasing the memory of the singleton because when the singleton will be recreated in
 * the unit tests
 *
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors" The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors InitializedJsonFileParser::releaseInitializedFileParserInitialization() {
    // If the static pointer is not nullptr, the pointer shall be referred to a static object defined in the function
    if (InitializedJsonFileParser::initializedFileParserPointer != nullptr) {
        InitializedJsonFileParser::initializedFileParserPointer.reset();  // Destroying the instance/object; the destroyed process will executed the
                                                                          // destructor of the instance/object
        InitializedJsonFileParser::initializedFileParserPointer = nullptr;
    }
    return Commons::POSIXErrors::OK;
}

/**
 * Obtaining the setting contents from the source path; this function may be the bottleneck depending on
 * the complexity of the .json structure because the recursions are used in the method
 *
 * @param sourcePath [const unsigned char*] The absolute path of the source path
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors InitializedJsonFileParser::parseInitializedFile(const unsigned char* sourcePath) {
    // Creating the singleton by reference automatically, the function, getInitializedFileParserInitialization, will be
    // done once, even though the function, getInitializedFileParserInitialization(.) has been called many times
    std::unique_ptr<InitializedJsonFileParser>& initialedFileParserInstance = InitializedJsonFileParser::getInitializedFileParserInitialization();

    // Opening the specified file
    FILE* descriptor = fopen((const char*)sourcePath, "r+");
    if (descriptor == nullptr) {
        std::cerr << "No file exists\n";
        return Commons::POSIXErrors::E_EXIST;
    }

    // Calculating the size of the contents of the .json file
    fseek(descriptor, 0, SEEK_END);
    unsigned long length = ftell(descriptor);
    fseek(descriptor, 0, SEEK_SET);  // Resetting the file descriptor to the starting position

    // Dynamic memory allocation and reading the .json content
    unsigned char* jsonContent = new unsigned char[length + 1];
    fread(jsonContent, 1, length, descriptor);
    jsonContent[length] = '\0';

    // Closing the descriptor
    if (descriptor != nullptr) {
        fclose(descriptor);
    }

    // Parsing .json file recursively
    cJSON* jsonInstance = cJSON_Parse((char*)jsonContent);
    if (jsonInstance == nullptr) {  // JSON syntax is error.
        delete[] jsonContent;
        std::cerr << "JSON syntax is error\n";
        return Commons::POSIXErrors::E_EXIST;
    }

    std::vector<std::string> stack;
    // Removing elements if the stack is not empty
    if (stack.empty() == false) {
        stack.clear();
        stack.shrink_to_fit();
    }

    // Parsing the JSON
    initialedFileParserInstance->InitializedJsonFileParser::jsonParser(jsonInstance, &stack);
    // unsigned char key[1024] = {'\0'};
    // unsigned char value[2048] = {'\0'};

    delete[] jsonContent;
    return Commons::POSIXErrors::OK;
}

/**
 * The JSON parser; all attributes will be parsed recursively
 *
 * @param item [cJSON*] The tuple in the .json structure
 * @param stack [std::vector<std::string>*] The stack implementation for generating key for the hash table
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors InitializedJsonFileParser::jsonParser(cJSON* item, std::vector<std::string>* stack) {
    // Determining if the existence of the current item
    if (item == nullptr) {
        return Commons::POSIXErrors::OK;
    }

    // Considering the current item type
    switch (item->type) {
        case cJSON_Object: {
            std::cout << "Object:" << std::endl;
            cJSON* child = item->child;
            while (child) {
                std::cout << "Key: " << child->string << " - ";
                InitializedJsonFileParser::jsonParser(child, stack);
                child = child->next;
            }
            break;
        }
        case cJSON_Array: {
            std::cout << "Array:" << std::endl;
            int size = cJSON_GetArraySize(item);
            for (int i = 0; i < size; ++i) {
                cJSON* element = cJSON_GetArrayItem(item, i);
                std::cout << "Index " << i << ": ";
                InitializedJsonFileParser::jsonParser(element, stack);
            }
            break;
        }
        case cJSON_String: {
            std::cout << "String: " << item->valuestring << std::endl;
            break;
        }
        case cJSON_Number: {
            std::cout << "Number: " << item->valuedouble << std::endl;
            break;
        }
        case cJSON_True:
        case cJSON_False: {
            std::cout << "Boolean: " << (item->type == cJSON_True ? "true" : "false") << std::endl;
            break;
        }
        case cJSON_NULL: {
            std::cout << "Null" << std::endl;
            break;
        }
        default:
            std::cout << "Unknown type" << std::endl;
            break;
    }
}

/**
 * Obtaining the value from the hash table
 *
 * @param columnName [const unsigned char*] The key of the element in the hash table
 * @param value [unsigned char*] The pointer to the value in the hash table; the value is searched from the columnName;
 * In addition, the value shall be assigned a static memory space
 * @return [Commons::POSIXErrors] The successful flag
 */
Commons::POSIXErrors InitializedJsonFileParser::getValueFromFileParser(const unsigned char* columnName, unsigned char* value) {
    // Creating the singleton by reference automatically, the function, getInitializedFileParserInitialization, will be
    // done once, even though the function, getInitializedFileParserInitialization(.) has been called many times
    std::unique_ptr<InitializedJsonFileParser>& initialedFileParserInstance = InitializedJsonFileParser::getInitializedFileParserInitialization();

    unsigned char* copiedColumnNameAddress = nullptr;                                              // The pointer for referring to the columnName defined in the hash table
    void* valuePointer = nullptr;                                                                  // The pointer for referring to the value which is searching by using the columnName
    size_t valueSize = 0;                                                                          // The memory size of the value
    Commons::HashTable::ElementType type = Commons::HashTable::ElementType::unsignedCharStarType;  // The data type of the value

    // Obtaining the value
    char isExisted = initialedFileParserInstance->initializedTable->getValueByName(
        (char*)columnName, (char**)(&copiedColumnNameAddress), &valuePointer, &valueSize, &type);

    // When the key does not exist in the hash table, ...
    if (isExisted == 0x0) {
        // std::cerr << "There is no item in the hash table.\n";
        return Commons::POSIXErrors::E_NOITEM;
    }

    // When the type is equal to the "unsigned char*"
    if (type == Commons::HashTable::ElementType::unsignedCharStarType) {
        memcpy((void*)value, valuePointer, valueSize);
        value[valueSize] = '\0';  // For ensuring that the '\0' will be appeared
    } else {
        // Do nothing
        return Commons::POSIXErrors::E_NOITEM;
    }

    return Commons::POSIXErrors::OK;
}

}  // namespace FileParsers