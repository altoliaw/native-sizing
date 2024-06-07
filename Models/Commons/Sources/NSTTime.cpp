/**
 * @see GMTTime.hpp
 */
#include "../Headers/NSTTime.hpp"
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>


namespace Commons {

/**
 * The time for obtaining GMT epoch 
 * 
 * @return [long] The GMT epoch value; if the value is -1, the error orccurs
 */
std::string NSTTTime::changeDatetime(long timestamp){
	//time_t now = time(NULL);
	time_t nst_timestamp = (long)timestamp + 8 * 3600;
	tm* nst_time = gmtime(&nst_timestamp);
	std::stringstream ss;
	ss << std::put_time(nst_time, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

long NSTTTime::Timestamp(std::string datetime){
	return 0;
}

}  // namespace Commons