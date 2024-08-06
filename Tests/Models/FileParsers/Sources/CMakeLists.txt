# Creating an execution in each model
add_executable(
	Tests.FileParsers
	Test_InitializedFileParser.cpp
)
	
# linking to the headers
target_include_directories(Tests.FileParsers PUBLIC ../Headers ../../../../Models/Commons/Headers ../../../../Models/FileParsers/Headers)

# linking to the gtest library
target_link_libraries(
	Tests.FileParsers
	Models.FileParsers
	Models.Commons
	GTest::gtest_main
)
		
include(GoogleTest)

# Registering the tests
add_test(NAME Tests.FileParsers COMMAND Tests.FileParsers --gtest_color=yes)