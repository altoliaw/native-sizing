add_library(Models.FileParsers
	InitializedFileParser.cpp
)

target_include_directories(Models.FileParsers PUBLIC ../Headers ../../Commons/Headers)