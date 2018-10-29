#include <vector>
#include "compiler/models/ast.h"
#include "compiler/models/token.h"
#include "llvm/Support/raw_ostream.h"
#include "compiler/models/globals.h"
#include "compiler/models/exceptions.h"

typedef Exceptions::UndeclaredException UndeclaredException;

AST::BinaryOpExpression::BinaryOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
    : leftExpr(std::move(leftExpr)), rightExpr(std::move(rightExpr)) { }

AST::AddOpExpression::AddOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
    : BinaryOpExpression(std::move(leftExpr), std::move(rightExpr)) { }

llvm::Value* AST::AddOpExpression::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::AddOpExpression::print(llvm::raw_ostream& stream) const {
    stream << "(";
    this->leftExpr->print(stream);
    stream << ") + (";
    this->rightExpr->print(stream);
    stream << ")";
}

AST::SubOpExpression::SubOpExpression(std::shared_ptr<const AST::Expression> leftExpr,
        std::shared_ptr<const AST::Expression> rightExpr)
    : BinaryOpExpression(std::move(leftExpr), std::move(rightExpr)) { }

llvm::Value* AST::SubOpExpression::generate(IGenerator& generator) const {
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

llvm::Value* AST::MulOpExpression::generate(IGenerator& generator) const {
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

llvm::Value* AST::DivOpExpression::generate(IGenerator& generator) const {
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

llvm::IntegerType* AST::IntegerType::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::StringType::print(llvm::raw_ostream& stream) const {
    stream << "string";
}

llvm::PointerType* AST::StringType::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

AST::FunctionPrototype::FunctionPrototype(const std::vector<std::shared_ptr<const AST::Type>>& parameters,
        std::shared_ptr<const AST::Type> returnType)
    : parameters(parameters), returnType(std::move(returnType)) { }

llvm::FunctionType* AST::FunctionPrototype::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::FunctionPrototype::print(llvm::raw_ostream& stream) const {
    stream << "(";
    if (!this->parameters.empty()) this->parameters[0]->print(stream);
    for (unsigned int i = 1; i < this->parameters.size(); ++i) {
        stream << ", ";
        this->parameters[i]->print(stream);
    }
    stream << ") -> ";
    this->returnType->print(stream);
}

AST::Function::Function(const std::string& name, std::shared_ptr<const AST::FunctionPrototype> type)
    : name(name), type(std::move(type)) { }

llvm::Function* AST::Function::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::Function::print(llvm::raw_ostream& stream) const {
    stream << "extern " << this->name << ": ";
    this->type->print(stream);
    stream << ";";
}

AST::StatementExpression::StatementExpression(std::shared_ptr<const AST::Expression> expr) : expr(std::move(expr)) { }

void AST::StatementExpression::generate(IGenerator& generator) const {
    generator.generate(*this);
}

void AST::StatementExpression::print(llvm::raw_ostream& stream) const {
    this->expr->print(stream);
    stream << ";";
}

AST::StatementLet::StatementLet(std::shared_ptr<const Token> name, std::shared_ptr<const AST::Type> type,
        std::shared_ptr<const AST::Expression> expr)
    : name(name->source), type(std::move(type)), expr(std::move(expr)) { }

void AST::StatementLet::generate(IGenerator& generator) const {
    generator.generate(*this);
}

void AST::StatementLet::print(llvm::raw_ostream& stream) const {
    stream << "let " << this->name << ": ";
    this->type->print(stream);
    stream << " = ";
    this->expr->print(stream);
    stream << ";";
}

AST::File::File(std::vector<std::shared_ptr<const AST::Function>> funcs,
        std::vector<std::shared_ptr<const AST::Statement>> statements)
    : funcs(std::move(funcs)), statements(std::move(statements)) { }

llvm::Function* AST::File::generate(IGenerator& generator) const {
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

llvm::Value* AST::CharLiteral::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::CharLiteral::print(llvm::raw_ostream& stream) const {
    stream << "\'" << this->value << "\'";
}

AST::IntegerLiteral::IntegerLiteral(std::shared_ptr<const Token> value) : value(std::stoi(value->source)) { }

llvm::Value* AST::IntegerLiteral::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::IntegerLiteral::print(llvm::raw_ostream& stream) const {
    stream << this->value;
}

AST::StringLiteral::StringLiteral(std::shared_ptr<const Token> value) : value(value->source) { }

llvm::Value* AST::StringLiteral::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::StringLiteral::print(llvm::raw_ostream& stream) const {
    stream << "\"" << this->value << "\"";
}

AST::FunctionCall::FunctionCall(std::shared_ptr<const Token> callee,
        const std::vector<std::shared_ptr<const AST::Expression>> arguments)
    : callee(callee->source), arguments(arguments) { }

llvm::Value* AST::FunctionCall::generate(IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::FunctionCall::print(llvm::raw_ostream& stream) const {
    stream << this->callee << "(";
    if (!this->arguments.empty()) this->arguments[0]->print(stream);
    for (unsigned int i = 1; i < this->arguments.size(); ++i) {
        stream << ", ";
        this->arguments[i]->print(stream);
    }
    stream << ")";
}

AST::IdentifierExpr::IdentifierExpr(const std::shared_ptr<const Token> name) : name(name->source) { }

llvm::Value* AST::IdentifierExpr::generate(AST::IGenerator& generator) const {
    return generator.generate(*this);
}

void AST::IdentifierExpr::print(llvm::raw_ostream& stream) const {
    stream << this->name;
}