# Linux compiling / Windows compiling
if(OPERATING_SYSTEM STREQUAL "0")

	add_library(Sources.SizingMainCaller
		SizingMainCallerPrototype.cpp
		SizingMainCaller.cpp
		LinuxSizingMainCaller.cpp
	)

	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	target_include_directories(Sources.SizingMainCaller PUBLIC 
		../../Headers/SizingController
		${STATIC_LIB_PATH}/libpcap/Includes)

else()
	add_library(Sources.SizingMainCaller
		SizingMainCallerPrototype.cpp
		SizingMainCaller.cpp
		WindowsSizingMainCaller.cpp
	)

	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	target_include_directories(Sources.SizingMainCaller PUBLIC 
		../../Headers/SizingController
		${STATIC_LIB_PATH}/WinDivert/Includes)
endif()