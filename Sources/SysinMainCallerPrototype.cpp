/**
 * @see SysinMainCallerPrototype.hpp
 */
#include "../Headers/SysinMainCallerPrototype.hpp"

namespace SysinMainCaller {

/**
 * Constructor
 */
unitService::unitService() {
    interfaceName[0] = '\0';
    if (port.empty() == false) {
        port.clear();
        port.shrink_to_fit();
    }
}

/**
 * Destructor
 */
unitService::~unitService() {
    interfaceName[0] = '\0';
    if (port.empty() == false) {
        port.clear();
        port.shrink_to_fit();
    }
}

}  // namespace SysinMainCaller