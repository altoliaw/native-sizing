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
else() # For Windows
	if(WINDOWS_PCAP_KERNEL STREQUAL "NPCAP")
		add_library(Models.PCAP
			PCAPPrototype.cpp
			WindowsNpcapPCAP.cpp
		)
		target_include_directories(Models.PCAP PUBLIC
			${CMAKE_SOURCE_DIR}/Models/PCAP/Headers
			${STATIC_LIB_PATH}/npcap/Includes
		)
	elseif(WINDOWS_PCAP_KERNEL STREQUAL "WINDIVERT")
		add_library(Models.PCAP
			PCAPPrototype.cpp
			WindowsPCAP.cpp
		)
		target_include_directories(Models.PCAP PUBLIC
			${CMAKE_SOURCE_DIR}/Models/PCAP/Headers
			${STATIC_LIB_PATH}/winDivert/Includes
		)
		target_compile_definitions(Models.PCAP PUBLIC USE_WINDIVERT_KERNEL)
	else()
		message(FATAL_ERROR "No valid WINDOWS_PCAP_KERNEL specified. Choose NPCAP or WINDIVERT.")
	endif()
endif()