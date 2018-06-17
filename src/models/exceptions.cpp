#include <cstring>
#include <sstream>
#include "exceptions.h"

typedef Exceptions::AssertionException AssertionException;
typedef Exceptions::FileNotFoundException FileNotFoundException;
typedef Exceptions::IllegalStateException IllegalStateException;
typedef Exceptions::ParseException ParseException;
typedef Exceptions::SyntaxException SyntaxException;
typedef Exceptions::TypeException TypeException;
typedef Exceptions::UndeclaredException UndeclaredException;

AssertionException::AssertionException(const std::string& reason) : reason(reason) { }

const char* AssertionException::what() const noexcept {
    return this->reason.c_str();
}

FileNotFoundException::FileNotFoundException(const std::string& filePath) : filePath(filePath) { }

const char* FileNotFoundException::what() const noexcept {
    const std::string message = std::string("File not found: ") + this->filePath;

    // Copy the string to heap memory so it can be returned.
    auto cstring = new char[message.size()];
    strcpy(cstring, message.c_str());

    return cstring;
}

IllegalStateException::IllegalStateException(const std::string& reason) : reason(reason) { }

const char* IllegalStateException::what() const noexcept {
    return this->reason.c_str();
}

ParseException::ParseException(const std::string& message): message(message) { }

const char* ParseException::what() const noexcept {
    return this->message.c_str();
}

SyntaxException::SyntaxException(const std::string& message, const int line, const int startCol, const int endCol)
    : message(message), line(line), startCol(startCol), endCol(endCol) { }

const char* SyntaxException::what() const noexcept {
    std::ostringstream ss;
    ss << "Syntax exception (line " << this->line << ", col " << this->startCol << " -> " << this->endCol << "): "
        << this->message;
    const std::string message = ss.str();
    auto cstring = new char[message.size()];
    strcpy(cstring, message.c_str());

    return cstring;
}

TypeException::TypeException(const std::string& message) : message(message) { }

const char* TypeException::what() const noexcept {
    return this->message.c_str();
}

UndeclaredException::UndeclaredException(const std::string& message) : message(message) { }

const char* UndeclaredException::what() const noexcept {
    return this->message.c_str();
}