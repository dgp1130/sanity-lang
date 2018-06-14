#include <functional>
#include <memory>
#include <sstream>
#include "parser.h"
#include "../models/ast.h"
#include "../models/token.h"
#include "../models/exceptions.h"

typedef Exceptions::ParseException ParseException;

Parser::Parser(std::queue<std::shared_ptr<const Token>>& tokens) {
    this->tokens = tokens;
}

std::shared_ptr<const Token> Parser::match(const std::function<bool (std::shared_ptr<const Token>)>& matcher,
        const std::string& expected) {
    if (this->tokens.empty()) {
        throw ParseException("Expected \"" + expected + "\", but got EOF.");
    }

    std::shared_ptr<const Token> token = this->tokens.front();
    if (matcher(token)) {
        this->tokens.pop();
        return token;
    } else {
        std::ostringstream ss;
        ss << "Expected \"" << expected << "\", but got \"" << token->source << "\" (line " << token->line
           << ", col " << token->startCol << " -> " << token->endCol << ")";

        throw ParseException(ss.str());
    }
};

std::shared_ptr<const Token> Parser::match(const std::string& expected) {
    return this->match([&expected](std::shared_ptr<const Token> token) { return token->source == expected; }, expected);
};

// <file> => <block>
std::shared_ptr<const AST::Block> Parser::file() {
    return this->block();
}

// <block> -> <statement> <block> | ø
std::shared_ptr<const AST::Block> Parser::block() {
    std::vector<std::shared_ptr<const AST::Statement>> statements;
    while (!this->tokens.empty()) statements.push_back(this->statement());

    return std::make_shared<const AST::Block>(AST::Block(statements));
}

// <statement> -> <expression> ;
std::shared_ptr<const AST::Statement> Parser::statement() {
    std::shared_ptr<const AST::Expression> expr = this->expression();
    this->match(";");
    return std::make_shared<const AST::Statement>(AST::Statement(expr));
}

// <expression> -> <function-call> | <char-literal>
std::shared_ptr<const AST::Expression> Parser::expression() {
    if (this->tokens.empty()) throw ParseException("Expected an expression, but got EOF.");

    if (this->tokens.front()->isCharLiteral) {
        return this->charLiteral();
    } else {
        return this->functionCall();
    }
}

// <function-call> -> <identifier> ( <expression> )
std::shared_ptr<const AST::FunctionCall> Parser::functionCall() {
    std::shared_ptr<const AST::Identifier> callee = this->identifier();
    this->match("(");
    std::shared_ptr<const AST::Expression> argument = this->expression();
    this->match(")");

    return std::make_shared<AST::FunctionCall>(AST::FunctionCall(callee, argument));
}

std::shared_ptr<const AST::CharLiteral> Parser::charLiteral() {
    const std::shared_ptr<const Token> literal = this->match([](std::shared_ptr<const Token> token) {
        return token->isCharLiteral;
    }, "char literal");

    return std::make_shared<AST::CharLiteral>(AST::CharLiteral(literal));
}

std::shared_ptr<const AST::Identifier> Parser::identifier() {
    const std::shared_ptr<const Token> name = this->match([](std::shared_ptr<const Token> token) {
        return !token->isCharLiteral;
    }, "identifier");

    return std::make_shared<AST::Identifier>(AST::Identifier(name));
}

std::shared_ptr<const AST::Block> Parser::parse(std::queue<std::shared_ptr<const Token>>& tokens) {
    return Parser(tokens).file();
}