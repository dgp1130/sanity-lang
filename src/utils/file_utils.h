#ifndef SANITY_FILEUTILS_H
#define SANITY_FILEUTILS_H

#include <fstream>
#include <queue>
#include "../models/exceptions.h"

namespace FileUtils {
    /**
     * Opens the file with the given name and reads it character-by-character, putting each character into a queue.
     * Includes all forms of whitespace.
     * @throws FileNotFoundException
     */
    std::queue<char> readFileChars(const std::string& filename);
}

#endif //SANITY_FILEUTILS_H