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
    void* descriptor;                // PCAP descriptor
    std::string deviceInterface;     // The interface name
    int port;                        // The port number for distinguishing to the packets of rx an tx
    long long rxSize;                // The size of the rx channel
    long long txSize;                // The size of the rx channel
    long rxPacketNumber;             // The number of the packets in the rx channel
    long txPacketNumber;             // The number of the packets in the tx channel
    virtual void open(const char*, const int,
                      const int, const int, int) = 0;  // "open(.)" approach
    virtual void execute(void (*)(u_char*, const pcap_pkthdr*,
                                  const u_char*)) = 0;  // Executing the loop for obtaining the packets;
                                                        // here the type pcap_pkthdr is for user-defined function
    virtual void close(void) = 0;                       // "close(.)" approach
};

}  // namespace PCAP