#pragma once
/** @file IOSpecification.hpp
 * IO specifications
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */

#include <unistd.h>
#include <iostream>

#include "./POSIXErrors.hpp"

namespace Commons {
/**
 * The IO specification; the object of the class is a singleton
 */
class IOSpecification {
   public:
    // For recording the current file descriptor
    int current;

    // For the output file descriptor
    int fileDescriptor;

    ~IOSpecification();
    static IOSpecification* getSingleton();
    int migrateOutput(int, int);
    int recoverOutput(int);

   private:
    IOSpecification();
};

}  // namespace Commons