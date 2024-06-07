#pragma once
/** @file TimePrototype.hpp
 * The timestamp in UTC
 *
 * @author William, Chiu
 * @date 2024/06/06
 */
#include <time.h>
#include <iostream>
#include <string>

namespace Commons {
class TimeStamp {
   public:
	virtual long Timestamp(std::string datetime) = 0;
    virtual std::string changeDatetime(long timestamp) = 0;
};
}  // namespace Commons