#pragma once
/** @file SysinMainCaller.hpp
 * The headers and global variables from other package
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#include <algorithm>

#include "./SysinMainCallerPrototype.hpp"
#include "../Models/Commons/Headers/POSIXErrors.hpp"

#ifdef __linux__  // Linux

#include "./LinuxSysinMainCaller.hpp"

#elif defined(_WIN32) || defined(_WIN64)

#include "./WindowsSysinMainCaller.hpp"

#endif

namespace SysinMainCaller {
class SysinMainCaller {
   public:
    Commons::POSIXErrors start(int, char**);
};
}  // namespace SysinMainCaller