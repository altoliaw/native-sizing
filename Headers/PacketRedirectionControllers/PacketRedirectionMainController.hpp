#pragma once
/** @file PacketRedirectionMainController.hpp
 * The factory controller for packet redirection
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#include "../../Models/Commons/Headers/POSIXErrors.hpp"
#include "./PacketRedirectionMainControllerPrototype.hpp"

namespace PacketRedirectionControllers {

/**
 * Factory class that selects the appropriate platform-specific controller
 */
class PacketRedirectionMainController {
   public:
    Commons::POSIXErrors start(int argc, char** argv);
};

}  // namespace PacketRedirectionControllers
