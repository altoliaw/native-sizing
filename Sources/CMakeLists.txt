# add_library(Main.Sources
# 	SysinMainCallerPrototype.cpp
# 	SysinMainCaller.cpp
# 	LinuxSysinMainCaller.cpp
# )

# if (WIN32)
# 	target_include_directories(Main.Sources PUBLIC 
# 	../Headers 
# 	../Vendors/nPcap/Includes)
# 	message(STATUS "Configuring include directories for Windows")
# elseif (UNIX AND NOT APPLE)
# 	# Linux platforms
# 	target_include_directories(Main.Sources PUBLIC ../Headers)
# endif()
