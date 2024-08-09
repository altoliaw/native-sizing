#include "../Headers/SysinMainCaller.hpp"

int main(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;

    // Main process
    result = std::max((int)SysinMainCaller::start(argC, argV), result);

    return result;
}