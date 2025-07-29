/**
 * @see WindowsPCAP.hpp
 */
#include "../Headers/WindowsPCAP.hpp"

#ifdef _WIN32
namespace PCAP {
/**
 * Constructor
 */
WindowsPCAP::WindowsPCAP() {
    errBuff[0] = '\0';
    descriptor = nullptr;
    pcapDescriptor = (HANDLE)descriptor; // HANDLE is the macro, thr original definition is equal to the pointer type.
    deviceInterface = "";
    rxSize = 0;
    txSize = 0;
    rxPacketNumber = 0;
    txPacketNumber = 0;
    rxGroupCount = 0;
    txGroupCount = 0;
    flowChangeCount = 0;
    maxRxSize = 0;
    maxTxSize = 0;
}

/**
 * Destructor
 */
WindowsPCAP::~WindowsPCAP() {
    // PCAP handle shall be closed and NULL.
    if (pcapDescriptor != nullptr) {
        std::cerr << "[Error] pcapDescriptor shall be deallocated.\n";
        this->close();
    } else {
        pcapDescriptor = nullptr;
        descriptor = nullptr;
    }
    errBuff[0] = '\0';
    deviceInterface = "";
    rxSize = 0;
    txSize = 0;
    rxPacketNumber = 0;
    txPacketNumber = 0;
    rxGroupCount = 0;
    txGroupCount = 0;
    flowChangeCount = 0;
    maxRxSize = 0;
    maxTxSize = 0;

    // To ensure that the map is empty
    if (portRelatedInformation.empty() == false) {
        for (std::unordered_map<int, PCAPPortInformation*>::iterator it = portRelatedInformation.begin();
             it != portRelatedInformation.end();
             it++) {
            if (it->second != nullptr) {
                delete (it->second);
            }
            (it->second) = nullptr;
        }
        portRelatedInformation.clear();
    }
}

/**
 * Opening the PCAP object according to an interface with different ports
 *
 * @param device [const char*] The interface
 * @param snaplen [const int] The upper bound of the bits for each packet
 * @param promisc [const int] Specifying that the device is to be put into promiscuous mode.
 * A value of 1 (True) turns promiscuous mode on.
 * @param timeout [const int] Timeout (milliseconds)
 * @param port [std::vector<int>*] The port of the server for distinguishing with the packets from rx and tx
 */
void WindowsPCAP::open(const char* device, const int snaplen, const int promisc, const int timeout, std::vector<int>* port) {
    /* Setting the variable of receiving packet filter; when the value is equal to "" or "true",
       that implies that the winDivert will receive all packets; when the value equals to "tcp or udp",
       that indicates that the winDivert will receive tcp or udp packets; for more information, 
       please refer to the URL as below: https://github.com/basil00/WinDivert/wiki/WinDivert-Documentation#filter_language.
     */
    std::string filter = "true";

    // Opening the WinDivert handle
    pcapDescriptor = WinDivertOpen((const char*)filter.c_str(), WINDIVERT_LAYER_NETWORK, 0, 0);
    if (pcapDescriptor == INVALID_HANDLE_VALUE) {
        std::cerr << "[Error] PCAP open failed; please verifying if the permission is root\n";
    }
    descriptor = (void*)pcapDescriptor;  // Passing the descriptor to the general type

    // Copying the NIC information into the object
    std::string deviceInterface(device);
    this->deviceInterface = deviceInterface;
    // Copying the ports information into each portRelatedInformation (set)
    for (unsigned int i = 0; i < port->size(); i++) {
        PCAPPortInformation* PCAPPortInstance = new PCAPPortInformation();
        PCAPPortInstance->port = (*port)[i];
        portRelatedInformation.emplace((*port)[i], PCAPPortInstance);
    }
}

/**
 * Looping for obtaining packets; if the developer does not pass the argument, the default static function, LinuxPCAP::packetHandler,
 * defined in class will be injected; otherwise, the user-defined function will be referred
 *
 * @param callback [void (*)(u_char*, const pcap_pkthdr*, const u_char*)] The callback function for pcap_loop;
 * the default value of the function is "nullptr" (has been initialized in the declaration)
 */
void WindowsPCAP::execute(void (*callback)(u_char*, const pcap_pkthdr*, const u_char*)) {
    // When pcapDescriptor belongs to INVALID_HANDLE_VALUE, ...
    if (pcapDescriptor != INVALID_HANDLE_VALUE) {

    }
}

/**
 * Closing the PCAP
 */
void WindowsPCAP::close() {
    if (pcapDescriptor != INVALID_HANDLE_VALUE) {
        WinDivertClose(pcapDescriptor);
        pcapDescriptor = INVALID_HANDLE_VALUE;  // Setting the handle to invalid value
    }
}

/**
 * Calculating the amount of the packets, a callback function to throw into the PCAP module (default)
 * When the outer does not throw the user defined callback function, the function below will execute automatically.
 *
 * @param userData [u_char*] The additional information for the function, packetHandler; the additional information will be binding with
 * the forth argument in the pcap_loop
 * @param pkthdr [const struct pcap_pkthdr*] The header of the packet (metadata)
 * @param packet [const u_char*] The data from the last position of the header of the packet
 */
void WindowsPCAP::packetHandler(u_char* userData, const pcap_pkthdr* pkthdr, const u_char* packet) {
    int* packetCount = (int*)userData;
    (*packetCount)++;
    std::cout << *packetCount << "  packets\t";

    long* totalSize = (long*)(userData + sizeof(int));
    // *totalSize += pkthdr->len;
    // std::cout << pkthdr->len << "  total size\n";
    // Unit: milliseconds; 2000 milliseconds = 2 seconds
    Sleep(2000);
}

/**
 * Displaying the devices for .Json settings; this function is for windows platform because
 * the interface name on windows platform is involved with the registered machine codes; as
 * a result, the on windows platform, the editor shall provide device information for users'
 * json definition
 */
void WindowsPCAP::show() {
    WSADATA wsaData; // An object for reserving the "window sockets application" (wsa) information for winsock 
                     // and the winsock introduction is list as below:
                     // https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2
    WSAStartup(MAKEWORD(2, 2), &wsaData); // Initializing the Winsock with the major and the minor versions are 2 and 2, respectively

    IP_ADAPTER_ADDRESSES* adapterAddresses = nullptr; // A window network interface pointer
    ULONG outBuffLength = (ULONG)OUT_BUFF_lENGTH; // The default size of the buffer
    DWORD dwReturnVal = 0; // double word size's return value

    // Using for loop to extend the memory buffer automatically; the for loop implies a retry mechanism; only 5 tries are in the case
    for (int attempts = 0; attempts < 5; attempts++) {
        adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBuffLength); // Allocating the memory size dynamically
        if (!adapterAddresses) {
            std::cerr << "Memory allocation failed\n";
            return;
        }

        /* Obtaining all network interfaces' information from the NIC, for more details, please refer to th following URL:
         * https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses?utm_source=chatgpt.com */
        dwReturnVal = GetAdaptersAddresses( AF_UNSPEC, // Obtaining the IPV4 and IPV6 information
                                            GAA_FLAG_INCLUDE_PREFIX, // The address length
                                            NULL,
                                            adapterAddresses, // The related contents
                                            &outBuffLength // The size of the related contents
                      );

        if (dwReturnVal == ERROR_BUFFER_OVERFLOW) { // When the error code belongs to "buffer too small", 
            free(adapterAddresses);
            adapterAddresses = nullptr; 
            continue; // Trying again
        }
        // When accepted, ...
        break;
    }

