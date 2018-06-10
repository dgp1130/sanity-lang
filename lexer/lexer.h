#ifndef SANITY_LEXER_H
#define SANITY_LEXER_H

#include <queue>
#include "../models/token.h"

namespace Lexer {
    /**
     * Tokenize the given queue of characters into a queue of Tokens.
     */
    std::queue<const Token*> tokenize(std::queue<char>& chars);
}

#endif //SANITY_LEXER_H