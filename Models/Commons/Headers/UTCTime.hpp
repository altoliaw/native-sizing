#pragma once
/** @file UTCTIme.hpp
 * The timestamp in UTC
 *
 * @author Nick, Liao
 * @date 2024/05/27
 */

#include <time.h>

namespace Commons {
class UTCTime {
   public:
	static long getEpoch ();
};
}  // namespace Commons
