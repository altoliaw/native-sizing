/**
 * @see LinuxPCAP.hpp
 */
#include "../Headers/LinuxPCAP.hpp"

namespace PCAP {
/**
 * Constructor
 */
LinuxPCAP::LinuxPCAP() {
    handle = NULL;
	pcapHandle = (pcap_t*) handle;
    deviceInterface = "";
    rxSize = 0;
    txSize = 0;
    rxPacketNumber = 0;
    txPacketNumber = 0;
}

/**
 * Destructor
 */
LinuxPCAP::~LinuxPCAP() {
	// PCAP handle shall be closed and NULL.
    if (pcapHandle != nullptr) {
        this->close();
    } else {
        pcapHandle = nullptr;
		handle = nullptr;
    }
    deviceInterface = "";
    rxSize = 0;
    txSize = 0;
    rxPacketNumber = 0;
    txPacketNumber = 0;
}

/**
 * Opening the PCAP
 * 
 * @param device [const char*] The interface
 * @param snaplen [const int] The upper bound of the bits for each packet
 * @param promisc [const int] Specifying that the device is to be put into promiscuous mode. 
 * A value of 1 (True) turns promiscuous mode on.
 * @param timeout [const int] Timeout (milliseconds)
 */
void LinuxPCAP::open(const char* device, const int snaplen, const int promisc, const int timeout) {
	pcapHandle = pcap_open_live(device, snaplen, promisc, timeout, this->errBuff);
	handle = (void*)pcapHandle;
}

/**
 * Looping for obtaining packets
 */
void LinuxPCAP::execute() {
    pcap_loop(pcapHandle, 0, LinuxPCAP::packetHandler, reinterpret_cast<u_char*>(&rxPacketNumber));
}

/**
 * Closing the PCAP
 */
void LinuxPCAP::close(void) {
	pcap_close(pcapHandle);
}


/**
 * Calculating the amount of the packets
 * 
 * @param userData [u_char*]
 * @param pkthdr [const struct pcap_pkthdr*]
 * @param packet [const u_char*]
 */
static void packetHandler (u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    int* packetCount = (int*)userData;
    (*packetCount)++;

    long* totalSize = (long*)(userData + sizeof(int));
    *totalSize += pkthdr->len;
}

}  // namespace PCAP