/**
 * @see WindowsNpcapSizingMainController.hpp
 */
#include "../../Headers/SizingControllers/WindowsNpcapSizingMainController.hpp"
#ifdef _WIN32

namespace SizingControllers {
//===Global Declaration===
// Variables in .ini file
// Writing file path
char* _WRITING_FILE_LOCATION_ = nullptr;
// The time interval, "s" (the file will be recorded every "s" second(s))
unsigned int _WRITING_FILE_SECOND_ = 30;
// The output layout format type (default value is equal to 0.)
unsigned int _OUTPUT_LAYOUT_TYPE_ = 0;

// Determining if the "pcap_loop" shall be still working, 0x0: halting, 0x1: working
volatile char _IS_PCAP_WORKED_ = 0x1;
// Determining if the alarm shall be still working, 0x0: halting, 0x1: working
volatile char _IS_ALARM_WORKED_ = 0x1;
// Mutual locker(Windows version), using by the critical section (only works on a process on Windows)
CRITICAL_SECTION _CRITICAL_SECTION_;
// For the windows timer
HANDLE _TIMER_ = nullptr;
// For the windows exit signal
HANDLE _EXITED_EVENT_ = nullptr;

// Referring to the objects for stopping "pcap_loop"
std::vector<PCAP::PCAPPrototype*> _PCAP_POINTER_;
// The address of the global pointer referring to the file descriptor object
FILE** _FILE_POINTER_ = nullptr;

//===Static fields Declaration===
// For reserving the session's previous, the key is a tuple which combines sorted ip and port information;
// the second one is the session's previous packet type; the value is defined as follows: 0: undefined; 1: TX, and 2: RX
std::map<std::tuple <uint32_t, uint32_t, uint16_t, uint16_t>, char> WindowsNpcapSizingMainController::sessionMap;
// For recording the maximum number of packets per second
long WindowsNpcapSizingMainController::currentSqlMaxRequestNumberPerSec = 0;
// For reserving the starting time in the beginning or the updating time when the SQL statements receive
std::chrono::steady_clock::time_point WindowsNpcapSizingMainController::startingTime = std::chrono::steady_clock::time_point::min();

/**
 * The starting process, the entry of the process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors WindowsNpcapSizingMainController::start(int argC, char** argV) {
    Commons::POSIXErrors result = Commons::POSIXErrors::OK;

    // If the argument is "-l", show network interfaces and exit.
    if (argC == 2 && strcmp(argV[1], "-l") == 0) {
        PCAP::WindowsNpcapPCAP::show();
        return result;
    }

    // Data structure to hold interface and port configuration.
    std::vector<unitService> interfaceNameArray;
    if (interfaceNameArray.empty() == false) {
        interfaceNameArray.clear();
        interfaceNameArray.shrink_to_fit();
    }

    // Load configuration from JSON file.
    result = config(&interfaceNameArray);
    if (result != Commons::POSIXErrors::OK) {
        return result;
    }

    // Prepare the output file path with a timestamp.
    char* OutputFilePathRule = (char*)"Outputs/trafficMonitor_%lu.tsv";
    char OuputFilePathWithTime[128] = {'\0'};
    sprintf(OuputFilePathWithTime, OutputFilePathRule, Commons::Time::getEpoch());
    _WRITING_FILE_LOCATION_ = OuputFilePathWithTime;

    // Set up a handler for console control signals (e.g., CTRL+C).
    SetConsoleCtrlHandler(WindowsNpcapSizingMainController::signalInterruptedHandler, TRUE);

    {
        // Create and configure Npcap objects for each specified interface.
        std::vector<PCAP::WindowsNpcapPCAP*> pcapObjectOfInterface;
        for (unsigned int i = 0; i < interfaceNameArray.size(); i++) {
            PCAP::WindowsNpcapPCAP* pcapObject = new PCAP::WindowsNpcapPCAP();
            pcapObject->open(interfaceNameArray[i].interfaceName, BUFSIZ, 1, 1000, &(interfaceNameArray[i].port));
            pcapObjectOfInterface.push_back(pcapObject);
        }

        // Free memory used by the configuration array as it's now stored in pcap objects.
        if (interfaceNameArray.empty() == false) {
            interfaceNameArray.clear();
            interfaceNameArray.shrink_to_fit();
        }

        // Initialize the critical section for thread synchronization.
        InitializeCriticalSection(&_CRITICAL_SECTION_);

        FILE* fileDescriptor = nullptr;
        // Create n+1 threads: n for packet capturing (one per interface) and 1 for writing to file.
        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < pcapObjectOfInterface.size(); i++) {
            threads.emplace_back(packetTask, pcapObjectOfInterface[i], packetHandler);
            // Store pcap object pointers globally to allow signal handlers to access them.
            _PCAP_POINTER_.push_back(pcapObjectOfInterface[i]);
        }

        // The thread for writing packet data to a file periodically.
        std::thread writePacketFileThread{packetFileTask, &fileDescriptor, OuputFilePathWithTime};

        // Wait for all packet capturing threads to complete.
        for (unsigned int i = 0; i < pcapObjectOfInterface.size(); i++) {
            threads[i].join();
        }
        // Wait for the file writing thread to complete.
        writePacketFileThread.join();

        // Clean up the critical section.
        DeleteCriticalSection(&_CRITICAL_SECTION_);

        // Clean up pcap objects and close handles.
        for (unsigned int i = 0; i < pcapObjectOfInterface.size(); i++) {
            if (pcapObjectOfInterface[i] != nullptr) {
                (pcapObjectOfInterface[i])->close();
                delete (pcapObjectOfInterface[i]);
            }
            pcapObjectOfInterface[i] = nullptr;
        }
        // Close the file descriptor if it's still open.
        if (fileDescriptor != nullptr) {
            fclose(fileDescriptor);
            fileDescriptor = nullptr;
        }
    }

    return result;
}

/**
 * Reads and parses configuration from the SizingMain.json file.
 *
 * @param services [std::vector<unitService>*] A pointer to a vector to be filled with service configurations (interfaces and ports).
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors".
 */
Commons::POSIXErrors WindowsNpcapSizingMainController::config(std::vector<unitService>* services) {
    Commons::POSIXErrors error = Commons::POSIXErrors::OK;
    const unsigned char* path = (const unsigned char*)"Settings/.Json/SizingMain.json";
    FileParsers::InitializedJsonFileParser::parseInitializedFile(path);

    unsigned char serviceJsonString[2048] = {'\0'};
    // Get 'base.writingFileSecond' from JSON.
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.writingFileSecond", serviceJsonString);
    if (error != Commons::POSIXErrors::OK) {
        std::cerr << "base.writingFileSecond does not exist in the .json file.\n";
        return Commons::POSIXErrors::E_EXIST;
    }
    std::stringstream stream;
    stream.clear();
    stream.str("");
    stream << serviceJsonString;
    stream >> _WRITING_FILE_SECOND_;

    // Get 'base.outputLayoutType' from JSON.
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.outputLayoutType", serviceJsonString);
    if (error != Commons::POSIXErrors::OK) {
        std::cerr << "base.outputLayoutType does not exist in the .json file.\n";
        return Commons::POSIXErrors::E_EXIST;
    }
    stream.clear();
    stream.str("");
    stream << serviceJsonString;
    stream >> _OUTPUT_LAYOUT_TYPE_;

    // Get the 'base.service' array from JSON.
    cJSON* cJsonItem = nullptr;
    error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)"base.service", serviceJsonString, &cJsonItem);
    if (error != Commons::POSIXErrors::OK && cJsonItem->type != cJSON_Array) {
        std::cerr << "base.service does not exist in the .json file.\n";
        return Commons::POSIXErrors::E_EXIST;
    }
    // Iterate through each service object in the array.
    unsigned int serviceLength = (unsigned int)cJSON_GetArraySize(cJsonItem);
    for (unsigned int i = 0; i < serviceLength; i++) {
        // Get the interface name for the current service.
        std::string interfaceName = "base.service.[" + std::to_string(i) + "].interface";
        error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)(interfaceName.c_str()), serviceJsonString);
        if (error != Commons::POSIXErrors::OK) {
            std::cerr << "base.service.[i].interface does not exist in the .json file.\n";
            break;
        }

        unitService unit;
        memcpy(unit.interfaceName, serviceJsonString, strlen((char*)serviceJsonString));
        unit.interfaceName[strlen((char*)serviceJsonString)] = '\0';

        // Get the port array for the current service.
        interfaceName = "base.service.[" + std::to_string(i) + "].port";
        error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)(interfaceName.c_str()), serviceJsonString, &cJsonItem);
        if (error != Commons::POSIXErrors::OK && cJsonItem->type != cJSON_Array) {
            std::cerr << "base.service.[i].port does not exist in the .json file.\n";
            return Commons::POSIXErrors::E_EXIST;
        }

        // Iterate through the ports and add them to the unit.
        unsigned int portLength = (unsigned int)cJSON_GetArraySize(cJsonItem);
        unsigned int portNumber = 0;
        for (unsigned int j = 0; j < portLength; j++) {
            interfaceName = "base.service.[" + std::to_string(i) + "].port.[" + std::to_string(j) + "]";
            error = FileParsers::InitializedJsonFileParser::getValueFromFileParser((const unsigned char*)(interfaceName.c_str()), serviceJsonString);
            if (error != Commons::POSIXErrors::OK) {
                std::cerr << "base.service.[i].port.[j] does not exist in the .json file.\n";
                break;
            }
            stream.clear();
            stream.str("");
            stream << serviceJsonString;
            stream >> portNumber;
            unit.port.push_back(portNumber);
        }
        services->push_back(unit);
    }
    if (error != Commons::POSIXErrors::OK) {
        return Commons::POSIXErrors::E_EXIST;
    }

    return Commons::POSIXErrors::OK;
}

