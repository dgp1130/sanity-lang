#include "token.h"

Token::Token(const std::string& source, const bool isCharLiteral) : source(source), isCharLiteral(isCharLiteral) { }

std::ostream& operator<<(std::ostream& stream, const Token& token) {
    stream << token.source;
    if (token.isCharLiteral) stream << std::string(" (char literal)");
    return stream;
}