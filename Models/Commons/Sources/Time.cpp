/**
 * @see Time.hpp
 */
#include "../Headers/Time.hpp"
namespace Commons {

/**
 * The function is the first method when using static methods in this class; all
 * variables in the class will be initialized
 */
void Time::getTimeInitialization() {
    time_t now = time(NULL);
    static long timeEpoch = (now <= -1) ? (long)-1 : (long)now;
    Time::timeEpochPointer = &timeEpoch;
}

/**
 * The time for obtaining the epoch
 *
 * @param timeInstance [time_t] The time value; the default value is the current time; please verifying the
 * setting in the declaration
 * @return [long] The epoch; if the value is -1, the error orccurs
 */
long Time::getEpoch(time_t timeInstance) {
    // Reserving the value of the parameter into the static variable in the function, getTimeInitialization
    *(Time::timeEpochPointer) = timeInstance;
    return (timeInstance <= -1) ? (long)-1 : (long)timeInstance;
}

/**
 * Transforming the POSIX ("%Y-%m-%d %H:%M:%S") time string into the epoch from OS time setting
 *
 * @param timeString [const char*] The time string (e.g., "2024-06-07 15:30:00")
 * @return [long] The epoch; if the value is -1, the error orccurs
 */
long Time::getStringToEpoch(const char* timeString) {
    // ISO C "broken-down" time structure
    tm tm;
    memset(&tm, 0, sizeof(tm));

    // Parsing the string into the tm object
    if (strptime(timeString, "%Y-%m-%d %H:%M:%S", &tm) == nullptr) {
        std::cerr << "Failed to parse date string\n";
        return -1;
    }

    // Putting the time by using the mktime and reserving the value into the static variable from the
    // function, getTimeInitialization
    *(Time::timeEpochPointer) = (long)mktime(&tm);

    // Determining if the OS time is on the summer/daylight time
    if (tm.tm_isdst == 1) {
        *(Time::timeEpochPointer) -= 3600;  // -1 hour
    }

    return *(Time::timeEpochPointer);
}

/**
 * Obtaining the time string with user-defined format
 *
 * @param format [const char*] The layout format presented by using POSIX time format ("%Y-%m-%d %H:%M:%S")
 * @param zone [Time::TimeZone] The time zone definined in the enumeration
 * @param timeEpoch [long] The epoch which users define for transforming to the time string;
 * the default value is from the static variable in the funciton, getTimeInitialization
 * @return [std::string] The time string
 */
std::string Time::getEpochToString(const char* format, Time::TimeZone zone, long timeEpoch) {
    // The pointer of the ISO C "broken-down" time structure
    tm* tm;
    time_t tmpTimeEpoch = 0;

    // Setting the new time zone
    switch (zone) {
        case Time::TimeZone::UTC:
        case Time::TimeZone::GMT:
            tm = gmtime(&timeEpoch);
            break;

        case Time::TimeZone::PST:
            // The offset between the PST and UTC is 8 hours. In addition, the offset of the PST is -8
            tmpTimeEpoch = (time_t)timeEpoch - 8 * 3600;
            tm = localtime(&tmpTimeEpoch);
            break;

        case Time::TimeZone::NST:
            // The offset between the PST and UTC is 3.5 hours. In addition, the offset of the PST is -3 hours and 30 mins
            tmpTimeEpoch = (time_t)timeEpoch - 3 * 3600 - (30 * 60);
            tm = localtime(&tmpTimeEpoch);
            break;

        default:  // Same as the UTC/GMT
            tm = gmtime(&timeEpoch);
    }

    // Copying the time string into the buffer
    int length = (int)strlen(format) * 4;
    char buffer[length] = {'\0'};
    strftime(buffer, (size_t)length, format, tm);
    std::string timeString(buffer);
    *(Time::timeEpochPointer) = timeEpoch;

    return timeString;
}

}  // namespace Commons