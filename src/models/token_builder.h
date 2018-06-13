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
    bool isCharLiteral = false;

public:
    explicit TokenBuilder(const std::string& source);

    TokenBuilder charLiteral();

    std::shared_ptr<const Token> build();
};

#endif //SANITY_TOKEN_BUILDER_H