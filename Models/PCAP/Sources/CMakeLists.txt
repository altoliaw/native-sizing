set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

# Linux compiling / Windows compiling
if(OPERATING_SYSTEM STREQUAL "0" OR OPERATING_SYSTEM STREQUAL "0.1")
	add_library(Models.PCAP
		PCAPPrototype.cpp
		LinuxPCAP.cpp
	)

	target_include_directories(Models.PCAP 
		PUBLIC 
		${CMAKE_SOURCE_DIR}/Models/PCAP/Headers 
		${STATIC_LIB_PATH}/libpcap/Includes
	)
else()
	add_library(Models.PCAP
		PCAPPrototype.cpp
		WindowsPCAP.cpp
	)

	target_include_directories(Models.PCAP PUBLIC 
	${CMAKE_SOURCE_DIR}/Models/PCAP/Headers 
		${STATIC_LIB_PATH}/winDivert/Includes
	)
endif()