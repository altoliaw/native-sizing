/**
 * @see MainCaller.hpp
 */
#include "../Headers/MainCaller.hpp"

//===Global Declaration===
// Determining if the "pcap_loop" shall be still working, 0x0: halting, 0x1: working
volatile char isPCAPWorked = 0x1;
// Referring the object for stopping pcap_loop
PCAP::LinuxPCAP* pcapPointer = nullptr;

static void packetHandler(u_char*, const struct pcap_pkthdr*, const u_char*);
static void packetTask(PCAP::LinuxPCAP*, void (*)(u_char*, const pcap_pkthdr*, const u_char*));
static void packetFileTask(FILE*);

/**
 * The starting process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [int]
 */
int start(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;
    char* interfaceName = (argC <= 1) ? (char*)"eno2" : argV[0] ;

    // Installing a signal handler
    signal(SIGINT, signalHandler);


    {   // Creating an object, opening the interface, executing the packet calculations
        // and closing the interface
        FILE* fileDescriptor = nullptr;
        PCAP::LinuxPCAP pcapObject;
        pcapPointer = &pcapObject;
        pcapObject.open(interfaceName, BUFSIZ, 1, 1000);
        std::thread packetThread {packetTask, pcapPointer, packetHandler};
        std::thread writePacketFileThread {packetFileTask, fileDescriptor};

        // When the functions finish or interrupt, those two threads shall 
        // be joined into the main procrss
        packetThread.join();
        writePacketFileThread.join();
        pcapObject.close();
    }

    return result;
}

/**
 * The function for the thread, packetThread; the task is to executing the "pcap_loop"
 * 
 * @param pcap [PCAP::LinuxPCAP*] The address of the PCAP::LinuxPCAP object
 * @param packetHandler [void (*)(u_char*, const pcap_pkthdr*, const u_char*)] The callback function for pcap_loop
 */
static void packetTask(PCAP::LinuxPCAP* pcap, void (*packetHandler)(u_char*, const pcap_pkthdr*, const u_char*)) {
    pcap->execute(packetHandler);
}

/**
 * The function for the thread, packetThread; the task is to executing the "pcap_loop"
 * 
 */
static void packetFileTask(FILE* descriptor) {
    std::cerr<< "B function";
    sleep(2);
    std::cerr<< "B function 2";
}
/**
 * Calculating the amount of the packets
 *
 * @param userData [u_char*]
 * @param pkthdr [const struct pcap_pkthdr*]
 * @param packet [const u_char*]
 * @param [void]
 */
static void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    int* packetCount = (int*)userData;
    (*packetCount)++;
    std::cout << *packetCount << " packets\t";

    long* totalSize = (long*)(userData + sizeof(int));
    *totalSize += pkthdr->len;
    std::cout << pkthdr->len << "  total size\n";
    

    // Verifying if the "pcap_loop" shall be stopped; "isPCAPWorked" is
    // a global variable and is controlled by the signal mechanism
    if (isPCAPWorked == 0x0) {
        pcap_breakloop((pcap_t*)pcapPointer->descriptor); 
    }
}

/**
 * A handler when receiving the SIGINT signal
 * 
 * @param [int] The signal type (ignore)
 */
void signalHandler(int){
    isPCAPWorked = 0x0;
}