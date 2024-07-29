#pragma once
/** @file MainHeaders.hpp
 * The headers and global variables from other package
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */
#include <netinet/if_ether.h>  // For Ethernet header structure
#include <netinet/ip.h>        // For IP header structure
#include <netinet/tcp.h>       // For TCP header structure
#include <netinet/udp.h>       // For UDP header structure
#include <signal.h>            // For the alarm and interrupted signal
#include <stdio.h>             // For some C io functions
#include <stdlib.h>            // For some C functions
#include <unistd.h>            // For sleep and pid functions

#include <algorithm>  // For std::max
#include <mutex>
#include <thread>

#include "../Models/Commons/Headers/IOSpecification.hpp"
#include "../Models/Commons/Headers/InitializedFileParser.hpp"
#include "../Models/Commons/Headers/POSIXErrors.hpp"
#include "../Models/Commons/Headers/Time.hpp"
#include "../Models/PCAP/Headers/LinuxPCAP.hpp"

namespace SysinMainCaller {
//===Function Declaration===
int start(int, char**);
void signalInterruptedHandler(int);
void signalAlarmHandler(int);

}  // namespace SysinMainCaller