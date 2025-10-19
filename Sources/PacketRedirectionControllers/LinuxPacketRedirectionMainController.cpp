/**
 * @see LinuxPacketRedirectionMainController.hpp
 */
#include "../../Headers/PacketRedirectionControllers/LinuxPacketRedirectionMainController.hpp"

#ifdef __linux__
#ifndef OS_ID

namespace PacketRedirectionControllers {

//===Global Declaration===
// Variables from config file
char* _WRITING_FILE_LOCATION_ = nullptr;
unsigned int _WRITING_FILE_SECOND_ = 30;
unsigned int _OUTPUT_LAYOUT_TYPE_ = 0;

// Runtime flags
volatile char _IS_PCAP_WORKED_ = 0x1;
volatile char _IS_ALARM_WORKED_ = 0x1;

// Synchronization
std::mutex _MUTEX_;

// Global objects
std::vector<PCAP::PCAPPrototype*> _PCAP_POINTER_;
FILE** _FILE_POINTER_ = nullptr;
PacketRedirectionServices::LoadBalancer* _LOAD_BALANCER_ = nullptr;
PacketRedirectionServices::RedirectionStatistics* _STATISTICS_ = nullptr;
int _RAW_SOCKET_ = -1;

/**
 * The starting process, the entry of the process
 */
Commons::POSIXErrors LinuxPacketRedirectionMainController::start(int argC, char** argV) {
    Commons::POSIXErrors result = Commons::POSIXErrors::OK;

    // Parse config file
    std::vector<CaptureConfig> captureConfigs;
    std::vector<RedirectionTarget> targets;

    result = config(&captureConfigs, &targets);
    if (result != Commons::POSIXErrors::OK) {
        fprintf(stderr, "Error: Failed to load configuration\n");
        return result;
    }

    if (captureConfigs.empty() || targets.empty()) {
        fprintf(stderr, "Error: No capture config or targets configured\n");
        return Commons::POSIXErrors::EINVAL;
    }

    // Initialize load balancer
    PacketRedirectionServices::LoadBalancer loadBalancer;
    loadBalancer.setTargets(targets);
    _LOAD_BALANCER_ = &loadBalancer;

    // Initialize statistics
    PacketRedirectionServices::RedirectionStatistics statistics;
    _STATISTICS_ = &statistics;

    // Create raw socket for sending packets
    _RAW_SOCKET_ = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (_RAW_SOCKET_ < 0) {
        perror("Error: Failed to create raw socket");
        return Commons::POSIXErrors::EACCES;
    }

    // Allow socket to send packets
    int one = 1;
    const int *val = &one;
    if (setsockopt(_RAW_SOCKET_, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
        perror("Error: Failed to set socket options");
        close(_RAW_SOCKET_);
        return Commons::POSIXErrors::EINVAL;
    }

    // Setup output file path
    char outputFilePathRule[] = "Outputs/packetRedirection_%lu.tsv";
    char outputFilePathWithTime[128] = {'\0'};
    sprintf(outputFilePathWithTime, outputFilePathRule, Commons::Time::getEpoch());
    _WRITING_FILE_LOCATION_ = outputFilePathWithTime;

    // Install signal handlers
    signal(SIGINT, LinuxPacketRedirectionMainController::signalInterruptedHandler);
    signal(SIGALRM, LinuxPacketRedirectionMainController::signalAlarmHandler);

    {
        // Create PCAP objects for each capture interface
        std::vector<PCAP::LinuxPCAP*> pcapObjectOfInterface;
        std::vector<std::thread> packetThreads;

        for (size_t i = 0; i < captureConfigs.size(); i++) {
            PCAP::LinuxPCAP* pcap = new PCAP::LinuxPCAP();

            try {
                pcap->open(captureConfigs[i].interfaceName, 1, 65535, 1000,
                           &captureConfigs[i].ports);
            } catch (...) {
                fprintf(stderr, "Error: Failed to open interface %s\n",
                        captureConfigs[i].interfaceName);
                delete pcap;
                continue;
            }

            pcapObjectOfInterface.push_back(pcap);
            _PCAP_POINTER_.push_back(pcap);
        }

        if (pcapObjectOfInterface.empty()) {
            fprintf(stderr, "Error: No interfaces could be opened\n");
            close(_RAW_SOCKET_);
            return Commons::POSIXErrors::EIO;
        }

        // Start packet capture threads
        for (size_t i = 0; i < pcapObjectOfInterface.size(); i++) {
            packetThreads.push_back(
                std::thread(packetTask, pcapObjectOfInterface[i], packetHandler));
        }

        // Start statistics file output thread
        FILE* fp = nullptr;
        std::thread fileThread(statisticsFileTask, &fp, _WRITING_FILE_LOCATION_);

        // Set alarm for periodic statistics output
        alarm(_WRITING_FILE_SECOND_);

        // Wait for all threads to complete
        for (auto& thread : packetThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        _IS_ALARM_WORKED_ = 0x0;
        if (fileThread.joinable()) {
            fileThread.join();
        }

        // Cleanup
        for (auto* pcap : pcapObjectOfInterface) {
            pcap->close();
            delete pcap;
        }

        pcapObjectOfInterface.clear();
        _PCAP_POINTER_.clear();

        if (fp != nullptr) {
            fclose(fp);
        }
    }

    // Close raw socket
    if (_RAW_SOCKET_ >= 0) {
        close(_RAW_SOCKET_);
        _RAW_SOCKET_ = -1;
    }

    return result;
}

/**
 * Signal handler for SIGINT
 */
void LinuxPacketRedirectionMainController::signalInterruptedHandler(int sig) {
    _IS_PCAP_WORKED_ = 0x0;

    for (auto* pcap : _PCAP_POINTER_) {
        if (pcap != nullptr) {
            pcap->close();
        }
    }

    printf("\nInterrupted by user (SIGINT)\n");
}

/**
 * Signal handler for SIGALRM
 */
void LinuxPacketRedirectionMainController::signalAlarmHandler(int sig) {
    // Re-arm the alarm
    alarm(_WRITING_FILE_SECOND_);
}

/**
 * Configuration loader
 */
Commons::POSIXErrors LinuxPacketRedirectionMainController::config(
    std::vector<CaptureConfig>* captureConfigs,
    std::vector<RedirectionTarget>* targets) {

    Commons::POSIXErrors result = Commons::POSIXErrors::OK;

    // Parse JSON config file
    const char* configPath = "Settings/.Json/PacketRedirectionMain.json";
    FileParsers::InitializedJsonFileParser parser;

    try {
        parser.open(configPath);
    } catch (...) {
        fprintf(stderr, "Error: Failed to open config file: %s\n", configPath);
        return Commons::POSIXErrors::ENOENT;
    }

    // Parse capture configuration
    auto baseNode = parser.getNode("base");
    if (baseNode == nullptr) {
        fprintf(stderr, "Error: 'base' node not found in config\n");
        return Commons::POSIXErrors::EINVAL;
    }

    auto captureNode = parser.getNode("base.capture");
    if (captureNode != nullptr) {
        CaptureConfig config;

        auto interfaceNode = parser.getNode("base.capture.interface");
        if (interfaceNode != nullptr) {
            std::string interfaceName = parser.getStringValue(interfaceNode);
            strncpy(config.interfaceName, interfaceName.c_str(),
                    sizeof(config.interfaceName) - 1);
        }

        auto portsNode = parser.getNode("base.capture.port");
        if (portsNode != nullptr) {
            auto portArray = parser.getArray(portsNode);
            for (const auto& portStr : portArray) {
                config.ports.push_back(std::stoi(portStr));
            }
        }

        captureConfigs->push_back(config);
    }

    // Parse target configurations
    auto targetsNode = parser.getNode("base.targets");
    if (targetsNode != nullptr) {
        auto targetsArray = parser.getObjectArray(targetsNode);

        for (const auto& targetObj : targetsArray) {
            std::string host;
            uint16_t port = 0;

            if (targetObj.find("host") != targetObj.end()) {
                host = targetObj.at("host");
            }
            if (targetObj.find("port") != targetObj.end()) {
                port = static_cast<uint16_t>(std::stoi(targetObj.at("port")));
            }

            if (!host.empty() && port > 0) {
                targets->push_back(RedirectionTarget(host, port));
            }
        }
    }

    // Parse statistics configuration
    auto statsNode = parser.getNode("base.statistics");
    if (statsNode != nullptr) {
        auto writingSecondNode = parser.getNode("base.statistics.writingFileSecond");
        if (writingSecondNode != nullptr) {
            _WRITING_FILE_SECOND_ = static_cast<unsigned int>(
                parser.getIntValue(writingSecondNode));
        }

        auto layoutNode = parser.getNode("base.statistics.outputLayoutType");
        if (layoutNode != nullptr) {
            _OUTPUT_LAYOUT_TYPE_ = static_cast<unsigned int>(
                parser.getIntValue(layoutNode));
        }
    }

    parser.close();
    return result;
}

/**
 * Packet handler callback
 */
void LinuxPacketRedirectionMainController::packetHandler(
    u_char* userData,
    const struct pcap_pkthdr* pkthdr,
    const u_char* packet) {

    if (_LOAD_BALANCER_ == nullptr || _STATISTICS_ == nullptr || _RAW_SOCKET_ < 0) {
        return;
    }

    // Select target using load balancer
    RedirectionTarget* target = _LOAD_BALANCER_->selectTarget();
    if (target == nullptr) {
        return;
    }

    // Prepare destination address
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(target->port);
    destAddr.sin_addr.s_addr = inet_addr(target->host.c_str());

    // Send the raw packet
    ssize_t sentBytes = sendto(_RAW_SOCKET_, packet, pkthdr->len, 0,
                               (struct sockaddr*)&destAddr, sizeof(destAddr));

    // Record statistics
    std::lock_guard<std::mutex> lock(_MUTEX_);
    if (sentBytes > 0) {
        _STATISTICS_->recordSent(target->host, target->port,
                                 static_cast<uint64_t>(sentBytes));
    } else {
        _STATISTICS_->recordFailure(target->host, target->port);
    }
}

/**
 * Packet capture task
 */
void LinuxPacketRedirectionMainController::packetTask(
    PCAP::LinuxPCAP* pcap,
    void (*handler)(u_char*, const pcap_pkthdr*, const u_char*)) {

    if (pcap == nullptr) {
        return;
    }

    pcap->execute(handler);
}

/**
 * Statistics file output task
 */
void LinuxPacketRedirectionMainController::statisticsFileTask(
    FILE** filePtr,
    const char* filePath) {

    while (_IS_ALARM_WORKED_) {
        sleep(1);

        if (_STATISTICS_ != nullptr && filePath != nullptr) {
            _STATISTICS_->exportToFile(filePath, _OUTPUT_LAYOUT_TYPE_);
        }
    }
}

}  // namespace PacketRedirectionControllers

#endif  // OS_ID
#endif  // __linux__
