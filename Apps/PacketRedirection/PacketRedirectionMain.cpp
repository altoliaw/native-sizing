#include "../../Headers/PacketRedirectionControllers/PacketRedirectionMainController.hpp"

int main(int argC, char** argV) {
    int result = static_cast<int>(Commons::POSIXErrors::OK);

    // Main process
    PacketRedirectionControllers::PacketRedirectionMainController controller;
    result = static_cast<int>(controller.start(argC, argV));

    return result;
}
