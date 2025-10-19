#pragma once
/** @file PacketRedirectionMainControllerPrototype.hpp
 * The prototypes of PacketRedirectionMainController
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#include <string>
#include <vector>
#include "../../Models/Commons/Headers/POSIXErrors.hpp"

namespace PacketRedirectionControllers {

/**
 * Structure representing a redirection target
 */
struct RedirectionTarget {
    std::string host;
    uint16_t port;

    RedirectionTarget();
    RedirectionTarget(const std::string& h, uint16_t p);
    ~RedirectionTarget();
};

/**
 * Structure representing capture configuration
 */
struct CaptureConfig {
    char interfaceName[256];
    std::vector<int> ports;

    CaptureConfig();
    ~CaptureConfig();
};

/**
 * Abstract base class for packet redirection controllers
 */
class PacketRedirectionMainControllerPrototype {
   public:
    virtual Commons::POSIXErrors start(int argc, char** argv) = 0;
    virtual ~PacketRedirectionMainControllerPrototype() = default;
};

}  // namespace PacketRedirectionControllers
