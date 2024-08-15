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
    int result = Commons::POSIXErrors::OK;
    #ifdef __linux__
        result = std::max((int)LinuxSysinMainCaller::start(argC, argV), result);
    #endif
}
}  // namespace SysinMainCaller