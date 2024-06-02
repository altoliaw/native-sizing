#pragma once
/** @file GMTTIme.hpp
 * The timestamp in GMT
 *
 * @author William, Chiu
 * @date 2024/06/01
 */

#include <time.h>

namespace Commons {
class NSTTTime {
   public:
	static string getEpoch_NST();
};
}  // namespace Commons