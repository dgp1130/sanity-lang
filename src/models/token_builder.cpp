#include "token_builder.h"

#include <memory>
#include "token.h"

TokenBuilder::TokenBuilder(const std::string& source) : source(source) {}

TokenBuilder TokenBuilder::setLine(const int line) {
    this->line = line;
    return *this;
}

TokenBuilder TokenBuilder::setStartCol(const int col) {
    this->startCol = col;
    return *this;
}

TokenBuilder TokenBuilder::setEndCol(const int col) {
    this->endCol = col;
    return *this;
}

TokenBuilder TokenBuilder::setCharLiteral(bool isCharLiteral){
    this->isCharLiteral = isCharLiteral;
    return *this;
}

TokenBuilder TokenBuilder::setIntegerLiteral(bool isIntegerLiteral) {
    this->isIntegerLiteral = isIntegerLiteral;
    return *this;
}

std::shared_ptr<const Token> TokenBuilder::build() {
    return std::make_shared<const Token>(Token(this->source, this->line, this->startCol, this->endCol,
        this->isCharLiteral, this->isIntegerLiteral));
}