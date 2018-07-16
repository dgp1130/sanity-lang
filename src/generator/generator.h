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
class Generator {
public:
    llvm::Value* generate(const AST::AddOpExpression& addition);
    llvm::Value* generate(const AST::SubOpExpression& subtraction);
    llvm::Value* generate(const AST::MulOpExpression& multiplication);
    llvm::Value* generate(const AST::DivOpExpression& division);
    llvm::IntegerType* generate(const AST::IntegerType& integer);
    llvm::PointerType* generate(const AST::StringType& string);
    llvm::FunctionType* generate(const AST::FunctionPrototype& prototype);
    llvm::Function* generate(const AST::Function& func);
    llvm::Value* generate(const AST::Statement& stmt);
    llvm::Function* generate(const AST::File& file);
    llvm::Value* generate(const AST::CharLiteral& literal);
    llvm::Value* generate(const AST::IntegerLiteral& literal);
    llvm::Value* generate(const AST::StringLiteral& literal);
    llvm::CallInst* generate(const AST::FunctionCall& call);
};

#endif //SANITY_SANITY_GENERATOR_H