/**
 * @see UTCTime.hpp
 */
#include "../Headers/UTCTime.hpp"

namespace Commons {

/**
 * The time for obtaining UTC epoch 
 * 
 * @return [long] The UTC epoch value; if the value is -1, the error orccurs
 */
long UTCTime::getEpoch() {
	time_t now = time(NULL);
	return (now <= -1) ? (long) -1: (long)now;
}

}  // namespace Commons