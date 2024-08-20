# For windows compiling because some third party libraries are made by MSVC
# Creating an execution in each model
add_executable(
	Apps.SysinMain
	SysinMain.cpp
)
	
# Linking to the library
set(STATIC_LIB_PATH ${CMAKE_BINARY_DIR}/Vendors)

# Linking to the headers
target_include_directories(Apps.SysinMain PUBLIC ../Headers)

# Linking to the gtest library
target_link_libraries(
	Apps.SysinMain
	Main.Sources
	Models.Commons
	Models.FileParsers
	Models.PCAP
	${STATIC_LIB_PATH}/cJson/Libs/libcjson.a
	${STATIC_LIB_PATH}/nPcap/Libs/x64/Packet.lib
	${STATIC_LIB_PATH}/nPcap/Libs/x64/wpcap.lib
)