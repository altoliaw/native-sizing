/**
 * @see SysinMainCaller.hpp
 */
#include "../Headers/SysinMainCaller.hpp"

namespace SysinMainCaller {
//===Global Declaration===
// Variables in .ini file
// Writing file path
char* _WRITING_FILE_LOCATION_ = nullptr;
// The time interval, "s" (the file will be recorded every "s" second(s))
unsigned int _WRITING_FILE_SECOND_ = 30;

// Determining if the "pcap_loop" shall be still working, 0x0: halting, 0x1: working
volatile char _IS_PCAP_WORKED_ = 0x1;
// Determining if the alarm shall be still working, 0x0: halting, 0x1: working
volatile char _IS_ALARM_WORKED_ = 0x1;
// Mutual locker
std::mutex _MUTEX_;

// Referring to the object for stopping "pcap_loop"
PCAP::LinuxPCAP* _PCAP_POINTER_ = nullptr;
// The address of the global pointer referring to the file descriptor object
FILE** _FILE_POINTER_ = nullptr;

static Commons::POSIXErrors config(std::vector<unitService>*);
static void packetHandler(u_char*, const struct pcap_pkthdr*, const u_char*);
static void packetTask(PCAP::LinuxPCAP*, void (*)(u_char*, const pcap_pkthdr*, const u_char*));
static void packetFileTask(FILE**, const char*);

/**
 * The starting process, the entry of the process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors" The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors start(int argC, char** argV) {
    Commons::POSIXErrors result = Commons::POSIXErrors::OK;

    // The data structure from the configure function
    // The array for reserving the interface name
    std::vector<unitService> interfaceNameArray;
    if (interfaceNameArray.empty() == false) {
        interfaceNameArray.clear();
        interfaceNameArray.shrink_to_fit();
    }

    // Calling the config function
    result = config(&interfaceNameArray);
    if (result != Commons::POSIXErrors::OK) {
        return result;
    }

    // The starting postion of the process
    // The output path
    char* OutputFilePathRule = (char*)"Outputs/trafficMonitor_%lu.tsv";

    // Obtaining the epoch
    char OuputFilePathWithTime[128] = {'\0'};
    sprintf(OuputFilePathWithTime, OutputFilePathRule, Commons::Time::getEpoch());
    _WRITING_FILE_LOCATION_ = OuputFilePathWithTime;

    // Installing a signal handler, interrupt
    signal(SIGINT, signalInterruptedHandler);

    {  // Creating objects, opening the interfaces, executing the packet calculations
       // and closing the interfaces; the number of objects is equal to the number of
       // the interfaces
        std::vector<PCAP::LinuxPCAP> interfaceForThread;
        for (unsigned int i = 0; i < interfaceNameArray.size(); i++) {
            PCAP::LinuxPCAP pcapObject;
            pcapObject.open(interfaceNameArray[i].interfaceName, BUFSIZ, 1, 1000, &(interfaceNameArray[i].port));

            // Putting each pcap object into thread array
            interfaceForThread.push_back(pcapObject);
        }

        FILE* fileDescriptor = nullptr;
        // n + 1  threads created; the n is equal to the number of interfaces
        // the values in the thread imply function name, argument 1, argument2 and so on;
        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < interfaceNameArray.size(); i++) {
            // threads.emplace_back(packetTask, i);
        }
        //     std::thread packetThread{packetTask, &pcapObject, packetHandler};
        std::thread writePacketFileThread{packetFileTask, &fileDescriptor, OuputFilePathWithTime};

        //     // When the functions finish or interrupt, those two threads shall
        //     // be joined into the main process
        //     packetThread.join();
        for (unsigned int i = 0; i < interfaceNameArray.size(); i++) {
            threads[i].join();
        }
        writePacketFileThread.join();

        // All pcap objects shall call the close function
        for (unsigned int i = 0; i < interfaceForThread.size(); i++) {
            (interfaceForThread[i]).close();
        }
        if (fileDescriptor != nullptr) {
            fclose(fileDescriptor);
        }
    }

    return result;
}

/**
 * Setting the config to this application
 *
 * @param services [std::vector<unitService>*] The data structure which is the key "service" defined in the .json file
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors" The status defined in the class "POSIXErrors"
 */
static Commons::POSIXErrors config(std::vector<unitService>* services) {
    Commons::POSIXErrors error = Commons::POSIXErrors::OK;

    // Loading information from the .json file for the application
    // The current working directory is the project root; as a result, the related path is shown as follows.
    const unsigned char* path = (const unsigned char*)"Settings/.Json/SysinMain.json";
    FileParsers::InitializedJsonFileParser::parseInitializedFile(path);

    // Obtaining the number of interfaces
    unsigned char serviceJsonString[2048] = {'\0'};
    // Obtaining the attribute, writingFileSecond, in the .json file
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.writingFileSecond", serviceJsonString);
    if (error != Commons::POSIXErrors::OK) {
        std::cerr << "base.writingFileSecond does not exist in the .json file.\n";
        return Commons::POSIXErrors::E_EXIST;
    }
    // Parsing the string into the unsigned int
    std::stringstream stream;
    stream << serviceJsonString;
    stream >> _WRITING_FILE_SECOND_;

    // Obtaining the attribute, service, in the .json file
    cJSON* cJsonItem = nullptr;
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.service", serviceJsonString, &cJsonItem);
    if (error != Commons::POSIXErrors::OK && cJsonItem->type != cJSON_Array) {
        std::cerr << "base.service does not exist in the .json file.\n";
        return Commons::POSIXErrors::E_EXIST;
    }
    // Obtaining the length of the service array
    unsigned int serviceLength = (unsigned int)cJSON_GetArraySize(cJsonItem);  // Obtaining the size of the array from "base.service" in the .json file
    // The array for reserving the interface name
    // Traversal of "service" defined in the .json file
    for (unsigned int i = 0; i < serviceLength; i++) {
        std::string interfaceName = "base.service.[" + std::to_string(i) + "].interface";
        error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)(interfaceName.c_str()), serviceJsonString);
        if (error != Commons::POSIXErrors::OK) {
            std::cerr << "base.service.[i].interface does not exist in the .json file.\n";
            break;
        }

