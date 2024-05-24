/**
 * @see MainCaller.hpp
 */
#include "../Headers/MainCaller.hpp"

//===Global Declaration===
// Determining if the "pcap_loop" shall be still working, 0x0: halting, 0x1: working
volatile char _IS_PCAP_WORKED_ = 0x1;
// Determining if the alarm shall be still working, 0x0: halting, 0x1: working
volatile char _IS_ALARM_WORKED_ = 0x1;
// The time interval, s (the file will be recorded every s second(s))
unsigned int _WRITING_FILE_SECOND_ = 30;

// Referring to the object for stopping "pcap_loop"
PCAP::LinuxPCAP* _PCAP_POINTER_ = nullptr;
// The address of the global pointer referring to the file descriptor object
FILE** _FILE_POINTER_ = nullptr;

static void packetHandler(u_char*, const struct pcap_pkthdr*, const u_char*);
static void packetTask(PCAP::LinuxPCAP*, void (*)(u_char*, const pcap_pkthdr*, const u_char*));
static void packetFileTask(FILE**);

/**
 * The starting process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [int]
 */
int start(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;
    char* interfaceName = (argC <= 1) ? (char*)"eno2" : argV[0];

    // Installing a signal handler, interrupt
    signal(SIGINT, signalInterruptedHandler);    

    {  // Creating an object, opening the interface, executing the packet calculations
        // and closing the interface
        PCAP::LinuxPCAP pcapObject;
        pcapObject.open(interfaceName, BUFSIZ, 1, 1000);

        FILE* fileDescriptor = nullptr;        
        // Two threads; the values in the thread imply function name, argument 1, argument2 and so on
        std::thread packetThread{packetTask, &pcapObject, packetHandler};
        std::thread writePacketFileThread{packetFileTask, &fileDescriptor};

        // When the functions finish or interrupt, those two threads shall
        // be joined into the main procrss
        packetThread.join();
        writePacketFileThread.join();
        pcapObject.close();
        if(fileDescriptor != nullptr) {
            fclose(fileDescriptor);
        }
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
    _PCAP_POINTER_ = pcap;
    pcap->execute(packetHandler);
}

/**
 * The function for the thread, packetThread; the task is to executing the "pcap_loop"
 *
 * @param fileDescriptor [FILE**] The address of the pointer of the FILE descriptor
 */
static void packetFileTask(FILE** fileDescriptor) {
    // Installing a signal handler, alarm
    signal(SIGALRM, signalAlarmHandler);
    _FILE_POINTER_ = fileDescriptor; // Passing to the global variable
    // _PCAP_POINTER_

    // The first calling the function
    alarm(_WRITING_FILE_SECOND_);

    // Using a gloal variable to verify if the interrupt occurs
    while (_IS_ALARM_WORKED_ == 0x1) {
        sleep(5); // A routine clock checker
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
static void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    int* packetCount = (int*)userData;
    (*packetCount)++;
    std::cout << *packetCount << " packets\t";

    long* totalSize = (long*)(userData + sizeof(int));
    *totalSize += pkthdr->len;
    std::cout << pkthdr->len << "  total size\n";

    // Verifying if the "pcap_loop" shall be stopped; "_IS_PCAP_WORKED_" is
    // a global variable and is controlled by the signal mechanism
    if (_IS_PCAP_WORKED_ == 0x0) {
        pcap_breakloop((pcap_t*)_PCAP_POINTER_->descriptor);
    }
}

/**
 * A handler when receiving the SIGINT signal
 *
 * @param [int] The signal type (ignore)
 */
void signalInterruptedHandler(int) {
    std::cerr << "\n" <<"Interrupted signal occurs, please wait.\n";
    _IS_PCAP_WORKED_ = 0x0;
    _IS_ALARM_WORKED_ = 0x0;
    alarm(0);
}

/**
 * A handler when receiving the SIGALRM signal; in the function, the main task is 
 * writing the packet information to the file
 */
void signalAlarmHandler(int) {
    std::cerr << "signalAlarmHandler called\n";
    // TODO File writing 
    alarm(_WRITING_FILE_SECOND_);
}