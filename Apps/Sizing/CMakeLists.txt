# When the type does not belong to the unit test
if (NOT BUILD_TEST)

	add_executable(
		SizingMain
		SizingMain.cpp
	)

	add_custom_command(TARGET SizingMain POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SizingMain>"
        "${CMAKE_SOURCE_DIR}/Bin/"
	)
	
	
	# Linking to the headers
	target_include_directories(SizingMain PUBLIC ../Headers/SizingController)

	# Linking to the library
	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	# Linux compiling / Windows compiling
	if(OPERATING_SYSTEM STREQUAL "0")
		target_link_libraries(
			SizingMain
			Main.Sources
			Models.Commons
			Models.FileParsers
			Models.PCAP
			# Linux built-in libraries
			pthread
			# Vendor-specific libraries
			${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
			${STATIC_LIB_PATH}/libpcap/Libs/libpcap.a
		)
	else()
		# Creating an execution in each model
		set(CMAKE_EXE_LINKER_FLAGS "-static")
		# For windows compiling because some third party libraries are made by MSVC
		# Linking to the all library
		target_link_libraries(
			SizingMain -static
			Main.Sources
			Models.Commons
			Models.FileParsers
			Models.PCAP
			# Windows built-in libraries
			iphlpapi
			ws2_32
			# Vendor-specific libraries
			${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
			${STATIC_LIB_PATH}/winDivert/Libs/WinDivert.lib
		)
	endif()
endif()