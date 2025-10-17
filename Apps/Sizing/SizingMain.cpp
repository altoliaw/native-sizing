#include "../../Headers/SizingControllers/SizingMainCaller.hpp"

int main(int argC, char** argV) {
    int result = Commons::POSIXErrors::OK;

    // Main process
    SizingControllers::SizingMainCaller caller;
    result = caller.start(argC, argV);

    return result;
}