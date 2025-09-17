# Linux compiling / Windows compiling
if(OPERATING_SYSTEM STREQUAL "0" OR OPERATING_SYSTEM STREQUAL "0.1")

	add_library(Sources.SizingMainCaller
		SizingMainCallerPrototype.cpp
		SizingMainCaller.cpp
		LinuxSizingMainCaller.cpp
		LinuxCentosSizingMainCaller.cpp
	)

	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	target_include_directories(Sources.SizingMainCaller PUBLIC 
		../../Headers/SizingController
		${STATIC_LIB_PATH}/libpcap/Includes)
	
	# To determine which code in the c++ file shall be joined because of different linux distributions
	if(OPERATING_SYSTEM STREQUAL "0.1")
		target_compile_definitions(Sources.SizingMainCaller PUBLIC OS_ID=${OS_ID})
	endif()
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