#include <iostream>
#include <vector>
#include "ast.h"
#include "token.h"

AST::Statement::Statement(std::shared_ptr<const AST::Expression> expr) : expr(std::move(expr)) { }

void AST::Statement::print(std::ostream& stream) const {
    this->expr->print(stream);
    stream << ";";
}

AST::Block::Block(std::vector<std::shared_ptr<const AST::Statement>> statements) : statements(std::move(statements)) { }

void AST::Block::print(std::ostream& stream) const {
    for (const auto& stmt : this->statements) {
        stmt->print(stream);
        stream << std::endl;
    }
}

AST::Identifier::Identifier(const std::shared_ptr<const Token> name) : name(name->source) { }

void AST::Identifier::print(std::ostream& stream) const {
    stream << this->name;
}

AST::CharLiteral::CharLiteral(const std::shared_ptr<const Token> value) : value(value->source[0]) { }

void AST::CharLiteral::print(std::ostream& stream) const {
    stream << "\'" << this->value << "\'";
}

AST::FunctionCall::FunctionCall(std::shared_ptr<const AST::Identifier> callee,
        std::shared_ptr<const AST::Expression> argument)
    : callee(std::move(callee)), argument(std::move(argument)) { }

void AST::FunctionCall::print(std::ostream& stream) const {
    this->callee->print(stream);
    stream << "(";
    this->argument->print(stream);
    stream << ")";
}