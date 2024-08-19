add_library(Main.Sources
	SysinMainCallerPrototype.cpp
	SysinMainCaller.cpp
	WindowsSysinMainCaller.cpp
)

set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

target_include_directories(Main.Sources PUBLIC 
	../Headers 
	${STATIC_LIB_PATH}/nPcap/Includes)