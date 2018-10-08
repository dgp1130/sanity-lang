#ifndef SANITY_ERRORS_H
#define SANITY_ERRORS_H

#include <exception>
#include <string>

namespace Exceptions {
    /**
     * Exception for a failed assertion.
     */
    struct AssertionException : public std::exception {
        const std::string reason;

        explicit AssertionException(const std::string& reason);

        const char* what() const noexcept override;
    };

    /**
     * Exception for a file not being found.
     */
    struct FileNotFoundException : public std::exception {
        const std::string message;

        /**
         * @param filePath The path to the given file. This can be absolute or relative to any location, but should be
         *     as specific as reasonably possible.
         */
        explicit FileNotFoundException(const std::string& filePath);

        const char* what() const noexcept override;
    };

    /**
     * Exception to throw when an illegal state is detected which makes it impossible to continue.
     */
    struct IllegalStateException : public std::exception {
        const std::string reason;

        explicit IllegalStateException(const std::string& reason = "IllegalStateException");

        const char* what() const noexcept override;
    };

    /**
     * Exception to throw on a parsing error.
     */
    struct ParseException : public std::exception {
        const std::string message;

        explicit ParseException(const std::string& message);

        const char* what() const noexcept override;
    };

    /**
     * Exception to throw on a syntax error encountered during lexical analysis.
     */
    struct SyntaxException : public std::exception {
        std::string message;

        SyntaxException(const std::string& message, int line, int startCol, int endCol);

        const char* what() const noexcept override;
    };

    /**
     * Exception to throw when there is a type mismatch.
     */
    struct TypeException : public std::exception {
        const std::string message;

        explicit TypeException(const std::string& message);

        const char* what() const noexcept override;
    };

    /**
     * Exception to throw when accessing an undeclared symbol.
     */
    struct UndeclaredException : public std::exception {
        const std::string message;

        UndeclaredException(const std::string& message);

        const char* what() const noexcept override;
    };
}

#endif //SANITY_ERRORS_H