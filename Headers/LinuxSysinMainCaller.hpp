#pragma once
/** @file LinuxSysinMainCaller.hpp
 * The headers and global variables from other package for Linux programs
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#ifdef __linux__
#include <netinet/if_ether.h>  // For Ethernet header structure
#include <netinet/ip.h>        // For IP header structure
#include <netinet/tcp.h>       // For TCP header structure
#include <netinet/udp.h>       // For UDP header structure
#include <unistd.h>            // For sleep and pid functions

#include <signal.h>            // For the alarm and interrupted signal
#include <stdio.h>             // For some C io functions
#include <stdlib.h>            // For some C functions

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
#include "./SysinMainCallerPrototype.hpp"

namespace SysinMainCaller {
class LinuxSysinMainCaller : public SysinMainCallerPrototype {
   public:
    Commons::POSIXErrors start(int, char**);
    void signalInterruptedHandler(int);
    void signalAlarmHandler(int);
};
}  // namespace SysinMainCaller
#endif