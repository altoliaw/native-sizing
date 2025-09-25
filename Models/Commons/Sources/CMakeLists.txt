add_library(Models.Commons
	StringImplement.cpp
	IOExporters.cpp
	HashTable.cpp
	Time.cpp
)

target_include_directories(Models.Commons PUBLIC ${CMAKE_SOURCE_DIR}/Models/Commons/Headers)