#include "../../Headers/SizingControllers/SizingMainCaller.hpp"

int main(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;

    // Main process
    SizingMainCaller::SizingMainCaller caller;
    result = caller.start(argC, argV);

    return result;
}