#include "token_builder.h"

#include "token.h"

TokenBuilder::TokenBuilder(const std::string& source) : source(source) {}

TokenBuilder TokenBuilder::charLiteral() {
    this->isCharLiteral = true;
    return *this;
}

const Token* TokenBuilder::build() {
    return new Token(this->source, this->isCharLiteral);
}