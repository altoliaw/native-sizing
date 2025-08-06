add_library(Models.FileParsers
	InitializedFileParser.cpp
	InitializedJsonFileParser.cpp
)

target_include_directories(Models.FileParsers 
	PUBLIC 
	${CMAKE_SOURCE_DIR}/Models/FileParsers/Headers 
	${CMAKE_SOURCE_DIR}/Models/Commons/Headers
)