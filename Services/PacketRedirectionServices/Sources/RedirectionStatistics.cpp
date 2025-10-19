/**
 * @see RedirectionStatistics.hpp
 */
#include "../Headers/RedirectionStatistics.hpp"
#include <fstream>
#include <sstream>

namespace PacketRedirectionServices {

/**
 * RedirectionTargetStats Constructor
 */
RedirectionTargetStats::RedirectionTargetStats()
    : targetHost("")
    , targetPort(0)
    , sentPackets(0)
    , sentBytes(0)
    , failedPackets(0) {
}

/**
 * RedirectionTargetStats Constructor with parameters
 */
RedirectionTargetStats::RedirectionTargetStats(const std::string& host, uint16_t port)
    : targetHost(host)
    , targetPort(port)
    , sentPackets(0)
    , sentBytes(0)
    , failedPackets(0) {
}

/**
 * RedirectionTargetStats Destructor
 */
RedirectionTargetStats::~RedirectionTargetStats() {
}

/**
 * Reset statistics
 */
void RedirectionTargetStats::reset() {
    sentPackets = 0;
    sentBytes = 0;
    failedPackets = 0;
}

/**
 * RedirectionStatistics Constructor
 */
RedirectionStatistics::RedirectionStatistics() {
}

/**
 * RedirectionStatistics Destructor
 */
RedirectionStatistics::~RedirectionStatistics() {
    stats.clear();
}

/**
 * Generate key for target
 */
std::string RedirectionStatistics::generateKey(const std::string& host, uint16_t port) const {
    std::ostringstream oss;
    oss << host << ":" << port;
    return oss.str();
}

/**
 * Record a successful packet send
 */
void RedirectionStatistics::recordSent(const std::string& host, uint16_t port, uint64_t packetSize) {
    std::lock_guard<std::mutex> lock(mutex);

    std::string key = generateKey(host, port);

    if (stats.find(key) == stats.end()) {
        stats[key] = RedirectionTargetStats(host, port);
    }

    stats[key].sentPackets++;
    stats[key].sentBytes += packetSize;
}

/**
 * Record a failed packet send
 */
void RedirectionStatistics::recordFailure(const std::string& host, uint16_t port) {
    std::lock_guard<std::mutex> lock(mutex);

    std::string key = generateKey(host, port);

    if (stats.find(key) == stats.end()) {
        stats[key] = RedirectionTargetStats(host, port);
    }

    stats[key].failedPackets++;
}

/**
 * Get statistics for a specific target
 */
RedirectionTargetStats RedirectionStatistics::getStats(const std::string& host, uint16_t port) {
    std::lock_guard<std::mutex> lock(mutex);

    std::string key = generateKey(host, port);

    if (stats.find(key) != stats.end()) {
        return stats[key];
    }

    return RedirectionTargetStats(host, port);
}

/**
 * Get all statistics
 */
std::map<std::string, RedirectionTargetStats> RedirectionStatistics::getAllStats() {
    std::lock_guard<std::mutex> lock(mutex);
    return stats;
}

/**
 * Reset all statistics
 */
void RedirectionStatistics::resetAll() {
    std::lock_guard<std::mutex> lock(mutex);

    for (auto& pair : stats) {
        pair.second.reset();
    }
}

/**
 * Export statistics to file
 */
bool RedirectionStatistics::exportToFile(const char* filePath, int layoutType) {
    std::lock_guard<std::mutex> lock(mutex);

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        return false;
    }

    // Header
    if (layoutType == 1) {
        // TSV format (tab-separated values)
        outFile << "Target Host\tTarget Port\tSent Packets\tSent Bytes\tFailed Packets\n";

        for (const auto& pair : stats) {
            const RedirectionTargetStats& stat = pair.second;
            outFile << stat.targetHost << "\t"
                    << stat.targetPort << "\t"
                    << stat.sentPackets << "\t"
                    << stat.sentBytes << "\t"
                    << stat.failedPackets << "\n";
        }
    } else {
        // Default format
        outFile << "Packet Redirection Statistics\n";
        outFile << "==============================\n\n";

        for (const auto& pair : stats) {
            const RedirectionTargetStats& stat = pair.second;
            outFile << "Target: " << stat.targetHost << ":" << stat.targetPort << "\n";
            outFile << "  Sent Packets: " << stat.sentPackets << "\n";
            outFile << "  Sent Bytes: " << stat.sentBytes << "\n";
            outFile << "  Failed Packets: " << stat.failedPackets << "\n";
            outFile << "\n";
        }
    }

    outFile.close();
    return true;
}

}  // namespace PacketRedirectionServices