/**
 * Task function for packet capturing threads. Calls the pcap execution loop.
 *
 * @param pcap [PCAP::WindowsNpcapPCAP*] Pointer to the Npcap object for a specific interface.
 * @param packetHandler [void (*)(u_char*, const pcap_pkthdr*, const u_char*)] Callback function to process each packet.
 */
void WindowsNpcapSizingMainController::packetTask(PCAP::WindowsNpcapPCAP* pcap, void (*packetHandler)(u_char*, const pcap_pkthdr*, const u_char*)) {
    pcap->execute(packetHandler);
}

/**
 * Task function for the file writing thread. Periodically writes captured data statistics to a file.
 *
 * @param fileDescriptor [FILE**] Address of the pointer to the file descriptor.
 * @param filePath [const char*] The path of the file to write to.
 */
void WindowsNpcapSizingMainController::packetFileTask(FILE** fileDescriptor, const char* filePath) {
    // Create an event for signaling thread exit.
    _EXITED_EVENT_ = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (_EXITED_EVENT_ == nullptr) {
        _IS_ALARM_WORKED_ = 0x0;
        std::cerr << "[Error] CreateEvent failed.\n";
        return;
    }

    // Create a waitable timer for periodic file writing.
    _TIMER_ = CreateWaitableTimer(nullptr, FALSE, nullptr);
    if (_TIMER_ == nullptr) {
        std::cerr << "[Error] Failed to create a waitable timer.\n";
        _IS_ALARM_WORKED_ = 0x0;
        return;
    }

    _FILE_POINTER_ = fileDescriptor;
    // Open the output file and write the header row if it's a new file.
    if (*_FILE_POINTER_ == nullptr) {
        *_FILE_POINTER_ = fopen(filePath, "a+");
        if (*_FILE_POINTER_ == nullptr) {
            std::cerr << "Error opening the file!\n";
            WindowsNpcapSizingMainController::signalInterruptedHandler(CTRL_C_EVENT);
        } else {
            char output[1024] = {'\0'};
            int length = sprintf(output,
                                 "UTC\tType\tInterface\tPort\tNumber(amount)\tSize(bytes)\tMaxSize(bytes)\t"
                                 "SQL number in the time interval\tSQL size(bytes) in the time interval\tAverage SQL number per sec(eps)\tPeak SQL number per sec(eps)\n");
            fwrite(output, sizeof(char), length, *_FILE_POINTER_);
            if (*_FILE_POINTER_ != nullptr) {
                fclose(*_FILE_POINTER_);
                *_FILE_POINTER_ = nullptr;
            }
        }
    }

    // Set the timer to fire for the first time after _WRITING_FILE_SECOND_ seconds, and then periodically.
    LARGE_INTEGER dueTime;
    dueTime.QuadPart = (LONGLONG)(-1) * (10000000LL) * (LONGLONG)_WRITING_FILE_SECOND_;

    if (!SetWaitableTimer(_TIMER_, &dueTime, (_WRITING_FILE_SECOND_ * 1000), nullptr, nullptr, FALSE)) {
        std::cerr << "[Error] Failed to create a waitable timer.\n";
        _IS_ALARM_WORKED_ = 0x0;
        return;
    }

    // Wait for either the timer to fire or the exit event to be signaled.
    HANDLE handles[] = {_TIMER_, _EXITED_EVENT_};

    while (_IS_ALARM_WORKED_ == 0x1) {
        DWORD waitedResult = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        switch (waitedResult) {
            case WAIT_OBJECT_0: // Timer fired
                signalAlarmHandler();
            case (WAIT_OBJECT_0 + 1): // Exit event signaled
                break;
        }
    }

    // Cleanup handles and file pointers upon exiting the loop.
    {
        if (_EXITED_EVENT_ != nullptr) {
            CloseHandle(_EXITED_EVENT_);
            _EXITED_EVENT_ = nullptr;
        }
        if (_TIMER_ != nullptr) {
            CloseHandle(_TIMER_);
            _TIMER_ = nullptr;
        }
        if (*_FILE_POINTER_ != nullptr) {
            fclose(*_FILE_POINTER_);
            *_FILE_POINTER_ = nullptr;
        }
    }
}

