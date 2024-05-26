#pragma once
/** @file LinuxPCAP.hpp
 * The PCAP class for Linux
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#include <iostream>
#include <string>
#include <unistd.h>
// The linux's pcap file, not PF-RING's one
#include <pcap.h>

#include "./PCAPPrototype.hpp"

namespace PCAP {
/**
 * The abstract class as the parent class
 */
class LinuxPCAP : public PCAPPrototype {
   public:
    pcap_t* pcapDescriptor;  // The pointer for referring to the variable "handle" from the prototype
    LinuxPCAP();
    ~LinuxPCAP();
    void open(const char*, const int, const int, const int, int);
    void execute(void (*)(u_char*, const pcap_pkthdr*, const u_char*) = nullptr);
    void close(void);

   private:
    static void packetHandler(u_char*, const struct pcap_pkthdr*, const u_char*);
};

}  // namespace PCAP