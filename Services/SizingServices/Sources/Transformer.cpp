/**
 * @see Transformer.hpp
 */
#include "../Headers/Transformer.hpp"

namespace SizingServices {

/**
 * The initialization of the static variable
 */
int Transformer::defaultDescriptor = Commons::IOExporters::DescriptorType::STDOUT;

/**
 * Displaying the string with the specified string format and the arguments by using the specified descriptor
 *
 * @param descriptor [IN, const int, DescriptorType::STDOUT] The descriptor where users specify
 * @param stringFormat [IN, const unsigned char*] The format of the string modelled in C-style, such as "This is %d %s\n"
 * @param ... [IN] The arguments for the parameter, stringFormat
 * @return [OUT std::pair<Commons::POSIXErrors, long> ] Two types of result will be returned; the first one is the flag (defined in Commons::POSIXErrors) of the function;
 * the second one is the length of the processed string
 */
std::pair<Commons::POSIXErrors, long> Transformer::printContent(const int descriptor, const unsigned char* stringFormat, ...) {
    std::pair<Commons::POSIXErrors, long> returnedValue;

    va_list arguments;
    va_start(arguments, stringFormat);  // Stating from the location from the variable, descriptor defined in the parameter list
    returnedValue = Commons::IOExporters::printFromRegisteredDescriptor(descriptor, stringFormat, arguments);
    va_end(arguments);  // The obtained arguments' process end
    return returnedValue;
}

/**
 * Displaying the string with the specified string format and the arguments by using the specified descriptor (overloading)
 *
 * @param stringFormat [IN, const unsigned char*] The format of the string modelled in C-style, such as "This is %d %s\n"
 * @param ... [IN] The arguments for the parameter, stringFormat
 * @return [OUT std::pair<Commons::POSIXErrors, long> ] Two types of result will be returned; the first one is the flag (defined in Commons::POSIXErrors) of the function;
 * the second one is the length of the processed string
 */
std::pair<Commons::POSIXErrors, long> Transformer::printContent(const unsigned char* stringFormat, ...) {
    std::pair<Commons::POSIXErrors, long> returnedValue;

    va_list arguments;
    va_start(arguments, stringFormat);  // Stating from the location from the variable, descriptor defined in the parameter list
    returnedValue = Commons::IOExporters::printFromRegisteredDescriptor(Commons::IOExporters::DescriptorType::STDOUT, stringFormat, arguments);
    va_end(arguments);  // The obtained arguments' process end
    return returnedValue;
}

}