        // Copying the interface value into the element of the array
        unitService unit;
        memcpy(unit.interfaceName, serviceJsonString, strlen((char*)serviceJsonString));
        unit.interfaceName[strlen((char*)serviceJsonString)] = '\0';

        // Reserving the ports into the unitService
        interfaceName = "base.service.[" + std::to_string(i) + "].port";
        error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)(interfaceName.c_str()), serviceJsonString, &cJsonItem);
        if (error != Commons::POSIXErrors::OK && cJsonItem->type != cJSON_Array) {
            std::cerr << "base.service.[i].port does not exist in the .json file.\n";
            return Commons::POSIXErrors::E_EXIST;
        }

        // Obtaining the port array size
        unsigned int portLength = (unsigned int)cJSON_GetArraySize(cJsonItem);
        unsigned int portNumber = 0;
        for (unsigned int j = 0; j < portLength; j++) {
            interfaceName = "base.service.[" + std::to_string(i) + "].port.[" + std::to_string(j) + "]";
            error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)(interfaceName.c_str()), serviceJsonString);
            if (error != Commons::POSIXErrors::OK) {
                std::cerr << "base.service.[i].port.[j] does not exist in the .json file.\n";
                break;
            }
            stream << serviceJsonString;
            stream >> portNumber;
            unit.port.push_back(portNumber);
        }
        // Pushing the unit into the "services"
        services->push_back(unit);
    }
    // For verifying the previous loop
    if (error != Commons::POSIXErrors::OK) {
        return Commons::POSIXErrors::E_EXIST;
    }

    return Commons::POSIXErrors::OK;
}

