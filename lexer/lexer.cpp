#include "lexer.h"

std::queue<Token> Lexer::tokenize(std::queue<char>& chars) {
    auto tokens = std::queue<Token>();

    // Copy each character into its own token for now.
    while (!chars.empty()) {
        const char c = chars.front();
        tokens.push(Token(std::string(1, c)));
        chars.pop();
    }

    return tokens;
}