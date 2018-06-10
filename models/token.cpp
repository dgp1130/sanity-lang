#include "token.h"

std::ostream& operator<<(std::ostream& stream, const Token& token) {
    return stream << token.source;
}