#pragma once
/** @file MainHeaders.hpp
 * The headers and global variables from other package
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#include <unistd.h>
#include <signal.h>
#include <thread>
#include <time.h>

#include "../Models/Commons/Headers/IOSpecification.hpp"
#include "../Models/Commons/Headers/POSIXErrors.hpp"
#include "../Models/PCAP/Headers/LinuxPCAP.hpp"


//===Function Declaration===

int start(int, char**);
void signalInterruptedHandler(int);
void signalAlarmHandler(int);