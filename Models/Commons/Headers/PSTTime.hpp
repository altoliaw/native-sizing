#pragma once
/** @file GMTTIme.hpp
 * The timestamp in GMT
 *
 * @author William, Chiu
 * @date 2024/06/07
 */
#include "../Headers/TimePrototype.hpp"
#include <time.h>

namespace Commons {
class PSTTime:TimeStamp {
   public:
	std::string changeDatetime(long timestamp);
   long Timestamp(std::string datetime);
};
}  // namespace Commons