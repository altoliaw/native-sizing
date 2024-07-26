#include "../Headers/Test_InitializedFileParser.hpp"

TEST(Commons, Test_InitializedFileParser_getInitializedFileParserInitialization) {
    Commons::InitializedFileParser* instance =
        Commons::InitializedFileParser::getInitializedFileParserInitialization();
    ASSERT_NE(nullptr, instance);
}

TEST(Commons, Test_InitializedFileParser_parseInitializedFile) {
    const unsigned char* path = (const unsigned char*)"../../../../.Ini/globalCompiling.ini";
    Commons::InitializedFileParser::parseInitializedFile(path);
	unsigned char value [256] = {'\0'};
	Commons::POSIXErrors error= Commons::InitializedFileParser::getValueFromFileParser((const unsigned char*)"compiler.WALL", value);
	ASSERT_EQ(error, Commons::POSIXErrors::OK);
	ASSERT_STREQ((char*)"-Wall", (char*)value);

	error= Commons::InitializedFileParser::getValueFromFileParser((const unsigned char*)"compiler.FMSG", value);
	ASSERT_EQ(error, Commons::POSIXErrors::OK);
	ASSERT_STREQ((char*)"-fmessage-length=0 -pthread", (char*)value);
}