/**
 * Callback function for pcap_loop. Processes each captured packet.
 *
 * @param userData [u_char*] User data passed from pcap_loop, expected to be a PCAPPrototype* instance.
 * @param pkthdr [const pcap_pkthdr*] The pcap packet header.
 * @param packet [const u_char*] The raw packet data.
 */
void WindowsNpcapSizingMainController::packetHandler(u_char* userData, const pcap_pkthdr* pkthdr, const u_char* packet) {
    // Initialize the starting time on the first packet.
    if (startingTime == std::chrono::steady_clock::time_point::min()) {
        startingTime = std::chrono::steady_clock::now();
    }
    PCAP::PCAPPrototype* pcapInstance = (PCAP::PCAPPrototype*)userData;
    PCAP::WindowsNpcapPCAP* npcapPCAP = nullptr;
    // Safely cast the generic PCAPPrototype to our specific WindowsNpcapPCAP type.
    if (dynamic_cast<PCAP::WindowsNpcapPCAP*>(pcapInstance)) {
        npcapPCAP = dynamic_cast<PCAP::WindowsNpcapPCAP*>(pcapInstance);
    }

    if (npcapPCAP != nullptr) {
        std::unordered_map<int, PCAP::PCAPPrototype::PCAPPortInformation*>* tmpMap = &(npcapPCAP->portRelatedInformation);
        static std::unordered_map<uint32_t, char> ipMap;

        // Parse Ethernet and IP headers.
        ip* ip_header = (ip*)(packet + sizeof(ether_header));

        // Prepare variables for transport layer headers and packet info.
        tcphdr* tcpHeader = nullptr;
        udphdr* udpHeader = nullptr;
        uint16_t packetSourcePort = 0;
        uint16_t packetDestinationPort = 0;
        uint32_t packetSourceIp = 0;
        uint32_t packetDestinationIp = 0;
        uint8_t tcpFlag = 0;

        // Extract port and IP information based on the protocol (TCP or UDP).
        switch (ip_header->ip_p) {
            case IPPROTO_TCP:
                tcpHeader = (tcphdr*)(packet + sizeof(ether_header) + sizeof(ip));
                packetSourcePort = ntohs(tcpHeader->th_sport);
                packetDestinationPort = ntohs(tcpHeader->th_dport);
                tcpFlag = tcpHeader->th_flags;
                packetSourceIp = ip_header->ip_src.s_addr;
                packetDestinationIp = ip_header->ip_dst.s_addr;
                break;
            case IPPROTO_UDP:
                udpHeader = (udphdr*)(packet + sizeof(ether_header) + sizeof(ip));
                packetSourcePort = ntohs(udpHeader->uh_sport);
                packetDestinationPort = ntohs(udpHeader->uh_dport);
                packetSourceIp = ip_header->ip_src.s_addr;
                packetDestinationIp = ip_header->ip_dst.s_addr;
                break;
            default:
                return; // Skip unknown protocols.
        }

        // Acquire lock to safely update shared statistics.
        EnterCriticalSection(&_CRITICAL_SECTION_);

        // Determine if the packet is TX or RX based on source/destination port.
        char packetTypeDetermineSet = 0x0;
        // Check if it's a TX packet.
        if (packetTypeDetermineSet == 0x0) {
            std::unordered_map<int, PCAP::PCAPPrototype::PCAPPortInformation*>::iterator it = tmpMap->find((int)packetSourcePort);
            if (it != tmpMap->end()) {
                executePacketInformationUpdate((long long)(pkthdr->len), &((it->second)->txPacketNumber), &((it->second)->txSize), &((it->second)->maxTxSize), &(npcapPCAP->txPacketNumber), &(npcapPCAP->txSize), &(npcapPCAP->maxTxSize), &packetTypeDetermineSet);
            }
        }

        // Check if it's an RX packet.
        if (packetTypeDetermineSet == 0x0) {
            std::unordered_map<int, PCAP::PCAPPrototype::PCAPPortInformation*>::iterator it = tmpMap->find((int)packetDestinationPort);
            if (it != tmpMap->end()) {
                executePacketInformationUpdate((long long)(pkthdr->len), &((it->second)->rxPacketNumber), &((it->second)->rxSize), &((it->second)->maxRxSize), &(npcapPCAP->rxPacketNumber), &(npcapPCAP->rxSize), &(npcapPCAP->maxRxSize), &packetTypeDetermineSet);
                // If it's an SQL request (PSH+ACK), update SQL-specific counters.
                if (tcpFlag == 0x18) { // PSH + ACK flag
                    (it->second)->sqlRequestNumber++;
                    (it->second)->sqlRequestSize += (long long)(pkthdr->len);
                    currentSqlMaxRequestNumberPerSec++;
                    // Calculate peak SQL requests per second.
                    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsedSeconds = now - startingTime;
                    if (elapsedSeconds.count() >= 1.0) {
                        (it->second)->sqlMaxRequestNumberPerSec = ((it->second)->sqlMaxRequestNumberPerSec) > currentSqlMaxRequestNumberPerSec ? ((it->second)->sqlMaxRequestNumberPerSec) : currentSqlMaxRequestNumberPerSec;
                        startingTime = now;
                        currentSqlMaxRequestNumberPerSec = 0;
                    }
                }
            }
        }

        // Release the lock.
        LeaveCriticalSection(&_CRITICAL_SECTION_);
    }

    // If the capture loop should be stopped, break it.
    if (_IS_PCAP_WORKED_ == 0x0) {
        if (PCAP::WindowsNpcapPCAP* npcapInstance = dynamic_cast<PCAP::WindowsNpcapPCAP*>((PCAP::PCAPPrototype*)userData)) {
            npcapInstance->pcap_breakloop();
        }
    }
}

