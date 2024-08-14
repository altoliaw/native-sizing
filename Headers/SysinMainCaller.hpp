#pragma once
/** @file MainHeaders.hpp
 * The headers and global variables from other package
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#ifdef __linux__  // Linux

#include <netinet/if_ether.h>  // For Ethernet header structure
#include <netinet/ip.h>        // For IP header structure
#include <netinet/tcp.h>       // For TCP header structure
#include <netinet/udp.h>       // For UDP header structure
#include <unistd.h>            // For sleep and pid functions

#elif defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#endif

#include <signal.h>  // For the alarm and interrupted signal
#include <stdio.h>   // For some C io functions
#include <stdlib.h>  // For some C functions

#include <algorithm>  // For std::max
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../Models/Commons/Headers/IOSpecification.hpp"
#include "../Models/Commons/Headers/POSIXErrors.hpp"
#include "../Models/Commons/Headers/Time.hpp"
#include "../Models/FileParsers/Headers/InitializedJsonFileParser.hpp"
#include "../Models/PCAP/Headers/LinuxPCAP.hpp"

namespace SysinMainCaller {
struct unitService {
   public:
    char interfaceName[256];
    std::vector<int> port;
    unitService();
    ~unitService();
};

Commons::POSIXErrors start(int, char**);
void signalInterruptedHandler(int);
void signalAlarmHandler(int);
}  // namespace SysinMainCaller