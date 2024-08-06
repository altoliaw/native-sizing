#include "../Headers/Test_InitializedJsonFileParser.hpp"

TEST(FileParsers, Test_InitializedJsonFileParser_getInitializedFileParserInitialization) {
    std::unique_ptr<FileParsers::InitializedJsonFileParser>& instance =
    	FileParsers::InitializedJsonFileParser::getInitializedFileParserInitialization();
    ASSERT_NE(nullptr, instance);
	FileParsers::InitializedJsonFileParser::releaseInitializedFileParserInitialization();
}

TEST(FileParsers, Test_InitializedJsonFileParser_parseInitializedFile) {
    const unsigned char* path = (const unsigned char*)"../../../../Settings/.Json/SysinMain.json";
    FileParsers::InitializedJsonFileParser::parseInitializedFile(path);
	ASSERT_EQ(true, true);
	FileParsers::InitializedJsonFileParser::releaseInitializedFileParserInitialization();
}