/**
 * @see GMTTime.hpp
 */
#include "../Headers/GMTTime.hpp"
#include <time.h>

#include <iomanip>
#include <sstream>

namespace Commons {

/**
 * The time for obtaining GMT epoch 
 * 
 * @return [long] The GMT epoch value; if the value is -1, the error orccurs
 */
long PSTTime::getEpoch_NST() {
	time_t now = time(NULL);
	time_t nst_timestamp = (long)now + 8 * 3600;
	tm nst_time;
	gmtime(&nst_timestamp, &nst_time); 
	stringstream ss;
	ss << std::put_time(&nst_time, "%Y-%m-%d %H:%M:%S");
	return ss;
}

}  // namespace Commons