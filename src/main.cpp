#include <iostream>
#include <memory>
#include <queue>
#include "utils/file_utils.h"
#include "lexer/lexer.h"
#include "models/ast.h"
#include "models/exceptions.h"
#include "parser/parser.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;
typedef Exceptions::ParseException ParseException;
typedef Exceptions::SyntaxException SyntaxException;

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
        const char* what = ex.what();
        std::cerr << what << std::endl;
        delete what;

        return 1;
    }

    // Tokenize the characters.
    std::queue<std::shared_ptr<const Token>> tokens;
    try {
        tokens = Lexer::tokenize(chars);
    } catch (const SyntaxException& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // Parse the tokens.
    std::shared_ptr<const AST::Block> block;
    try {
        block = Parser::parse(tokens);
    } catch (const ParseException& ex) {
        std::cerr << "ParseException: " << ex.what() << std::endl;
        return 1;
    }

    // Print the abstract syntax tree to stdout.
    block->print(std::cout);

    // Completed successfully.
    return 0;
}