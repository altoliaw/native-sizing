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
// Mutual locker
std::mutex _MUTEX_;
// Writing file path
char* _WRITING_FILE_LOCATION_ = nullptr;

// Referring to the object for stopping "pcap_loop"
PCAP::LinuxPCAP* _PCAP_POINTER_ = nullptr;
// The address of the global pointer referring to the file descriptor object
FILE** _FILE_POINTER_ = nullptr;

static void packetHandler(u_char*, const struct pcap_pkthdr*, const u_char*);
static void packetTask(PCAP::LinuxPCAP*, void (*)(u_char*, const pcap_pkthdr*, const u_char*));
static void packetFileTask(FILE**, const char*);

/**
 * The starting process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [int] The result defined in "POSIXErrors.hpp"
 */
int start(int argC, char** argV) {
    // Preparing some information
    char* interfaceName = (argC <= 1) ? (char*)"eno2" : argV[0];
    int port = (argC <= 2) ? 1521 : atoi(argV[1]);                                               // The server port
    char* OutputFilePathRule = (argC <= 3) ? (char*)"Outputs/trafficMonitor_%lu.tsv" : argV[2];  // The ouytput path
    
    // Obtaining the epoch
    Commons::Time::getTimeInitialization();
    char OuputFilePathWithTime[100] = {'\0'};
    sprintf(OuputFilePathWithTime, OutputFilePathRule, Commons::Time::getEpoch());
    _WRITING_FILE_LOCATION_ = OuputFilePathWithTime;

    int result = Commons::POSIXErrors::OK;
    // Installing a signal handler, interrupt
    signal(SIGINT, signalInterruptedHandler);

    {  // Creating an object, opening the interface, executing the packet calculations
        // and closing the interface
        PCAP::LinuxPCAP pcapObject;
        pcapObject.open(interfaceName, BUFSIZ, 1, 1000, (const int)port);

        FILE* fileDescriptor = nullptr;
        // Two threads; the values in the thread imply function name, argument 1, argument2 and so on
        std::thread packetThread{packetTask, &pcapObject, packetHandler};
        std::thread writePacketFileThread{packetFileTask, &fileDescriptor, OuputFilePathWithTime};

        // When the functions finish or interrupt, those two threads shall
        // be joined into the main procrss
        packetThread.join();
        writePacketFileThread.join();
        pcapObject.close();
        if (fileDescriptor != nullptr) {
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
 * @param filePath [const char*] The file path for recording the information
 */
static void packetFileTask(FILE** fileDescriptor, const char* filePath) {
    // Installing a signal handler, alarm
    signal(SIGALRM, signalAlarmHandler);
    _FILE_POINTER_ = fileDescriptor;  // Passing to the global variable

    // The first calling the function
    alarm(_WRITING_FILE_SECOND_);

    // Opening the file with the file descriptor
    if (*_FILE_POINTER_ == nullptr) {
        *_FILE_POINTER_ = fopen(filePath, "a+");
        if (*_FILE_POINTER_ == nullptr) {
            std::cerr << "Error opening the file!\n";
            signalInterruptedHandler(0);  // Going to the end of the thread
        } else {                          // Adding the header in the file
            char output[1024] = {'\0'};
            int length = sprintf(output, "UTC\tType\tNumber(amount)\tSize(byte)\teps(SQL number per time interval)\n");
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            if (*_FILE_POINTER_ != nullptr) {
                fclose(*_FILE_POINTER_);
                *_FILE_POINTER_ = nullptr;
            }
        }
    }

    // Using a gloal variable to verify if the interrupt occurs
    while (_IS_ALARM_WORKED_ == 0x1) {
        sleep(5);  // A routine clock checker
    }

    // Closing the file
    if (*_FILE_POINTER_ != nullptr) {
        fclose(*_FILE_POINTER_);
    }
}

/**
 * Calculating the amount of the packets
 *
 * @param userData [u_char*]
 * @param pkthdr [const struct pcap_pkthdr*] The address of the packet header
 * @param packet [const u_char*] The address of the packet
 */
static void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    static char previousPacketType = 0x0;  // 0x0: TX, 0x1: RX

    // Obtaining the IP header; the ip_p column implies the protocol;
    // the number of the TCP is 6, and the UDP is 17
    ip* ip_header = (ip*)(packet + sizeof(ether_header));

    // Preparing the headers and the packet source/destination port variables
    tcphdr* tcpHeader = nullptr;
    udphdr* udpHeader = nullptr;
    uint16_t packetSourcePort = 0;
    uint16_t packetDestinationPort = 0;

    // Determining the protocol (TCP or UDP)
    switch (ip_header->ip_p) {
        case IPPROTO_TCP:  // TCP
            tcpHeader = (tcphdr*)(packet + sizeof(ether_header) + sizeof(ip));
            packetSourcePort = ntohs(tcpHeader->th_sport);
            packetDestinationPort = ntohs(tcpHeader->th_dport);
            break;
        case IPPROTO_UDP:  // UDP
            udpHeader = (udphdr*)(packet + sizeof(ether_header) + sizeof(ip));
            packetSourcePort = ntohs(udpHeader->uh_sport);
            packetDestinationPort = ntohs(udpHeader->uh_dport);
            break;
        default:
            tcpHeader = (tcphdr*)(packet + sizeof(ether_header) + sizeof(ip));
            packetSourcePort = ntohs(tcpHeader->th_sport);
            packetDestinationPort = ntohs(tcpHeader->th_dport);
    }

    // Critical section, accessing the data area
    _MUTEX_.lock();
    // Comparing source and destination ports with the port to determine the direction
    if (packetSourcePort == _PCAP_POINTER_->port) {  // TX packet
        previousPacketType = 0x0;
        _PCAP_POINTER_->txPacketNumber++;
        _PCAP_POINTER_->txSize += (long long)(pkthdr->len);
    } else if (packetDestinationPort == _PCAP_POINTER_->port) {  // RX packet
        previousPacketType = 0x1;
        _PCAP_POINTER_->rxPacketNumber++;
        _PCAP_POINTER_->rxSize += (long long)(pkthdr->len);
    } else {                              // Obtaining no type; as a result, the packet will belong to the previous one
        if (previousPacketType == 0x0) {  // TX packet
            _PCAP_POINTER_->txPacketNumber++;
            _PCAP_POINTER_->txSize += (long long)(pkthdr->len);
        } else {  // RX packet
            _PCAP_POINTER_->rxPacketNumber++;
            _PCAP_POINTER_->rxSize += (long long)(pkthdr->len);
        }
    }
    // Critical section end
    _MUTEX_.unlock();

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
    std::cerr << "\n"
              << "Interrupted signal occurs, please wait.\n";
    _IS_PCAP_WORKED_ = 0x0;
    _IS_ALARM_WORKED_ = 0x0;
    alarm(0);
}

/**
 * A handler when receiving the SIGALRM signal; in the function, the main task is
 * writing the packet information to the file
 */
void signalAlarmHandler(int) {
    // File writing
    char output[1024] = {"\0"};
    if (*_FILE_POINTER_ == nullptr) {
        std::cerr << _WRITING_FILE_LOCATION_ << "\n";
        *_FILE_POINTER_ = fopen(_WRITING_FILE_LOCATION_, "a+");
        if (*_FILE_POINTER_ == nullptr) {
            std::cerr << "Error opening the file!\n";
            signalInterruptedHandler(0);  // Going to the end of the thread
        } else {
            _MUTEX_.lock();
            time_t timeEpoch = Commons::Time::getEpoch();
            // TX part
            int length = sprintf(output,
                                 "%lu\tTX\t%lu\t%llu\t%llu\n",
                                 timeEpoch,
                                 _PCAP_POINTER_->txPacketNumber,
                                 _PCAP_POINTER_->txSize / 8,
                                 _PCAP_POINTER_->txSize / (long long)_WRITING_FILE_SECOND_ / 8);
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            _PCAP_POINTER_->txPacketNumber = 0;
            _PCAP_POINTER_->txSize = 0;
            // RX part
            length = sprintf(output,
                             "%lu\tRX\t%lu\t%llu\t%llu\n",
                             timeEpoch,
                             _PCAP_POINTER_->rxPacketNumber,
                             _PCAP_POINTER_->rxSize / 8,
                             _PCAP_POINTER_->rxSize / (long long)_WRITING_FILE_SECOND_ / 8);
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            _PCAP_POINTER_->rxPacketNumber = 0;
            _PCAP_POINTER_->rxSize = 0;

            _MUTEX_.unlock();
            alarm(_WRITING_FILE_SECOND_);
            // Closing the file
            if (*_FILE_POINTER_ != nullptr) {
                fclose(*_FILE_POINTER_);
                *_FILE_POINTER_ = nullptr;
            }
        }
    } else {  // Closing the descriptor and skipping the handling in the ith loop
        fclose(*_FILE_POINTER_);
        *_FILE_POINTER_ = nullptr;
    }
}