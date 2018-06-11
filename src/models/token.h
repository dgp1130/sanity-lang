#ifndef SANITY_TOKEN_H
#define SANITY_TOKEN_H

#include <string>

class Token {
public:
    const std::string source;
    const bool isCharLiteral;

    /**
     * Construct a Token with the given values. This API kind of sucks, so use TokenBuilder instead.
     * @see TokenBuilder
     */
    explicit Token(const std::string& source, bool isCharLiteral);

    friend std::ostream& operator<<(std::ostream&, const Token&);
};

#endif //SANITY_TOKEN_H
