#include <vector>
#include "ast.h"
#include "token.h"
#include "../generator/generator.h"
#include "llvm/Support/raw_ostream.h"

AST::BinaryOpExpression::BinaryOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
    : leftExpr(std::move(leftExpr)), rightExpr(std::move(rightExpr)) { }

AST::AddOpExpression::AddOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
    : BinaryOpExpression(std::move(leftExpr), std::move(rightExpr)) { }

void AST::AddOpExpression::print(llvm::raw_ostream& stream) const {
    stream << "(";
    this->leftExpr->print(stream);
    stream << ") + (";
    this->rightExpr->print(stream);
    stream << ")";
}

llvm::Value* AST::AddOpExpression::generate(Generator& generator) const {
    return generator.generate(*this);
}

AST::SubOpExpression::SubOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
    : BinaryOpExpression(std::move(leftExpr), std::move(rightExpr)) { }

llvm::Value* AST::SubOpExpression::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::SubOpExpression::print(llvm::raw_ostream& stream) const {
    stream << "(";
    this->leftExpr->print(stream);
    stream << ") - (";
    this->rightExpr->print(stream);
    stream << ")";
}

AST::MulOpExpression::MulOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
        : BinaryOpExpression(std::move(leftExpr), std::move(rightExpr)) { }

llvm::Value* AST::MulOpExpression::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::MulOpExpression::print(llvm::raw_ostream& stream) const {
    stream << "(";
    this->leftExpr->print(stream);
    stream << ") * (";
    this->rightExpr->print(stream);
    stream << ")";
}

AST::DivOpExpression::DivOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
        : BinaryOpExpression(std::move(leftExpr), std::move(rightExpr)) { }

llvm::Value* AST::DivOpExpression::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::DivOpExpression::print(llvm::raw_ostream& stream) const {
    stream << "(";
    this->leftExpr->print(stream);
    stream << ") / (";
    this->rightExpr->print(stream);
    stream << ")";
}

void AST::IntegerType::print(llvm::raw_ostream& stream) const {
    stream << "int";
}

llvm::IntegerType* AST::IntegerType::generate(Generator& generator) const {
    return generator.generate(*this);
}

AST::FunctionPrototype::FunctionPrototype(const std::vector<std::shared_ptr<const AST::Type>>& parameters,
        std::shared_ptr<const AST::Type> returnType)
    : parameters(parameters), returnType(std::move(returnType)) { }

llvm::FunctionType* AST::FunctionPrototype::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::FunctionPrototype::print(llvm::raw_ostream& stream) const {
    stream << "(";
    if (!this->parameters.empty()) this->parameters[0]->print(stream);
    for (int i = 1; i < this->parameters.size(); ++i) {
        stream << ", ";
        this->parameters[i]->print(stream);
    }
    stream << ") -> ";
    this->returnType->print(stream);
}

AST::Function::Function(const std::string& name, std::shared_ptr<const AST::FunctionPrototype> type)
    : name(name), type(std::move(type)) { }

llvm::Function* AST::Function::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::Function::print(llvm::raw_ostream& stream) const {
    stream << "extern " << this->name << ": ";
    this->type->print(stream);
    stream << ";";
}

AST::Statement::Statement(std::shared_ptr<const AST::Expression> expr) : expr(std::move(expr)) { }

llvm::Value* AST::Statement::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::Statement::print(llvm::raw_ostream& stream) const {
    this->expr->print(stream);
    stream << ";";
}

AST::File::File(std::vector<std::shared_ptr<const AST::Function>> funcs,
        std::vector<std::shared_ptr<const AST::Statement>> statements)
    : funcs(std::move(funcs)), statements(std::move(statements)) { }

llvm::Function* AST::File::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::File::print(llvm::raw_ostream& stream) const {
    for (const auto& func : this->funcs) {
        func->print(stream);
        stream << "\n";
    }

    for (const auto& stmt : this->statements) {
        stmt->print(stream);
        stream << "\n";
    }
}

AST::CharLiteral::CharLiteral(std::shared_ptr<const Token> value) : value(value->source[0]) { }

llvm::Value* AST::CharLiteral::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::CharLiteral::print(llvm::raw_ostream& stream) const {
    stream << "\'" << this->value << "\'";
}

AST::IntegerLiteral::IntegerLiteral(std::shared_ptr<const Token> value) : value(std::stoi(value->source)) { }

llvm::Value* AST::IntegerLiteral::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::IntegerLiteral::print(llvm::raw_ostream& stream) const {
    stream << this->value;
}

AST::FunctionCall::FunctionCall(std::shared_ptr<const Token> callee,
        const std::vector<std::shared_ptr<const AST::Expression>> arguments)
    : callee(callee->source), arguments(arguments) { }

llvm::Value* AST::FunctionCall::generate(Generator& generator) const {
    return generator.generate(*this);
}

void AST::FunctionCall::print(llvm::raw_ostream& stream) const {
    stream << this->callee << "(";
    if (!this->arguments.empty()) this->arguments[0]->print(stream);
    for (int i = 1; i < this->arguments.size(); ++i) {
        stream << ", ";
        this->arguments[i]->print(stream);
    }
    stream << ")";
}