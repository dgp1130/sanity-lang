#ifndef SANITY_PARSER_H
#define SANITY_PARSER_H

#include <queue>
#include <functional>
#include "../models/token.h"

/**
 * Class for parsing the Sanity language. Uses a recursive descent parsing design.
 */
class Parser {
private:
    std::queue<const Token*> tokens;

    const Token* match(const std::function<bool (const Token*)>& matcher, const std::string& errMsg);
    const Token* match(const std::string& expected);

    void file();
    void statement();
    void functionCall();
    void expression();
    void literal();
    void identifier();

public:
    /**
     * Construct a parser which will parse the provided tokens and consume the queue.
     */
    explicit Parser(std::queue<const Token*>& tokens);

    /**
     * Parse the tokens provided.
     */
    void parse();
};

#endif //SANITY_PARSER_H