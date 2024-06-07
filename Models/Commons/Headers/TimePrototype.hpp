#pragma once
/** @file TimePrototype.hpp
 * The timestamp in UTC
 *
 * @author William, Chiu
 * @date 2024/06/06
 */

#include <time.h>

namespace Commons {
class TimeStamp {
   public:
	virtual long Timestamp(std::string datetime) = 0;
    virtual std::string chageDatetime(long timestamp) = 0;
};
}  // namespace Commons