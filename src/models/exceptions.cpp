#include <cstring>
#include <sstream>
#include "exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;
typedef Exceptions::IllegalStateException IllegalStateException;
typedef Exceptions::ParseException ParseException;
typedef Exceptions::SyntaxException SyntaxException;

const char* FileNotFoundException::what() const noexcept {
    const std::string message = std::string("File not found: ") + this->filePath;

    // Copy the string to heap memory so it can be returned.
    auto cstring = new char[message.size()];
    strcpy(cstring, message.c_str());

    return cstring;
}

const char* IllegalStateException::what() const noexcept {
    return this->reason.c_str();
}

const char* ParseException::what() const noexcept {
    return this->message.c_str();
}

const char* SyntaxException::what() const noexcept {
    std::ostringstream ss;
    ss << "Syntax exception (line " << this->line << ", col " << this->startCol << " -> " << this->endCol << "): "
        << this->message;
    const std::string message = ss.str();
    auto cstring = new char[message.size()];
    strcpy(cstring, message.c_str());

    return cstring;
}