/**
 * The function for the first thread, packetThread; the task is to execute the "pcap_loop"
 *
 * @param pcap [PCAP::LinuxPCAP*] The address of the PCAP::LinuxPCAP object
 * @param packetHandler [void (*)(u_char*, const pcap_pkthdr*, const u_char*)] The callback function for pcap_loop
 */
static void packetTask(PCAP::LinuxPCAP* pcap, void (*packetHandler)(u_char*, const pcap_pkthdr*, const u_char*)) {
    _PCAP_POINTER_ = pcap;
    pcap->execute(packetHandler);
}

/**
 * The function for the second thread, writePacketFileThread; the task is to write the packet information into the the specified file
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

        } else {  // Adding the header information in a line to the file
            char output[1024] = {'\0'};
            int length = sprintf(output,
                                 "UTC\tType\tPort\tNumber(amount)\tSize(bytes)\tMaxSize(bytes)\t"
                                 "SQL number in the time interval\tSQL size(bytes) in the time interval\tSQL size per time interval(eps)\n");
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            if (*_FILE_POINTER_ != nullptr) {
                fclose(*_FILE_POINTER_);
                *_FILE_POINTER_ = nullptr;
            }
        }
    }

    // Using a global variable to verify if the interrupt occurs
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
    // Preparing the flag information of the tcp;
    // when the flag of the tcp is equal to 0x18, the packet belongs to SQL packets
    uint8_t tcpFlag = 0;

    // Determining the protocol (TCP or UDP)
    switch (ip_header->ip_p) {
        case IPPROTO_TCP:  // TCP
            tcpHeader = (tcphdr*)(packet + sizeof(ether_header) + sizeof(ip));
            packetSourcePort = ntohs(tcpHeader->th_sport);
            packetDestinationPort = ntohs(tcpHeader->th_dport);
            tcpFlag = tcpHeader->th_flags;
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
            tcpFlag = tcpHeader->th_flags;
    }

    // Critical section, accessing the data area
    _MUTEX_.lock();
    // Comparing source and destination ports with the port to determine the direction
    if (packetSourcePort == _PCAP_POINTER_->port) {  // TX packet
        previousPacketType = 0x0;
        _PCAP_POINTER_->txPacketNumber++;
        _PCAP_POINTER_->txSize += (long long)(pkthdr->len);

        // Obtaining the maximum size
        if (_PCAP_POINTER_->maxTxSize < (long long)(pkthdr->len)) {
            _PCAP_POINTER_->maxTxSize = (long long)(pkthdr->len);
        }

    } else if (packetDestinationPort == _PCAP_POINTER_->port) {  // RX packet
        previousPacketType = 0x1;
        _PCAP_POINTER_->rxPacketNumber++;
        _PCAP_POINTER_->rxSize += (long long)(pkthdr->len);

        // In this if section, the meaning implies that the packet from the client to server contain a SQL statement
        if (tcpFlag == 0x18) {
            _PCAP_POINTER_->sqlRequestNumber++;
            _PCAP_POINTER_->sqlRequestSize += (long long)(pkthdr->len);
        }

        // Obtaining the maximum size
        if (_PCAP_POINTER_->maxRxSize < (long long)(pkthdr->len)) {
            _PCAP_POINTER_->maxRxSize = (long long)(pkthdr->len);
        }

    } else {                              // Obtaining no type; as a result, the packet will belong to the previous one
        if (previousPacketType == 0x0) {  // TX packet
            _PCAP_POINTER_->txPacketNumber++;
            _PCAP_POINTER_->txSize += (long long)(pkthdr->len);

            // Obtaining the maximum size
            if (_PCAP_POINTER_->maxTxSize < (long long)(pkthdr->len)) {
                _PCAP_POINTER_->maxTxSize = (long long)(pkthdr->len);
            }

        } else {  // RX packet
            _PCAP_POINTER_->rxPacketNumber++;
            _PCAP_POINTER_->rxSize += (long long)(pkthdr->len);

            // In this if section, the meaning implies that the packet from the client to server contain a SQL statement
            if (tcpFlag == 0x18) {
                _PCAP_POINTER_->sqlRequestNumber++;
                _PCAP_POINTER_->sqlRequestSize += (long long)(pkthdr->len);
            }

            // Obtaining the maximum size
            if (_PCAP_POINTER_->maxRxSize < (long long)(pkthdr->len)) {
                _PCAP_POINTER_->maxRxSize = (long long)(pkthdr->len);
            }
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
    // Using these two global variables to break the loops in different threads
    _IS_PCAP_WORKED_ = 0x0;
    _IS_ALARM_WORKED_ = 0x0;
    alarm(0);
}

/**
 * A handler when receiving the SIGALRM signal; in the function, the main task is
 * writing the packet information to the file
 *
 * @param signalType [int] The signal type and the parameter is useless in this method
 */
