/**
 * @see LinuxCentosPacketRedirectionMainController.hpp
 */
#include "../../Headers/PacketRedirectionControllers/LinuxCentosPacketRedirectionMainController.hpp"

#ifdef __linux__
#ifdef OS_ID

namespace PacketRedirectionControllers {

// Currently uses the same implementation as LinuxPacketRedirectionMainController
// Add CentOS-specific implementations here if needed in the future

}  // namespace PacketRedirectionControllers

#endif  // OS_ID
#endif  // __linux__
