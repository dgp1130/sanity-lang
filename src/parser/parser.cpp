#include <functional>
#include <memory>
#include <sstream>
#include "parser.h"
#include "../models/ast.h"
#include "../models/token.h"
#include "../models/exceptions.h"
#include "../models/globals.h"
#include "llvm/IR/Type.h"

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

// <file> -> <externDecl> <block> | <statement> <block> | ø
std::shared_ptr<const AST::File> Parser::file() {
    std::vector<std::shared_ptr<const AST::Function>> externDecls;
    std::vector<std::shared_ptr<const AST::Statement>> statements;

    while (!this->tokens.empty()) {
        if (this->tokens.front()->source == "extern") {
            externDecls.push_back(this->externDecl());
        } else {
            statements.push_back(this->statement());
        }
    }

    return std::make_shared<const AST::File>(AST::File(externDecls, statements));
}

// <externDecl> -> extern <name>: <func-type> ;
std::shared_ptr<const AST::Function> Parser::externDecl() {
    this->match("extern");
    std::shared_ptr<const Token> name = this->match([](std::shared_ptr<const Token> token) {
        return !token->isCharLiteral;
    }, "extern");
    this->match(":");
    std::shared_ptr<const AST::FunctionPrototype> type = this->funcType();
    this->match(";");

    return std::make_shared<const AST::Function>(AST::Function(name->source, type));
}

// <statement> -> <expression> ;
std::shared_ptr<const AST::Statement> Parser::statement() {
    std::shared_ptr<const AST::Expression> expr = this->expression();
    this->match(";");
    return std::make_shared<const AST::Statement>(AST::Statement(expr));
}

// <type> -> int | <func-type>
std::shared_ptr<const AST::Type> Parser::type() {
    if (this->tokens.empty()) throw ParseException("Expected a type, but got EOF.");

    if (this->tokens.front()->source == "int") {
        this->match("int");
        return std::make_shared<AST::IntegerType>(AST::IntegerType());
    } else if (this->tokens.front()->source == "(") {
        return this->funcType();
    } else {
        throw ParseException("Expected a type, but got \"" + this->tokens.front()->source + "\"");
    }
}

// <func-type> -> ( <type> <types> ) -> <type>
// <func-type> -> ( ) -> <type>
// <types> -> , <type> | ø
std::shared_ptr<const AST::FunctionPrototype> Parser::funcType() {
    std::vector<std::shared_ptr<const AST::Type>> parameters;

    this->match("(");
    if (this->tokens.front()->source != ")") { // Has parameters
        parameters.push_back(this->type());
        while (this->tokens.front()->source == ",") {
            this->match(",");
            parameters.push_back(this->type());
        }
    }
    this->match(")");

    this->match("->");
    std::shared_ptr<const AST::Type> returnType = this->type();

    return std::make_shared<AST::FunctionPrototype>(AST::FunctionPrototype(parameters, returnType));
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

// <function-call> -> <name> ( <expression> )
std::shared_ptr<const AST::FunctionCall> Parser::functionCall() {
    std::shared_ptr<const Token> callee = this->match([](std::shared_ptr<const Token> token) {
        return !token->isCharLiteral;
    }, "function name");;
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

std::shared_ptr<const AST::File> Parser::parse(std::queue<std::shared_ptr<const Token>>& tokens) {
    return Parser(tokens).file();
}