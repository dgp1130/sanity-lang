#ifndef SANITY_LEXER_H
#define SANITY_LEXER_H

#include <memory>
#include <queue>
#include "../models/token.h"

namespace Lexer {
    /**
     * Tokenize the given queue of characters into a queue of Tokens.
     */
    std::queue<std::shared_ptr<const Token>> tokenize(std::queue<char>& chars);
}

#endif //SANITY_LEXER_H