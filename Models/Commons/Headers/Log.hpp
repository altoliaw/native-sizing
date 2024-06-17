#pragma once
/** @file Log.hpp
 * Log information operation; the log is modeled as a hash table, the default key of the log
 * is default
 *
 * @author Nick, Liao
 * @date 2024/06/13
 */

#include <iostream>
#include <utility> // For the pair structure

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
    // The current working log name where the name is equal to the current log name of the element in the hash table
    

	static void getHashTableInitialization();
	static char setLogTableInformation(void*, size_t, HashTable::ElementType = HashTable::ElementType::charStarType, const char* = "default", char = 0x1);
	static void executeLog(std::ostream&, const char*, POSIXSysLog = POSIXSysLog::Debug, const char* = "default");
	static char getLogTableInformation(void**, size_t*, HashTable::ElementType*, const char* = "default");
    static void open(const char*);

   private:
    Log();
};

}  // namespace Commons