/**
 * Helper function to update packet statistics for both port-specific and interface-total counters.
 *
 * @param packetHeaderLength [long long] The length of the current packet.
 * @param portXPacketNumber [long*] Pointer to the port's TX/RX packet count.
 * @param portXSize [long long*] Pointer to the port's TX/RX total size.
 * @param portMaxXSize [long long*] Pointer to the port's TX/RX max packet size.
 * @param XPacketNumber [long*] Pointer to the interface's total TX/RX packet count.
 * @param XSize [long long*] Pointer to the interface's total TX/RX size.
 * @param maxXSize [long long*] Pointer to the interface's max TX/RX packet size.
 * @param packetTypeDetermineSet [char*] Flag to indicate that the packet has been processed.
 */
void WindowsNpcapSizingMainController::executePacketInformationUpdate(long long packetHeaderLength, long* portXPacketNumber, long long* portXSize, long long* portMaxXSize, long* XPacketNumber, long long* XSize, long long* maxXSize, char* packetTypeDetermineSet) {
    (*portXPacketNumber)++;
    (*portXSize) += packetHeaderLength;
    if ((*portMaxXSize) < packetHeaderLength) {
        (*portMaxXSize) = packetHeaderLength;
    }
    (*XPacketNumber)++;
    (*XSize) += packetHeaderLength;
    if ((*maxXSize) < packetHeaderLength) {
        (*maxXSize) = packetHeaderLength;
    }
    (*packetTypeDetermineSet) = 0x1;
}

