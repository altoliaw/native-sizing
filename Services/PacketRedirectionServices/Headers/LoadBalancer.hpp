#pragma once
/** @file LoadBalancer.hpp
 * Load balancer for packet redirection targets
 *
 * @author Claude Code
 * @date 2025/10/19
 */

#include <vector>
#include <string>
#include "../../../Headers/PacketRedirectionControllers/PacketRedirectionMainControllerPrototype.hpp"

namespace PacketRedirectionServices {

/**
 * Load balancer class implementing round-robin algorithm
 */
class LoadBalancer {
   private:
    size_t currentIndex;
    std::vector<PacketRedirectionControllers::RedirectionTarget> targets;

   public:
    LoadBalancer();
    ~LoadBalancer();

    /**
     * Set the targets for load balancing
     * @param targetList [std::vector<RedirectionTarget>&] The list of targets
     */
    void setTargets(const std::vector<PacketRedirectionControllers::RedirectionTarget>& targetList);

    /**
     * Select the next target using round-robin algorithm
     * @return [RedirectionTarget*] Pointer to the selected target, nullptr if no targets available
     */
    PacketRedirectionControllers::RedirectionTarget* selectTarget();

    /**
     * Get the number of configured targets
     * @return [size_t] Number of targets
     */
    size_t getTargetCount() const;
};

}  // namespace PacketRedirectionServices
