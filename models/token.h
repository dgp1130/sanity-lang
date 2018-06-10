#ifndef SANITY_TOKEN_H
#define SANITY_TOKEN_H

#include <string>

class Token {
public:
    const std::string source;

    explicit Token(const std::string& source) : source(source) { }

    friend std::ostream& operator<<(std::ostream&, const Token&);
};

#endif //SANITY_TOKEN_H
