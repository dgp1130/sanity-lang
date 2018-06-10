#ifndef SANITY_ERRORS_H
#define SANITY_ERRORS_H

#include <exception>
#include <string>

namespace Exceptions {
    /**
     * Exception for a file not being found.
     */
    struct FileNotFoundException : public std::exception {
        const std::string filePath;

        /**
         * @param filePath The path to the given file. This can be absolute or relative to any location, but should be
         *     as specific as reasonably possible.
         */
        explicit FileNotFoundException(const std::string& filePath) : filePath(filePath) { }

        const char* what() const noexcept override;
    };
}

#endif //SANITY_ERRORS_H