/**
 * @see PacketRedirectionMainControllerPrototype.hpp
 */
#include "../../Headers/PacketRedirectionControllers/PacketRedirectionMainControllerPrototype.hpp"
#include <cstring>

namespace PacketRedirectionControllers {

/**
 * RedirectionTarget Constructor
 */
RedirectionTarget::RedirectionTarget() : host(""), port(0) {
}

/**
 * RedirectionTarget Constructor with parameters
 */
RedirectionTarget::RedirectionTarget(const std::string& h, uint16_t p)
    : host(h), port(p) {
}

/**
 * RedirectionTarget Destructor
 */
RedirectionTarget::~RedirectionTarget() {
}

/**
 * CaptureConfig Constructor
 */
CaptureConfig::CaptureConfig() {
    memset(interfaceName, 0, sizeof(interfaceName));
}

/**
 * CaptureConfig Destructor
 */
CaptureConfig::~CaptureConfig() {
    memset(interfaceName, 0, sizeof(interfaceName));
    ports.clear();
}

}  // namespace PacketRedirectionControllers
