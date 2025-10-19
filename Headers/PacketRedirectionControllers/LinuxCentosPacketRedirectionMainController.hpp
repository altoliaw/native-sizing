#pragma once
/** @file LinuxCentosPacketRedirectionMainController.hpp
 * The headers for CentOS-specific packet redirection controller
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#ifdef __linux__
#ifdef OS_ID

#include "./LinuxPacketRedirectionMainController.hpp"

namespace PacketRedirectionControllers {

/**
 * CentOS-specific packet redirection controller
 * Inherits from Linux controller, can override methods if needed
 */
class LinuxCentosPacketRedirectionMainController : public LinuxPacketRedirectionMainController {
   public:
    // Currently uses the same implementation as Linux
    // Override methods here if CentOS-specific behavior is needed
};

}  // namespace PacketRedirectionControllers

#endif  // OS_ID
#endif  // __linux__
