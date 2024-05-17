/**
 * @see LinuxPCAP.hpp
 */
#include "../Headers/LinuxPCAP.hpp"

namespace PCAP {
/**
 * Constructor
 */
LinuxPCAP::LinuxPCAP() {
    descriptor = nullptr;
    pcapDescriptor = (pcap_t*)descriptor;
    deviceInterface = "";
    rxSize = 0;
    txSize = 0;
    rxPacketNumber = 0;
    txPacketNumber = 0;
    errBuff[0] = '\0';
}

/**
 * Destructor
 */
LinuxPCAP::~LinuxPCAP() {
    // PCAP handle shall be closed and NULL.
    if (pcapDescriptor != nullptr) {
        std::cerr << "Error.\n";
        this->close();
    } else {
        pcapDescriptor = nullptr;
        descriptor = nullptr;
    }
    deviceInterface = "";
    rxSize = 0;
    txSize = 0;
    rxPacketNumber = 0;
    txPacketNumber = 0;
    errBuff[0] = '\0';
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
    pcapDescriptor = pcap_open_live(device, snaplen, promisc, timeout, errBuff);
    if (pcapDescriptor == nullptr) {
        std::cerr << "[Error] PCAP open failed; please verifying if the permission is root\n";
    }
    descriptor = (void*)pcapDescriptor;
}

/**
 * Looping for obtaining packets
 */
void LinuxPCAP::execute() {
    if (pcapDescriptor != nullptr) {
        pcap_loop(pcapDescriptor, 0, LinuxPCAP::packetHandler, reinterpret_cast<u_char*>(&rxPacketNumber));
    }
}

/**
 * Closing the PCAP
 */
void LinuxPCAP::close() {
    if (pcapDescriptor != nullptr) {
        pcap_close(pcapDescriptor);
        pcapDescriptor = nullptr;
    }
}

/**
 * Calculating the amount of the packets
 *
 * @param userData [u_char*]
 * @param pkthdr [const struct pcap_pkthdr*]
 * @param packet [const u_char*]
 * @param [void]
 */
void LinuxPCAP::packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    int* packetCount = (int*)userData;
    (*packetCount)++;
    std::cout << *packetCount << "  packets\t";

    long* totalSize = (long*)(userData + sizeof(int));
    *totalSize += pkthdr->len;
    std::cout << pkthdr->len << "  total size\n";
    sleep(2);
}

}  // namespace PCAP