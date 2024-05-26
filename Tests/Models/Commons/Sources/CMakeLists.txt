# Opening the subdirectory for the next works
# add_library(Commons
# 	Test_StringImplement.cpp
# )

# Creating an execution in each model
add_executable(
	Test.Commons
	Test_StringImplement.cpp
)
	
# linking to the headers
target_include_directories(Test.Commons PUBLIC ../Headers ../../../../Models/Commons/Headers)

# linking to the gtest library
target_link_libraries(
	Test.Commons
	Models.Commons
	GTest::gtest_main
)
		
include(GoogleTest)

# Registering the test
gtest_discover_tests(Test.Commons)