#include <iostream>
#include "file_utils.h"
#include "../models/exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;

std::queue<char> FileUtils::readFileChars(const std::string& filename) {
    // Open the file to read.
    auto fileStream = std::ifstream(filename);

    // Verify that file was opened successfully.
    if (!fileStream) {
        throw FileNotFoundException(filename);
    }

    fileStream >> std::noskipws; // Don't skip whitespace.


    // Create a queue and add each character to it.
    std::queue<char> chars = std::queue<char>();
    char c;
    while ((c = (char) fileStream.get()) != EOF) {
        chars.push(c);
    }

    fileStream.close();

    return chars;
}