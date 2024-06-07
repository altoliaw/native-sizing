#pragma once
/** @file GMTTIme.hpp
 * The timestamp in GMT
 *
 * @author William, Chiu
 * @date 2024/06/01
 */

#include "../Headers/TimePrototype.hpp"
#include <time.h>
//#include "/root/GitPj/trafficMonitor/Models/Commons/Headers/TimePrototype.hpp"

namespace Commons {
class NSTTime:TimeStamp {
   public:
   long Timestamp(std::string datetime);
   std::string changeDatetime(long timestamp);
	//static string getEpoch_NST();
};
}  // namespace Commons