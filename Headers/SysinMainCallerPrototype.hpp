#pragma once
/** @file SysinMainCallerPrototype.hpp
 * The prototypes of SysinMainCaller
 *
 * @author Nick, Liao
 * @date 2024/08/15
 */

#include <vector>
#include "../Models/Commons/Headers/POSIXErrors.hpp"

namespace SysinMainCaller {
struct unitService {
   public:
    char interfaceName[256];
    std::vector<int> port;
    unitService();
    ~unitService();
};

class SysinMainCallerPrototype {
   public:
    virtual Commons::POSIXErrors start(int, char**) = 0;
    virtual void signalInterruptedHandler(int) = 0;
    virtual void signalAlarmHandler(int) = 0;
};
}  // namespace SysinMainCaller