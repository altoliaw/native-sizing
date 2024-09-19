#include "../Headers/SysinMainCaller.hpp"

int main(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;

    // Main process
    SysinMainCaller::SysinMainCaller caller;
    result = caller.start(argC, argV);

    return result;
}