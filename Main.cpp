#include "./Headers/MainCaller.hpp"

int main(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;

    // Main process
    result = std::max(start(argC, argV), result);

    return result;
}