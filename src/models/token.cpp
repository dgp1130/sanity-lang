#include "token.h"

Token::Token(const std::string& source, const int line, const int startCol, const int endCol, const bool isCharLiteral)
    : source(source), line(line), startCol(startCol), endCol(endCol), isCharLiteral(isCharLiteral) { }

std::ostream& operator<<(std::ostream& stream, const Token& token) {
    stream << token.source;
    if (token.isCharLiteral) stream << std::string(" (char literal)");
    return stream;
}