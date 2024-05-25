/**
 * @see StringImplment.hpp
 */
#include "../Headers/StringImplment.hpp"

namespace Commons {
/**
 * Obtaining the index of the first character from the string where the character is not a space;
 * the function may cause an error when the string is null/nullptr
 *
 * @param string [const char*] The input string
 * @return [unsigned int] The index; if there is no character in the string, the value is equal to -1;
 * if the value is equal to -2, the error occurs
 */
int StringImplment::obtainFirstNotSpaceCharLocation(const char* string) {
    int index = -2;      // Final result
    int tempIndex = -1;  // For recording the result from the loop
    if (string != nullptr) {
        for (unsigned int i = 0; i < strlen(string); i++) {
            if (string[i] != ' ') {
                tempIndex = (int)i;
                break;
            }
        }

        if (tempIndex == -1) {
            index = tempIndex;
        }
    }
    return index;
}

/**
 *  Obtaining the string with no space(s) from the input string by
 *  trimming spaces before the first non-space character of the input string
 *
 * @param string [char**] The address of the pointer of the string
 * @return [int] The length after trimming; if the value is equal to -1, the error occurs
 */
int StringImplment::trimLeftSpace(char** string) {
    int index = -2;
    int length = -1;
    index = StringImplment::obtainFirstNotSpaceCharLocation((const char*)*string);
    if (index >= -1 && index <= 0) {
        length = (int)strlen(*string);
    } else if (index > 0) {
        length = (int)strlen(*string) - index + 1;
        memmove(*string, (*string) + index, length);
        (*string)[length] = '\0';
    }
    return length;
}

int StringImplment::trimRightSpace(char**) {
}

int StringImplment::mergeRedundantSpace(char**) {
}

};  // namespace Commons