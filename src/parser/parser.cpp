#include <functional>
#include <memory>
#include <sstream>
#include "parser.h"
#include "../models/ast.h"
#include "../models/token.h"
#include "../models/exceptions.h"
#include "../models/globals.h"
#include "llvm/IR/Type.h"

typedef Exceptions::AssertionException AssertionException;
typedef Exceptions::ParseException ParseException;

Parser::Parser(std::queue<std::shared_ptr<const Token>>& tokens) {
    this->tokens = tokens;
}

std::shared_ptr<const Token> Parser::match(const std::function<bool (std::shared_ptr<const Token>&)>& matcher,
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
    return this->match([&expected](std::shared_ptr<const Token>& token) { return token->source == expected; }, expected);
};

std::shared_ptr<const Token> Parser::match() {
    return this->match([](std::shared_ptr<const Token>& token) { return true; }, "ShouldNeverPrint");
}

// <file> ::= <externDecl> <block>
//          | <statement> <block>
//          | ø
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

// <externDecl> ::= extern <name>: <func-type> ;
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

// <statement> ::= <expression> ;
std::shared_ptr<const AST::Statement> Parser::statement() {
    std::shared_ptr<const AST::Expression> expr = this->expression();
    this->match(";");
    return std::make_shared<const AST::Statement>(AST::Statement(expr));
}

// <type> ::= int
//          | <func-type>
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

// <func-type> ::= ( <types> ) -> <type>
// <types> ::= <type> <types'>
//           | ø
// <types'> ::= , <type>
//            | ø
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

// <expression> ::= <expr-add-sub>
std::shared_ptr<const AST::Expression> Parser::expression() {
    return this->exprAddSub();
}

// <expr-add-sub> ::= <expr-leaf> <expr-add-sub'>
// <expr-add-sub'> ::= + <expr-mul-div> <expr-add-sub'>
//                   | - <expr-mul-div> <expr-add-sub'>
//                   | ø
std::shared_ptr<const AST::Expression> Parser::exprAddSub() {
    std::shared_ptr<const AST::Expression> leftExpr = this->exprMulDiv();

    while (!this->tokens.empty() && (this->tokens.front()->source == "+" || this->tokens.front()->source == "-")) {
        const std::shared_ptr<const Token> op = this->match();
        const std::shared_ptr<const AST::Expression> rightExpr = this->exprMulDiv();

        if (op->source == "+") {
            leftExpr = std::make_shared<const AST::AddOpExpression>(AST::AddOpExpression(leftExpr, rightExpr));
        } else if (op->source == "-") {
            leftExpr = std::make_shared<const AST::SubOpExpression>(AST::SubOpExpression(leftExpr, rightExpr));
        } else {
            throw AssertionException("Expected operator + or -, but got " + op->source);
        }
    }

    return leftExpr;
}

// <expr-mul-div> ::= <expr-paren> <expr-mul-div'>
// <expr-mul-div'> ::= * <expr-paren> <expr-mul-div'>
//                   | / <expr-paren> <expr-mul-div'>
//                   | ø
std::shared_ptr<const AST::Expression> Parser::exprMulDiv() {
    std::shared_ptr<const AST::Expression> leftExpr = this->exprParen();

    while (!this->tokens.empty() && (this->tokens.front()->source == "*" || this->tokens.front()->source == "/")) {
        const std::shared_ptr<const Token> op = this->match();
        const std::shared_ptr<const AST::Expression> rightExpr = this->exprParen();

        if (op->source == "*") {
            leftExpr = std::make_shared<const AST::MulOpExpression>(AST::MulOpExpression(leftExpr, rightExpr));
        } else if (op->source == "/") {
            leftExpr = std::make_shared<const AST::DivOpExpression>(AST::DivOpExpression(leftExpr, rightExpr));
        } else {
            throw AssertionException("Expected operator * or /, but got " + op->source);
        }
    }

    return leftExpr;
}

// <expr-paren> ::= ( <expression> )
//                | <expr-leaf>
std::shared_ptr<const AST::Expression> Parser::exprParen() {
    if (!this->tokens.empty() && this->tokens.front()->source == "(") {
        this->match(); // (
        const std::shared_ptr<const AST::Expression> expr = this->expression();
        this->match(")");
        return expr;
    } else {
        return this->exprLeaf();
    }
}

// <expr-leaf> ::= <function-call>
//               | <char-literal>
//               | <integer-literal>
std::shared_ptr<const AST::Expression> Parser::exprLeaf() {
    if (this->tokens.empty()) throw ParseException("Expected an expression, but got EOF.");

    if (this->tokens.front()->isCharLiteral) {
        return this->charLiteral();
    } else if (this->tokens.front()->isIntegerLiteral) {
        return this->integerLiteral();
    } else {
        return this->functionCall();
    }
}

// <function-call> ::= <name> ( <arguments> )
// <arguments> ::= <argument> <arguments'>
//               | ø
// <arguments'> ::= , <argument>
//                | ø
std::shared_ptr<const AST::FunctionCall> Parser::functionCall() {
    std::shared_ptr<const Token> callee = this->match([](std::shared_ptr<const Token> token) {
        return !token->isCharLiteral;
    }, "function name");;
    this->match("(");

    // Parse arguments
    std::vector<std::shared_ptr<const AST::Expression>> arguments;
    if (this->tokens.front()->source != ")") {
        arguments.push_back(this->expression());
        while (this->tokens.front()->source == ",") {
            this->match(",");
            arguments.push_back(this->expression());
        }
    }

    this->match(")");

    return std::make_shared<const AST::FunctionCall>(AST::FunctionCall(callee, arguments));
}

std::shared_ptr<const AST::CharLiteral> Parser::charLiteral() {
    const std::shared_ptr<const Token> literal = this->match([](std::shared_ptr<const Token> token) {
        return token->isCharLiteral;
    }, "char literal");

    return std::make_shared<const AST::CharLiteral>(AST::CharLiteral(literal));
}

std::shared_ptr<const AST::IntegerLiteral> Parser::integerLiteral() {
    const std::shared_ptr<const Token> literal = this->match([](std::shared_ptr<const Token> token) {
        return token->isIntegerLiteral;
    }, "integer literal");

    return std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(literal));
}

std::shared_ptr<const AST::File> Parser::parse(std::queue<std::shared_ptr<const Token>>& tokens) {
    return Parser(tokens).file();
}