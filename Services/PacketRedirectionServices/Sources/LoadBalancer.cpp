/**
 * @see LoadBalancer.hpp
 */
#include "../Headers/LoadBalancer.hpp"

namespace PacketRedirectionServices {

/**
 * Constructor
 */
LoadBalancer::LoadBalancer() : currentIndex(0) {
}

/**
 * Destructor
 */
LoadBalancer::~LoadBalancer() {
    targets.clear();
}

/**
 * Set the targets for load balancing
 */
void LoadBalancer::setTargets(const std::vector<PacketRedirectionControllers::RedirectionTarget>& targetList) {
    targets = targetList;
    currentIndex = 0;
}

/**
 * Select the next target using round-robin algorithm
 */
PacketRedirectionControllers::RedirectionTarget* LoadBalancer::selectTarget() {
    if (targets.empty()) {
        return nullptr;
    }

    // Round-robin: select current target and move to next
    PacketRedirectionControllers::RedirectionTarget* selected = &targets[currentIndex];
    currentIndex = (currentIndex + 1) % targets.size();

    return selected;
}

/**
 * Get the number of configured targets
 */
size_t LoadBalancer::getTargetCount() const {
    return targets.size();
}

}  // namespace PacketRedirectionServices
