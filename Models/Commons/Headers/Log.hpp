#pragma once
/** @file Log.hpp
 * Log
 *
 * @author Nick, Liao
 * @date 2024/06/13
 */

#include <iostream>

#include "./POSIXErrors.hpp"
#include "./HashTable.hpp"
namespace Commons {
/**
 * A log class, the class is a static class and is modeled in a singleton
 */
class Log {
   public:
    // A log file will refer to a hash table because users can generate different logs flexibly.
    static HashTable* logTablePointer;
	static void getHashTableInitialization();
	static void setLogTableInformation(void*, size_t, HashTable::ElementType = HashTable::ElementType::charStarType, const char* = "default", char = 0x0);
	static void executeLog(std::ostream&, const char*, POSIXSysLog = POSIXSysLog::Debug, const char* = "default");
	static void getLogTableInformation(const char* = "default");
   private:
    Log();
};

}  // namespace Commons