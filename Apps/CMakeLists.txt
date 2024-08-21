if (NOT BUILD_TEST)
	# For windows compiling because some third party libraries are made by MSVC
	# Creating an execution in each model
	add_executable(
		SysinMain_Windows
		SysinMain.cpp
	)
		
	# Linking to the library
	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	# Linking to the headers
	target_include_directories(SysinMain_Windows PUBLIC ../Headers)

	# Linking to the gtest library
	target_link_libraries(
		SysinMain_Windows
		Main.Sources
		Models.Commons
		Models.FileParsers
		Models.PCAP
		# Windows built-in libraries
		ws2_32
		# Vendor-specific libraries
		${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
		${STATIC_LIB_PATH}/nPcap/Libs/x64/Packet.lib
		${STATIC_LIB_PATH}/nPcap/Libs/x64/wpcap.lib
	)
endif()