add_library(Main.Sources
	SysinMainCallerPrototype.cpp
	SysinMainCaller.cpp
	LinuxSysinMainCaller.cpp
)

target_include_directories(Main.Sources PUBLIC ../Headers)