#pragma once
/** @file StringImplement.hpp
 * IO specifications
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#include <iostream>
#include <string.h>
#include <string>

#include "./POSIXErrors.hpp"

namespace Commons {
/**
 * Serveral string implementation
 */
class StringImplement {
   public:
    static int obtainFirstNotSpaceCharLocation(const char*, const char = 0x0);
	static int trimLeftSpace(char**);
    static int trimRightSpace(char**);
	static int mergeRedundantSpace(char**);
};

}  // namespace Commons