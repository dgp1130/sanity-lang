#ifndef SANITY_SANITY_GENERATOR_H
#define SANITY_SANITY_GENERATOR_H

#include <memory>
#include <vector>
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "../models/ast.h"

/**
 * Visitor class for the AST models which generates LLVM IR objects based on the abstract syntax tree.
 */
class Generator : public AST::IGenerator {
protected:
    Generator() = default;

public:
    static llvm::Function* gen(const AST::File& file);

    llvm::Value* generate(const AST::AddOpExpression& addition) override;
    llvm::Value* generate(const AST::SubOpExpression& subtraction) override;
    llvm::Value* generate(const AST::MulOpExpression& multiplication) override;
    llvm::Value* generate(const AST::DivOpExpression& division) override;
    llvm::IntegerType* generate(const AST::IntegerType& integer) override;
    llvm::PointerType* generate(const AST::StringType& string) override;
    llvm::FunctionType* generate(const AST::FunctionPrototype& prototype) override;
    llvm::Function* generate(const AST::Function& func) override;
    void generate(const AST::StatementExpression& stmt) override;
    void generate(const AST::StatementLet& stmt) override;
    llvm::Function* generate(const AST::File& file) override;
    llvm::Value* generate(const AST::CharLiteral& literal) override;
    llvm::Value* generate(const AST::IntegerLiteral& literal) override;
    llvm::Value* generate(const AST::StringLiteral& literal) override;
    llvm::CallInst* generate(const AST::FunctionCall& call) override;
    llvm::Value* generate(const AST::IdentifierExpr& identifier) override;
};

#endif //SANITY_SANITY_GENERATOR_H