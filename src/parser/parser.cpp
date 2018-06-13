#include <functional>
#include "parser.h"
#include "../models/token.h"
#include "../models/exceptions.h"

typedef Exceptions::ParseException ParseException;

Parser::Parser(std::queue<const Token*>& tokens) {
    this->tokens = tokens;
}

const Token* Parser::match(const std::function<bool (const Token*)>& matcher, const std::string& expected) {
    if (this->tokens.empty()) {
        throw ParseException("Expected \"" + expected + "\", but got EOF.");
    }

    const Token* token = this->tokens.front();
    if (matcher(token)) {
        this->tokens.pop();
        return token;
    } else {
        throw ParseException("Expected \"" + expected + "\", but got \"" + token->source + "\"");
    }
};

const Token* Parser::match(const std::string& expected) {
    return this->match([&expected](const Token* token) { return token->source == expected; }, expected);
};

// <file> => <statement> <file> | ø
void Parser::file() {
    while (!this->tokens.empty()) this->statement();
}

// <statement> -> <function-call> ;
void Parser::statement() {
    this->functionCall();
    delete this->match(";");
}

// <function-call> -> <identifier> ( <expression> )
void Parser::functionCall() {
    this->identifier();
    delete this->match("(");
    this->expression();
    delete this->match(")");
}

// <expression> -> <char-literal>
void Parser::expression() {
    this->literal();
}

void Parser::literal() {
    delete this->match([](const Token* token) {
        return token->isCharLiteral;
    }, "char literal");
}

void Parser::identifier() {
    delete this->match([](const Token* token) {
        return !token->isCharLiteral;
    }, "identifier");
}

void Parser::parse() {
    this->file();
}