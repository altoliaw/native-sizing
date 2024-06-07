/**
 * @see GMTTime.hpp
 */
#include "../Headers/PSTTime.hpp"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace Commons {

/**
 * The time for obtaining GMT epoch 
 * 
 * @return [long] The GMT epoch value; if the value is -1, the error orccurs
 */
std::string PSTTime::changeDatetime(long timestamp) {
	time_t pst_timestamp = timestamp + -8 * 3600;
	tm* pst_time = gmtime(&pst_timestamp);
	std::stringstream ss;
	ss << std::put_time(pst_time, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

long PSTTime::Timestamp(std::string datetime){
	return 0;
}

}  // namespace Commons