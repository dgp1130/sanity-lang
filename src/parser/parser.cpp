#include <functional>
#include <memory>
#include <sstream>
#include "parser.h"
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

// <file> => <statement> <file> | ø
void Parser::file() {
    while (!this->tokens.empty()) this->statement();
}

// <statement> -> <function-call> ;
void Parser::statement() {
    this->functionCall();
    this->match(";");
}

// <function-call> -> <identifier> ( <expression> )
void Parser::functionCall() {
    this->identifier();
    this->match("(");
    this->expression();
    this->match(")");
}

// <expression> -> <char-literal>
void Parser::expression() {
    this->literal();
}

void Parser::literal() {
    this->match([](std::shared_ptr<const Token> token) {
        return token->isCharLiteral;
    }, "char literal");
}

void Parser::identifier() {
    this->match([](std::shared_ptr<const Token> token) {
        return !token->isCharLiteral;
    }, "identifier");
}

void Parser::parse() {
    this->file();
}