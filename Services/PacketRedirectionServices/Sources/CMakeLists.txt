add_library(Services.PacketRedirection
	LoadBalancer.cpp
	RedirectionStatistics.cpp
)

target_include_directories(Services.PacketRedirection PUBLIC
	../Headers
	../../../Headers/PacketRedirectionControllers
)
