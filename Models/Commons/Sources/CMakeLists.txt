add_library(Models.Commons
	StringImplement.cpp
	HashTable.cpp
	InitializedFileParser.cpp
)

target_include_directories(Models.Commons PUBLIC ../Headers)