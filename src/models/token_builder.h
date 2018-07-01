#ifndef SANITY_TOKEN_BUILDER_H
#define SANITY_TOKEN_BUILDER_H

#include "token.h"
#include <memory>
#include <string>

/**
 * Builder class for Token. Call #build() to actually create the Token based on the options set on this builder.
 * @see Token
 */
class TokenBuilder {
private:
    const std::string source;
    int line = -1;
    int startCol = -1;
    int endCol = -1;
    bool isCharLiteral = false;
    bool isIntegerLiteral = false;

public:
    explicit TokenBuilder(const std::string& source);

    TokenBuilder setLine(int line);

    TokenBuilder setStartCol(int col);

    TokenBuilder setEndCol(int col);

    TokenBuilder setCharLiteral(bool isCharLiteral);

    TokenBuilder setIntegerLiteral(bool isIntegerLiteral);

    std::shared_ptr<const Token> build();
};

#endif //SANITY_TOKEN_BUILDER_H