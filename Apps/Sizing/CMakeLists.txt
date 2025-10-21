# When the type does not belong to the unit test
if (NOT BUILD_TEST)

	add_executable(
		SizingMain
		SizingMain.cpp
	)


	
	# Linking to the headers
	target_include_directories(SizingMain PUBLIC ${CMAKE_SOURCE_DIR}/Headers/SizingControllers)

	# Linking to the library
	set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

	# Linux compiling / Windows compiling
	if(OPERATING_SYSTEM STREQUAL "0" OR OPERATING_SYSTEM STREQUAL "0.1")
	
		if(OPERATING_SYSTEM STREQUAL "0.1") # CentOS version
			target_link_libraries(
				SizingMain
				Sources.SizingControllers
				Services.Sizing
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
				SizingMain
				Sources.SizingControllers
				Services.Sizing
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

		# Changing the permission of the SizingMain
		add_custom_command(TARGET SizingMain POST_BUILD
        	COMMAND sudo chown root:root "${CMAKE_SOURCE_DIR}/Bin/SizingMain"
        	COMMAND sudo chmod 4755 "${CMAKE_SOURCE_DIR}/Bin/SizingMain"
    	)

	else()
		# Creating an execution in each model
		set(CMAKE_EXE_LINKER_FLAGS "-static")
		# For windows compiling because some third party libraries are made by MSVC
		# Linking to the all library
		target_link_libraries(
			SizingMain
			PUBLIC
				Sources.SizingControllers
				Models.Commons
				Models.FileParsers
				Models.PCAP
				# Windows built-in libraries
				iphlpapi
				ws2_32
				# Vendor-specific libraries
				${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
		)

		if(WINDOWS_PCAP_KERNEL STREQUAL "WINDIVERT")
			target_link_libraries(SizingMain PUBLIC ${STATIC_LIB_PATH}/winDivert/Libs/WinDivert.lib)
		elseif(WINDOWS_PCAP_KERNEL STREQUAL "NPCAP")
			target_link_libraries(SizingMain PUBLIC ${STATIC_LIB_PATH}/nPcap/Libs/x64/wpcap.lib)
		endif()
	endif()
endif()