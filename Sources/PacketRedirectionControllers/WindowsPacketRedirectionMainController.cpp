/**
 * @see WindowsPacketRedirectionMainController.hpp
 */
#include "../../Headers/PacketRedirectionControllers/WindowsPacketRedirectionMainController.hpp"

#ifdef _WIN32

#include <stdio.h>

namespace PacketRedirectionControllers {

/**
 * Windows implementation - to be completed in the future
 */
Commons::POSIXErrors WindowsPacketRedirectionMainController::start(int argC, char** argV) {
    fprintf(stderr, "Error: Windows implementation not yet available\n");
    fprintf(stderr, "Please use Linux platform for packet redirection\n");
    return Commons::POSIXErrors::ENOTSUP;
}

}  // namespace PacketRedirectionControllers

#endif  // _WIN32
