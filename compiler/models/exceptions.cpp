#include <cstring>
#include <sstream>
#include "exceptions.h"

typedef Exceptions::AssertionException AssertionException;
typedef Exceptions::FileNotFoundException FileNotFoundException;
typedef Exceptions::IllegalStateException IllegalStateException;
typedef Exceptions::ParseException ParseException;
typedef Exceptions::RedeclaredException RedeclaredException;
typedef Exceptions::SyntaxException SyntaxException;
typedef Exceptions::TypeException TypeException;
typedef Exceptions::UndeclaredException UndeclaredException;

AssertionException::AssertionException(const std::string& reason) : reason(reason) { }

const char* AssertionException::what() const noexcept {
    return this->reason.c_str();
}

FileNotFoundException::FileNotFoundException(const std::string& filePath)
        : message(std::string("File not found: ") + filePath) { }

const char* FileNotFoundException::what() const noexcept {
    return this->message.c_str();
}

IllegalStateException::IllegalStateException(const std::string& reason) : reason(reason) { }

const char* IllegalStateException::what() const noexcept {
    return this->reason.c_str();
}

ParseException::ParseException(const std::string& message): message(message) { }

const char* ParseException::what() const noexcept {
    return this->message.c_str();
}

RedeclaredException::RedeclaredException(const std::string& message): message(message) { }

const char* RedeclaredException::what() const noexcept {
    return this->message.c_str();
}

SyntaxException::SyntaxException(const std::string& message, const int line, const int startCol, const int endCol) {
    std::ostringstream ss;
    ss << "Syntax exception (line " << line << ", col " << startCol << " -> " << endCol << "): " << message;
    this->message = ss.str();
}

const char* SyntaxException::what() const noexcept {
    return this->message.c_str();
}

TypeException::TypeException(const std::string& message) : message(message) { }

const char* TypeException::what() const noexcept {
    return this->message.c_str();
}

UndeclaredException::UndeclaredException(const std::string& message) : message(message) { }

const char* UndeclaredException::what() const noexcept {
    return this->message.c_str();
}