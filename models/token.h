#ifndef SANITY_TOKEN_H
#define SANITY_TOKEN_H

#include <string>

class Token {
private:
    const std::string source;

    friend std::ostream& operator<<(std::ostream&, const Token&);

public:
    explicit Token(const std::string& source) : source(source) { }
};

#endif //SANITY_TOKEN_H