    if (dwReturnVal != NO_ERROR) { // When error occurs, ...
        std::cerr << "GetAdaptersAddresses failed with error: " << dwReturnVal << "\n";
        if (adapterAddresses) {
            free(adapterAddresses);
            adapterAddresses = nullptr;
        }
        WSACleanup(); // Closing and cleaning the winsock
        return;
    }

    // Displaying the device information (i.e., the codes are simulated as the function, pcap_findalldevs, in NPCAP)
    int index = 0;
    for (IP_ADAPTER_ADDRESSES* adapter = adapterAddresses;
         adapter != nullptr; adapter = adapter->Next) {
        std::string adapterName = "\\\\Device\\\\NPF_";
        adapterName = (adapter->AdapterName) ? (adapterName + adapter->AdapterName ) : "Unknown";
        std::cout << (++index) << ".\t" << adapterName << "\t\t";

        // if (adapter->FriendlyName) { // The type belongs to PWCHAR, which is for wide strings in UTF-16 
        //                              // (i.e., std::cout will obtain encoded strings; std::wcout will obtain correct strings)
        //     std::wcerr << L"   Friendly Name: " << adapter->FriendlyName << L"\n"; // "L" implies the wide string
        // }

        // Traversal of network adapters and their own unicast IP; (i.e., unicast implies a transformation from a node to a node)
        for ( IP_ADAPTER_UNICAST_ADDRESS* unicast = adapter->FirstUnicastAddress;
              unicast != nullptr; 
              unicast = unicast->Next ) {
            
            char ipStringBuffer[INET6_ADDRSTRLEN] = {'\0'};
            int family = unicast->Address.lpSockaddr->sa_family; // Obtaining the sa_family field indicates which protocol family this address belongs to

            if (family == AF_INET) {
                getnameinfo(
                    unicast->Address.lpSockaddr, // The pointer to the IP address (as sockaddr)
                    (family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6), // The address structure length based on protocol family
                    ipStringBuffer, // The output buffer for the resulting IP string
                    sizeof(ipStringBuffer), // The size of the output buffer
                    nullptr,
                    0,
                    NI_NUMERICHOST // Forcing numeric IP output (no DNS lookup)
                );
                std::cerr << "   IP Address: " << ipStringBuffer << std::endl;
            }
        }
    }

    free(adapterAddresses);
    WSACleanup();
    return;
}

/**
 * The simulation of the pcap_loop; in winDirvert, there is no pcap_loop function; as a result, the function shall be defined manually
 * 
 * @param pcapDescriptorPointer [HANDLE*] The pointer to the pcapDescriptorPointer
 * @param count [int] The number of packets to capture before the function returns; when the value is equal to 0 or -1, this implies that
 * the process will capture packets indefinitely until an error occurs or the loop is manually stopped; when the value is larger than 0, 
 * this implies that the process will capture exactly count packets
 * @param callback [void (*)(u_char*, const pcap_pkthdr*, const u_char*)] The callback function  
 * @param userDefinedData [uchar*] The pointer to the user defined data object
 */
static void pcap_loop(HANDLE* pcapDescriptorPointer, int count, void (*callback)(u_char*, const pcap_pkthdr*, const u_char*), u_char* userDefinedData) {
    if (count == 0 || count == -1) { // Case infinity
        while (true) {

        }
    } else if (count > 0) { // Case finiteness
        while (count) {
            count--;
        }
    }
}

}  // namespace PCAP
#endif