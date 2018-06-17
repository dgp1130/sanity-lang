#ifndef SANITY_PARSER_H
#define SANITY_PARSER_H

#include <queue>
#include <functional>
#include "../models/ast.h"
#include "../models/token.h"

/**
 * Class for parsing the Sanity language. Uses a recursive descent parsing design. The class is only used via the static
 * parse() function, however the class is necessary to maintain state so that the match() methods are usuable.
 */
class Parser {
private:
    std::queue<std::shared_ptr<const Token>> tokens;

    explicit Parser(std::queue<std::shared_ptr<const Token>>& tokens);

    std::shared_ptr<const Token> match(const std::function<bool (std::shared_ptr<const Token>)>& matcher,
        const std::string& errMsg);
    std::shared_ptr<const Token> match(const std::string& expected);

    std::shared_ptr<const AST::File> file();
    std::shared_ptr<const AST::FunctionPrototype> externDecl();
    std::shared_ptr<const AST::Statement> statement();
    std::shared_ptr<const AST::Expression> expression();
    std::shared_ptr<const AST::FunctionCall> functionCall();
    std::shared_ptr<const AST::CharLiteral> charLiteral();

public:
    /**
     * Parse the tokens provided.
     */
    static std::shared_ptr<const AST::File> parse(std::queue<std::shared_ptr<const Token>>& tokens);
};

#endif //SANITY_PARSER_H