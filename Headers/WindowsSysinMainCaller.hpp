#pragma once
/** @file WindowsSysinMainCaller.hpp
 * The headers and global variables from other package for Linux programs
 *
 * @author Nick, Liao
 * @date 2024/08/15
 */

#ifdef _WIN32

#include <signal.h>  // For the alarm and interrupted signal
#include <stdio.h>   // For some C io functions
#include <stdlib.h>  // For some C functions

// Windows
#include <winsock2.h>  // This included library shall be put in the front of the one, namely "windows.h";
                       // otherwise warning information will be displayed when compiling
#include <windows.h>   // For capturing the interrupt signal

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
#include "../Models/PCAP/Headers/PCAPPrototype.hpp"
#include "../Models/PCAP/Headers/WindowsPCAP.hpp"
#include "./SysinMainCallerPrototype.hpp"

namespace SysinMainCaller {
class WindowsSysinMainCaller : public SysinMainCallerPrototype {
   public:
    Commons::POSIXErrors start(int, char**);

    static BOOL WINAPI signalInterruptedHandler(DWORD signal);
    // static void signalAlarmHandler(int);
    static Commons::POSIXErrors config(std::vector<unitService>*);
    // static void packetHandler(u_char*, const struct pcap_pkthdr*, const u_char*);
    // static void packetTask(PCAP::LinuxPCAP*, void (*)(u_char*, const pcap_pkthdr*, const u_char*));
    // static void packetFileTask(FILE**, const char*);
};
}  // namespace SysinMainCaller
#endif