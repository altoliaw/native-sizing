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
string PSTTime::getEpoch_PST() {
	time_t now = time(NULL);
	time_t pst_timestamp = (long)now + -8 * 3600;
	tm pst_time;
	gmtime(&pst_timestamp, &pst_time); 
	stringstream ss;
	ss << std::put_time(&pst_time, "%Y-%m-%d %H:%M:%S");
	return ss;
}

}  // namespace Commons