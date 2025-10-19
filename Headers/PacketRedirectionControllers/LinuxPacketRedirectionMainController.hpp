#pragma once
/** @file LinuxPacketRedirectionMainController.hpp
 * The headers and global variables from other package for Linux programs
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#ifdef __linux__

#ifndef OS_ID

#include <netinet/if_ether.h>  // For Ethernet header structure
#include <netinet/ip.h>        // For IP header structure
#include <netinet/tcp.h>       // For TCP header structure
#include <netinet/udp.h>       // For UDP header structure
#include <signal.h>            // For the alarm and interrupted signal
#include <stdio.h>             // For some C io functions
#include <stdlib.h>            // For some C functions
#include <unistd.h>            // For sleep and pid functions
#include <arpa/inet.h>         // For inet_addr
#include <sys/socket.h>        // For socket functions

#include <algorithm>  // For std::max
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../../Models/Commons/Headers/POSIXErrors.hpp"
#include "../../Models/Commons/Headers/Time.hpp"
#include "../../Models/FileParsers/Headers/InitializedJsonFileParser.hpp"
#include "../../Models/PCAP/Headers/LinuxPCAP.hpp"
#include "../../Services/PacketRedirectionServices/Headers/LoadBalancer.hpp"
#include "../../Services/PacketRedirectionServices/Headers/RedirectionStatistics.hpp"
#include "./PacketRedirectionMainControllerPrototype.hpp"

namespace PacketRedirectionControllers {

class LinuxPacketRedirectionMainController : public PacketRedirectionMainControllerPrototype {
   public:
    Commons::POSIXErrors start(int argc, char** argv);

    static void signalInterruptedHandler(int signal);
    static void signalAlarmHandler(int signal);
    static Commons::POSIXErrors config(std::vector<CaptureConfig>* captureConfigs,
                                       std::vector<RedirectionTarget>* targets);
    static void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);
    static void packetTask(PCAP::LinuxPCAP* pcap,
                           void (*handler)(u_char*, const pcap_pkthdr*, const u_char*));
    static void statisticsFileTask(FILE** filePtr, const char* filePath);
};

}  // namespace PacketRedirectionControllers

#endif  // OS_ID
#endif  // __linux__
