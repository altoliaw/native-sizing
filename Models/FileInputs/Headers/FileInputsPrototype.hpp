#pragma once
/** @file FileInputsPrototype.hpp
 * The abstract class of FileInputs
 *
 * @author Nick, Liao
 * @date 2024/05/23
 */
#include <string.h>

namespace FileInputs {
class FileInputsPrototype {
   public:

	FileInputsPrototype();
	~FileInputsPrototype();
    virtual void readFileContent2HashTable() = 0;
};

}  // namespace FileInputs