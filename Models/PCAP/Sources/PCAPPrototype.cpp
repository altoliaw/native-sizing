/**
 * @see PCAPPrototype.hpp
 */
#include "../Headers/PCAPPrototype.hpp"

namespace PCAP {

/**
 * Constructor
 */
PCAPPrototype::PCAPPortInformation::PCAPPortInformation() {
    port = 0;
    sqlRequestNumber = 0;
    sqlRequestSize = 0;
}

/**
 * Destructor
 */
PCAPPrototype::PCAPPortInformation::~PCAPPortInformation() {
    port = 0;
    sqlRequestNumber = 0;
    sqlRequestSize = 0;
}

/**
 * Obtaining the SQL eps.
 * 
 * @param timeInterval [int] The time interval for eps; the unit is "second"
 * @return [long] The SQL eps
 */
long PCAPPrototype::PCAPPortInformation::sqlRequestNumberPerTime(int timeInterval) {
    return sqlRequestNumber/timeInterval;
}

}  // namespace PCAP