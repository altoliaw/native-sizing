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
 * @param direction [const char] The direction for searching the string; 0x0 is from the left-hand
 * side and the 0x1 is the right-hand side (the default is 0)
 * @return [unsigned int] The index; if there is no character in the string, the value is equal to -1;
 * if the value is equal to -2, the error occurs
 */
int StringImplment::obtainFirstNotSpaceCharLocation(const char* string, const char direction) {
    int index = -2;  // Final result
    if(string != nullptr) {
        unsigned int stringLength = strlen(string);
        if (string != nullptr) {
            index = -1;
            if (direction == 0x0) {
                for (unsigned int i = 0; i < stringLength; i++) {
                    if (string[i] != ' ') {
                        index = (int)i;
                        break;
                    }
                }
            } else if (direction == 0x1) {
                for (unsigned int i = 0; i < stringLength; i++) {
                    if (string[stringLength - 1 - i] != ' ') {
                        index = (int)(stringLength - 1 - i);
                        break;
                    }
                }
            }
        }
    }
    return index;
}

/**
 *  Obtaining the string with no space(s) from the input string by
 *  trimming spaces from the left-hand side before the first non-space character
 *  of the input string
 *
 * @param string [char**] The address of the pointer of the string
 * @return [int] The length after trimming; if the value is equal to -1, the error occurs
 */
int StringImplment::trimLeftSpace(char** string) {
    int length = -1;
    // Searching from the left-hand side
    int index = StringImplment::obtainFirstNotSpaceCharLocation((const char*)*string);
    if (index >= -1 && index <= 0) {
        length = (int)strlen(*string);
    } else if (index > 0) {
        length = (int)strlen(*string) - index;
        memmove(*string, (*string) + index, length);
        (*string)[length] = '\0';
    }
    return length;
}

/**
 *  Obtaining the string with no space(s) from the input string by
 *  trimming spaces from the right-hand side before the first non-space character
 *  of the input string
 *
 * @param string [char**] The address of the pointer of the string
 * @return [int] The length after trimming; if the value is equal to -1, the error occurs
 */
int StringImplment::trimRightSpace(char** string) {
    int length = -1;
    // Searching from the right-hand side
    int index = StringImplment::obtainFirstNotSpaceCharLocation((const char*)*string, (char)0x1);
    if (index >= -1 && index <= 0) {
        length = (int)strlen(*string);
    } else if (index > 0) {
        length = index + 1;
        (*string)[length] = '\0';
    }
    return length;
}

/**
 *  Obtaining the string between the frist character of the input string and the last character
 *  of the input string where all terms will be seperated by a space at least;
 *  this function must be implemented after StringImplment::trimLeftSpace and
 *  StringImplment::trimRightSpace
 *
 * @param string [char**] The address of the pointer of the string
 * @return [int] The length after trimming; if the value is equal to -1, the error occurs
 */
int StringImplment::mergeRedundantSpace(char** string) {
    int length = -1;
    // Using the shifted concept which the string comes from the two substrings
    if (*string != nullptr) {
        int fianlLength = (int)strlen(*string);
        int checkedIndex = 0;
        for (unsigned int i = 0, j = 0; i < (unsigned int)fianlLength; i++) {
            // When the pivot is a space and the previous checked character is a space as well,
            // the pivot shall be moved to the next one, and the checked character will be stayed in the
            // current index.
            j = ((checkedIndex == 0) ? 0 : (checkedIndex - 1));
            if ((*string)[i] == ' ' && (*string)[j] == ' ') {
                continue;
            }
            (*string)[j] = (*string)[i];
            checkedIndex++;
        }
        (*string)[checkedIndex] = '\0';
        length = (int)strlen(*string);
    }
    return length;
}

};  // namespace Commons