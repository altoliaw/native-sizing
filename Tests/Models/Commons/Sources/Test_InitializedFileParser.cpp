#include "../Headers/Test_InitializedFileParser.hpp"

TEST(Commons, Test_InitializedFileParser_getInitializedFileParserInitialization) {
	Commons::InitializedFileParser* instance = 
		Commons::InitializedFileParser::getInitializedFileParserInitialization();
    ASSERT_NE(nullptr, instance);
}

TEST(Commons, Test_InitializedFileParser_parseInitializedFile) {
	const unsigned char* path = (const unsigned char*)"../../../../.Ini/globalCompiling.ini";
	Commons::InitializedFileParser::parseInitializedFile(path);
    
}

