#pragma once
/** @file WindowsPCAP.hpp
 * The PCAP class for Windows
 *
 * @author Nick, Liao
 * @date 2024/08/19
 */
#ifdef _WIN32  // Windows
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>

#include "./PCAPPrototype.hpp"

/* This is for the default buffer size for displaying the network information by using window API. */
#define OUT_BUFF_lENGTH 15000
namespace PCAP {
/**
 * The inherited class to the parent class, PCAPPrototype
 */
class WindowsPCAP : public PCAPPrototype {
   public:
    HANDLE pcapDescriptor;  // The pointer for referring to the variable "handle" from the prototype
    WindowsPCAP();
    virtual ~WindowsPCAP();
    void open(const char*, const int, const int, const int, std::vector<int>*);
    void execute(void (*)(u_char*, const pcap_pkthdr*, const u_char*) = nullptr);
    void close(void);
    static void show(void);


   private:
    static void packetHandler(u_char*, const pcap_pkthdr*, const u_char*);
    static void pcap_loop(HANDLE*, int, void (*)(u_char*, const pcap_pkthdr*, const u_char*), u_char*);
};

}  // namespace PCAP
#endif