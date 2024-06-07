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
string NSTTTime::chageDatetime(long timestamp){
	//time_t now = time(NULL);
	time_t nst_timestamp = (long)timestamp + 8 * 3600;
	tm* nst_time = gmtime(&nst_timestamp);
	std::stringstream ss;
	ss << std::put_time(&nst_time, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

long NSTTTime::Timestamp(std::string datetime){
	return 0;
}

}  // namespace Commons