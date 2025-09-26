#pragma once
/** @file Transformer.hpp
 * The data transformer for outputing with specified descriptor
 *
 * @author Nick, Liao
 * @date 2024/09/26
 */
#include "../../../Models/Commons/Headers/POSIXErrors.hpp"
#include "../../../Models/Commons/Headers/IOExporters.hpp"

namespace SizingServices {
/**
 * The data transformer for outputing with specified descriptor
 */
class Transformer {
   public:
    static int defaultDescriptor;  // The default descriptor

    static std::pair<Commons::POSIXErrors, long> printContent(const int, const unsigned char*, ...);
    static std::pair<Commons::POSIXErrors, long> printContent(const unsigned char*, ...);

    protected:
    Transformer() = delete;
    ~Transformer();
};

}  // namespace SizingServices