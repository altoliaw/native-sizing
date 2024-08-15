#include "../Headers/SysinMainCaller.hpp"

int main(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;
    SysinMainCaller::SysinMainCaller caller;
    // Main process
    result = caller.start(argC, argV);
    // result = std::max((int)SysinMainCaller::start(argC, argV), result);

    return result;
}