/**
 * Handler for console control signals (e.g., CTRL+C).
 *
 * @param signal [DWORD] The type of signal received.
 * @return [BOOL WINAPI] TRUE to indicate the signal was handled.
 */
BOOL WINAPI WindowsNpcapSizingMainController::signalInterruptedHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        std::cerr << "\nInterrupted signal occurs, please wait.\n";
        // Set flags to signal all loops to terminate.
        _IS_PCAP_WORKED_ = 0x0;
        _IS_ALARM_WORKED_ = 0x0;

        // Explicitly break the pcap_loop for all interfaces.
        for (PCAP::PCAPPrototype* pcap_ptr : _PCAP_POINTER_) {
            if (PCAP::WindowsNpcapPCAP* npcapInstance = dynamic_cast<PCAP::WindowsNpcapPCAP*>(pcap_ptr)) {
                npcapInstance->pcap_breakloop();
            }
        }

        // Signal the file writing thread to exit its wait state.
        if (_EXITED_EVENT_ != nullptr) {
            SetEvent(_EXITED_EVENT_);
        }
        // Cancel the periodic timer.
        if (_TIMER_ != nullptr) {
            CancelWaitableTimer(_TIMER_);
        }
    }
    return TRUE;
}

/**
 * Handler for the periodic timer alarm. Writes current statistics to the file and resets counters.
 */
