#include "../Headers/Test_InitializedJsonFileParser.hpp"

TEST(FileParsers, Test_InitializedJsonFileParser_getInitializedFileParserInitialization) {
    std::unique_ptr<FileParsers::InitializedJsonFileParser>& instance =
        FileParsers::InitializedJsonFileParser::getInitializedFileParserInitialization();
    ASSERT_NE(nullptr, instance);
    FileParsers::InitializedJsonFileParser::releaseInitializedFileParserInitialization();
}

TEST(FileParsers, Test_InitializedJsonFileParser_parseInitializedFile) {
    const unsigned char* path = (const unsigned char*)"../../../../Settings/.Json/SysinMain.json";
    Commons::POSIXErrors error = FileParsers::InitializedJsonFileParser::parseInitializedFile(path);
    ASSERT_EQ(Commons::POSIXErrors::OK, error);

    unsigned char value[1024] = {'\0'};
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base", value);
	ASSERT_EQ(Commons::POSIXErrors::OK, error);

	
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"\"base\"", value);
	ASSERT_NE(Commons::POSIXErrors::OK, error);

	cJSON* current = nullptr;
	error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.port", value, &current);
	/** The types are included cJSON_Object, cJSON_Array, cJSON_String, cJSON_Number, cJSON_True,
	 *  cJSON_False, cJSON_NULL and others.
	*/
	ASSERT_EQ(Commons::POSIXErrors::OK, error);
	ASSERT_EQ(current->type, cJSON_Array);

	current = nullptr;
	error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.port.[0]", value, &current);
	/** The types are included cJSON_Object, cJSON_Array, cJSON_String, cJSON_Number, cJSON_True,
	 *  cJSON_False, cJSON_NULL and others.
	*/
	ASSERT_EQ(Commons::POSIXErrors::OK, error);
	ASSERT_EQ(current->type, cJSON_Number);
	ASSERT_STREQ((char*)value, "1521");

    FileParsers::InitializedJsonFileParser::releaseInitializedFileParserInitialization();
}