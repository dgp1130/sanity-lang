#include <iostream>
#include <queue>
#include "utils/file_utils.h"
#include "lexer/lexer.h"
#include "models/exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;

int main(const int argc, const char* argv[]) {
    // Validate arguments.
    if (argc < 2) {
        std::cerr << "Please provide a Sanity source file to compile." << std::endl;
        return 1;
    }

    // Read file into a queue of characters.
    std::queue<char> chars;
    try {
        chars = FileUtils::readFileChars(argv[1]);
    } catch (const FileNotFoundException& ex) {
        // Log error and exit.
        const auto what = ex.what();
        std::cerr << what << std::endl;
        delete what;

        return 1;
    }

    // Tokenize the characters.
    std::queue<Token> tokens = Lexer::tokenize(chars);

    // Print tokens for now.
    while (!tokens.empty()) {
        std::cout << tokens.front();
        tokens.pop();
    }
    std::cout << std::endl;

    // Completed successfully.
    return 0;
}