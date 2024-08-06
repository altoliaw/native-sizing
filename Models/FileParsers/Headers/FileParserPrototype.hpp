#pragma once
/** @file FileInputsPrototype.hpp
 * The abstract class of FileParser series
 *
 * @author Nick, Liao
 * @date 2024/08/03
 */
#include <string.h>

namespace FileParsers {
class FileParserPrototype {
   public:
	FileParsers();
	~FileParsers();
    virtual void readFileContent2HashTable() = 0;
};

}  // namespace FileInputs