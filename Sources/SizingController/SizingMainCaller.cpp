/**
 * @see SizingMainCaller.hpp
 */
#include "../../Headers/SizingController/SizingMainCaller.hpp"

namespace SizingMainCaller {

/**
 * The starting process, the entry of the process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors" The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors SizingMainCaller::start(int argC, char** argV) {
    Commons::POSIXErrors result = Commons::POSIXErrors::OK;
    SizingMainCallerPrototype* caller;

#ifdef __linux__
// The macro variable is from the compiled process where is in the CMakeLists.txt file from the proper cpp file folder
// For more variable information, please refer to the file, namely .globalCmakeVariable.cmake
// When the OS_ID is not defined in the CMakeLists.txt file from the proper cpp file folder,
// the OS_ID will not be passed defined into compiled process
#ifndef OS_ID
    LinuxSizingMainCaller instance;
#else
    LinuxCentosSizingMainCaller instance;
#endif

    caller = &instance;
#elif defined(_WIN32)
    WindowsSizingMainCaller instance;
    caller = &instance;
#endif
    
    result = caller->start(argC, argV);
    return result;
}
}  // namespace SizingMainCaller