#include "token_builder.h"

#include <memory>
#include "token.h"

TokenBuilder::TokenBuilder(const std::string& source) : source(source) {}

TokenBuilder TokenBuilder::charLiteral() {
    this->isCharLiteral = true;
    return *this;
}

std::shared_ptr<const Token> TokenBuilder::build() {
    return std::make_shared<const Token>(Token(this->source, this->isCharLiteral));
}