void signalAlarmHandler(int) {
    // File writing
    char output[1024] = {"\0"};
    if (*_FILE_POINTER_ == nullptr) {
        // Opening the file
        *_FILE_POINTER_ = fopen(_WRITING_FILE_LOCATION_, "a+");

        if (*_FILE_POINTER_ == nullptr) {
            std::cerr << "Error opening the file!\n";
            signalInterruptedHandler(0);  // Going to the end of the thread

        } else {
            _MUTEX_.lock();
            // "UTC\tType\tPort\tNumber(amount)\tSize(byte)\tMaxSize\tSQL number per time interval(eps)\tSQL size per time interval(eps)\n";
            time_t timeEpoch = Commons::Time::getEpoch();
            // TX part; in the section, the last two result will be to zero because the packets
            // from the record set from the SQL server shall be ignored
            int length = sprintf(output,
                                 "%lu\tTX\t%d\t%lu\t%llu\t%lu\t%lu\t%llu\t%llu\n",
                                 timeEpoch,
                                 _PCAP_POINTER_->port,
                                 _PCAP_POINTER_->txPacketNumber,
                                 _PCAP_POINTER_->txSize,
                                 _PCAP_POINTER_->maxTxSize,
                                 (long)0,
                                 (long long)0,
                                 (long long)0);
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            _PCAP_POINTER_->txPacketNumber = 0;
            _PCAP_POINTER_->txSize = 0;
            _PCAP_POINTER_->maxTxSize = 0;

            // RX part
            length = sprintf(output,
                             "%lu\tRX\t%d\t%lu\t%llu\t%lu\t%lu\t%llu\t%llu\n",
                             timeEpoch,
                             _PCAP_POINTER_->port,
                             _PCAP_POINTER_->rxPacketNumber,
                             _PCAP_POINTER_->rxSize,
                             _PCAP_POINTER_->maxRxSize,
                             _PCAP_POINTER_->sqlRequestNumber,
                             _PCAP_POINTER_->sqlRequestSize,
                             _PCAP_POINTER_->sqlRequestSize / (long long)_WRITING_FILE_SECOND_);
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            _PCAP_POINTER_->rxPacketNumber = 0;
            _PCAP_POINTER_->rxSize = 0;
            _PCAP_POINTER_->maxRxSize = 0;
            _PCAP_POINTER_->sqlRequestNumber = 0;
            _PCAP_POINTER_->sqlRequestSize = 0;

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

/**
 * Constructor
 */
unitService::unitService() {
    interfaceName[0] = '\0';
    if (port.empty() == false) {
        port.clear();
        port.shrink_to_fit();
    }
}

/**
 * Destructor
 */
unitService::~unitService() {
    interfaceName[0] = '\0';
    if (port.empty() == false) {
        port.clear();
        port.shrink_to_fit();
    }
}

}  // namespace SysinMainCaller