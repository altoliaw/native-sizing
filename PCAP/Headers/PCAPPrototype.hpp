#pragma once
/** @file PCAPPrototype.hpp
 * The abstract class of PCAP
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

// The linux's pcap file, not PF-RING's one
#include <pcap.h>

#include <string>

namespace PCAP {
/**
 * The abstract class as the parent class
 */
class PCAPPrototype {
   public:
    char errBuff[PCAP_ERRBUF_SIZE];  // For error message
    void* handle;                    // PCAP handler
    std::string deviceInterface;     // The interface name
    long rxSize;                     // The size of the rx channel
    long txSize;                     // The size of the rx channel
    long rxPacketNumber;             // The number of the packets in the rx channel
    long txPacketNumber;             // The number of the packets in the rx channel
    virtual void open(const char*, const int,
                      const int, const int) = 0;  // "open(.)" approach
                                                  //  virtual void getRX(std::string) = 0;  // Obtaining the contents of the RX
                                                  //	virtual void getTX(std::string) = 0;  // Obtaining the contents of the TX
    virtual void close(void) = 0;                 // "close(.)" approach
};

}  // namespace PCAP