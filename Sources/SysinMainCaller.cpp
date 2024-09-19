/**
 * @see SysinMainCaller.hpp
 */
#include "../Headers/SysinMainCaller.hpp"

namespace SysinMainCaller {

/**
 * The starting process, the entry of the process
 *
 * @param argC [int] The number of the argument
 * @param argV [char**] The array of the argument
 * @return [Commons::POSIXErrors] The status defined in the class "POSIXErrors" The status defined in the class "POSIXErrors"
 */
Commons::POSIXErrors SysinMainCaller::start(int argC, char** argV) {
    Commons::POSIXErrors result = Commons::POSIXErrors::OK;
    SysinMainCallerPrototype* caller;

#ifdef __linux__
    LinuxSysinMainCaller instance;
    caller = &instance;
#elif defined(_WIN32)
    WindowsSysinMainCaller instance;
    caller = &instance;
#endif
    
    result = caller->start(argC, argV);
    return result;
}
}  // namespace SysinMainCaller