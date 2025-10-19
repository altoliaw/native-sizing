#pragma once
/** @file RedirectionStatistics.hpp
 * Statistics management for packet redirection
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#include <string>
#include <map>
#include <mutex>
#include <cstdint>

namespace PacketRedirectionServices {

/**
 * Structure representing statistics for a single target
 */
struct RedirectionTargetStats {
    std::string targetHost;
    uint16_t targetPort;
    uint64_t sentPackets;
    uint64_t sentBytes;
    uint64_t failedPackets;

    RedirectionTargetStats();
    RedirectionTargetStats(const std::string& host, uint16_t port);
    ~RedirectionTargetStats();

    void reset();
};

/**
 * Statistics manager class
 */
class RedirectionStatistics {
   private:
    std::map<std::string, RedirectionTargetStats> stats;
    std::mutex mutex;

    /**
     * Generate key for target
     */
    std::string generateKey(const std::string& host, uint16_t port) const;

   public:
    RedirectionStatistics();
    ~RedirectionStatistics();

    /**
     * Record a successful packet send
     * @param host [std::string&] Target host
     * @param port [uint16_t] Target port
     * @param packetSize [uint64_t] Size of the packet in bytes
     */
    void recordSent(const std::string& host, uint16_t port, uint64_t packetSize);

    /**
     * Record a failed packet send
     * @param host [std::string&] Target host
     * @param port [uint16_t] Target port
     */
    void recordFailure(const std::string& host, uint16_t port);

    /**
     * Get statistics for a specific target
     * @param host [std::string&] Target host
     * @param port [uint16_t] Target port
     * @return [RedirectionTargetStats] Copy of the statistics
     */
    RedirectionTargetStats getStats(const std::string& host, uint16_t port);

    /**
     * Get all statistics
     * @return [std::map<std::string, RedirectionTargetStats>] Copy of all statistics
     */
    std::map<std::string, RedirectionTargetStats> getAllStats();

    /**
     * Reset all statistics
     */
    void resetAll();

    /**
     * Export statistics to file
     * @param filePath [const char*] Path to output file
     * @param layoutType [int] Output format type
     * @return [bool] True if successful
     */
    bool exportToFile(const char* filePath, int layoutType);
};

}  // namespace PacketRedirectionServices
