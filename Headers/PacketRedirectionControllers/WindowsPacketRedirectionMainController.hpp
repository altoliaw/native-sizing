#pragma once
/** @file WindowsPacketRedirectionMainController.hpp
 * The headers for Windows packet redirection controller
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#ifdef _WIN32

#include "../../Models/Commons/Headers/POSIXErrors.hpp"
#include "./PacketRedirectionMainControllerPrototype.hpp"

namespace PacketRedirectionControllers {

/**
 * Windows packet redirection controller
 * To be implemented in the future using WinDivert
 */
class WindowsPacketRedirectionMainController : public PacketRedirectionMainControllerPrototype {
   public:
    Commons::POSIXErrors start(int argc, char** argv);
};

}  // namespace PacketRedirectionControllers

#endif  // _WIN32
