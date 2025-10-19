# When the type does not belong to the unit test
if (NOT BUILD_TEST)

	add_executable(
		PacketRedirectionMain
		PacketRedirectionMain.cpp
	)

	add_custom_command(TARGET PacketRedirectionMain POST_BUILD
		# Copying the original .exe to the Bin directory
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"$<TARGET_FILE:PacketRedirectionMain>"
			"${CMAKE_SOURCE_DIR}/Bin/"
	)

	# Linking to the headers
	target_include_directories(PacketRedirectionMain PUBLIC ${CMAKE_SOURCE_DIR}/Headers/PacketRedirectionControllers)

	# Linking to the library
	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	# Linux compiling / Windows compiling
	if(OPERATING_SYSTEM STREQUAL "0" OR OPERATING_SYSTEM STREQUAL "0.1")

		if(OPERATING_SYSTEM STREQUAL "0.1") # CentOS version
			target_link_libraries(
				PacketRedirectionMain
				Sources.PacketRedirectionControllers
				Services.PacketRedirection
				Models.Commons
				Models.FileParsers
				Models.PCAP
				# Linux built-in libraries
				pthread
				# Vendor-specific libraries
				${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
				${STATIC_LIB_PATH}/libpcap/Libs/libpcap.a
				-lnl-3 -lnl-genl-3
			)
		else() # Other Linux version
			target_link_libraries(
				PacketRedirectionMain
				Sources.PacketRedirectionControllers
				Services.PacketRedirection
				Models.Commons
				Models.FileParsers
				Models.PCAP
				# Linux built-in libraries
				pthread
				# Vendor-specific libraries
				${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
				${STATIC_LIB_PATH}/libpcap/Libs/libpcap.a
			)
		endif()

		# Changing the permission of the PacketRedirectionMain
		add_custom_command(TARGET PacketRedirectionMain POST_BUILD
			COMMAND sudo chown root:root "${CMAKE_SOURCE_DIR}/Bin/PacketRedirectionMain"
			COMMAND sudo chmod 4755 "${CMAKE_SOURCE_DIR}/Bin/PacketRedirectionMain"
		)

	else()
		# Creating an execution in each model
		set(CMAKE_EXE_LINKER_FLAGS "-static")
		# For windows compiling because some third party libraries are made by MSVC
		# Linking to the all library
		target_link_libraries(
			PacketRedirectionMain -static
			Sources.PacketRedirectionControllers
			Services.PacketRedirection
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
