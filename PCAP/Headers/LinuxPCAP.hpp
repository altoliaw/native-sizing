#pragma once
/** @file LinuxPCAP.hpp
 * The PCAP class for Linux
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#include <string>
// The linux's pcap file, not PF-RING's one
#include <pcap.h>

#include "./PCAPPrototype.hpp"

namespace PCAP {
/**
 * The abstract class as the parent class
 */
class LinuxPCAP : public PCAPPrototype {
   public:
	pcap_t* pcapHandle; // The pointer for referring to the variable "handle" from the prototype
    LinuxPCAP();
    ~LinuxPCAP();
    void open(const char*, const int, const int, const int);
    void close(void);
};

}  // namespace PCAP