void WindowsNpcapSizingMainController::signalAlarmHandler() {
    char output[1024] = {"\0"};
    // Re-open the file in append mode.
    if (*_FILE_POINTER_ == nullptr) {
        *_FILE_POINTER_ = fopen(_WRITING_FILE_LOCATION_, "a+");
        if (*_FILE_POINTER_ == nullptr) {
            std::cerr << "Error opening the file!\n";
            WindowsNpcapSizingMainController::signalInterruptedHandler(CTRL_C_EVENT);
        } else {
            // Acquire lock to safely read and reset statistics.
            EnterCriticalSection(&_CRITICAL_SECTION_);
            time_t timeEpoch = Commons::Time::getEpoch();
            // Iterate through all pcap instances.
            for (PCAP::PCAPPrototype* pcap_ptr : _PCAP_POINTER_) {
                if (PCAP::WindowsNpcapPCAP* tmp = dynamic_cast<PCAP::WindowsNpcapPCAP*>(pcap_ptr)) {
                    // Iterate through all monitored ports for the current instance.
                    for (auto const& [port, info] : tmp->portRelatedInformation) {
                        // Write TX statistics.
                        int length = sprintf(output, "%lu\tTX\t%s\t%d\t%lu\t%llu\t%lu\t%lu\t%llu\t%llu\t%llu\n", timeEpoch, (tmp->deviceInterface).c_str(), port, tmp->txPacketNumber, tmp->txSize, tmp->maxTxSize, (long)0, (long long)0, (long long)0, (long long)0);
                        fwrite(output, sizeof(char), length, *_FILE_POINTER_);
                        // Reset port-specific TX counters.
                        info->txGroupNumber = 0;
                        info->txSize = 0;
                        info->maxTxSize = 0;

                        // Write RX statistics.
                        length = sprintf(output, "%lu\tRX\t%s\t%d\t%lu\t%llu\t%lu\t%lu\t%llu\t%llu\t%llu\n", timeEpoch, (tmp->deviceInterface).c_str(), port, tmp->rxPacketNumber, tmp->rxSize, tmp->maxRxSize, info->sqlRequestNumber, info->sqlRequestSize, info->sqlRequestNumber / (long long)_WRITING_FILE_SECOND_, info->sqlMaxRequestNumberPerSec);
                        fwrite(output, sizeof(char), length, *_FILE_POINTER_);
                        // Reset port-specific RX and SQL counters.
                        info->rxGroupNumber = 0;
                        info->rxSize = 0;
                        info->maxRxSize = 0;
                        info->sqlRequestNumber = 0;
                        info->sqlRequestSize = 0;
                        info->sqlMaxRequestNumberPerSec = 0;
                    }
                    // Reset interface-total counters.
                    tmp->txPacketNumber = 0;
                    tmp->txGroupNumber = 0;
                    tmp->txSize = 0;
                    tmp->maxTxSize = 0;
                    tmp->rxPacketNumber = 0;
                    tmp->rxGroupNumber = 0;
                    tmp->rxSize = 0;
                    tmp->maxRxSize = 0;
                }
            }
            // Release the lock.
            LeaveCriticalSection(&_CRITICAL_SECTION_);
            // Close the file until the next write cycle.
            if (*_FILE_POINTER_ != nullptr) {
                fclose(*_FILE_POINTER_);
                *_FILE_POINTER_ = nullptr;
            }
        }
    } else {
        // If file was somehow open, close it.
        fclose(*_FILE_POINTER_);
        *_FILE_POINTER_ = nullptr;
    }
}

} // namespace SizingControllers
#endif