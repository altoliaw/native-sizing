#pragma once
/** @file POSIXError.hpp
 * The definition of the errors in the POSIX format
 *
 * @author Nick, Liao
 * @date 2024/05/15
 */
namespace Commons{

/**
 * The definition of the POSIX errors 
 */
enum POSIXErrors {
    OK = 0,
    E_ACCESS,   // No permission
    E_AGAIN,   // Resource cannot be accessed.
    E_BADF,    // Invalid descriptor
    E_NOMEM,   // No memory
    E_INVAL,   // Invalid parameter/argument
    E_NOSPC,   // No storage
    E_NOTDIR,  // The path is not a directory
    E_NOENT,   // The path does not exist
    E_EXIST,   // The path exists
    E_INTR,    // Operations suspended/interrupted
    E_IO,      // IO error
    E_NOTTY,   // Not a teletypewriter (tty)
    
    // Custom definition
    E_NOITEM,  // No item
};

}