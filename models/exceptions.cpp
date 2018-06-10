#include <cstring>
#include "exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;

const char* FileNotFoundException::what() const noexcept {
    const std::string message = std::string("File not found: ") + this->filePath;

    // Copy the string to heap memory so it can be returned.
    auto cstring = new char[message.size()];
    strcpy(cstring, message.c_str());

    return cstring;
}