#include "generator.h"

#include <memory>
#include <iostream>
#include <vector>
#include <llvm/IR/Verifier.h>
#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "../models/ast.h"
#include "../models/exceptions.h"
#include "../models/globals.h"

typedef Exceptions::AssertionException AssertionException;
typedef Exceptions::TypeException TypeException;
typedef Exceptions::UndeclaredException UndeclaredException;

const int CHAR_BIT_SIZE = 32; // putchar() uses int32 rather than int8.
const int INTEGER_BIT_SIZE = 32;

llvm::Value* Generator::generate(const AST::Statement& stmt) {
    return stmt.expr->generate(*this);
}

llvm::Function* Generator::generate(const AST::FunctionPrototype& prototype) {
    llvm::FunctionType* type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), prototype.parameters,
            false /* isVarArgs */);
    return llvm::Function::Create(type, llvm::Function::ExternalLinkage, prototype.name,
            module.get());
}

llvm::Function* Generator::generate(const AST::File& file) {
    for (const auto& funcDeclaration : file.funcDeclarations) {
        funcDeclaration->generate(*this);
    }

    // Stub a main function because one is required.
    const AST::FunctionPrototype mainProto("main", std::vector<llvm::Type*>());
    llvm::Function* main = mainProto.generate(*this);

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* bb = llvm::BasicBlock::Create(*context, "entry", main);
    builder.SetInsertPoint(bb);

    // Record the function arguments in the NamedValues map.
    namedValues.clear();
    for (auto& Arg : main->args()) namedValues[Arg.getName()] = &Arg;

    // Generate the body of the main function.
    for (const auto& stmt : file.statements) {
        stmt->generate(*this);
    }

    // Return 0 always
    llvm::APInt retVal(INTEGER_BIT_SIZE, (uint32_t) 0, true /* signed */);
    builder.CreateRet(llvm::ConstantInt::get(*context, retVal));

    llvm::verifyFunction(*main);
    return main;
}

llvm::Value* Generator::generate(const AST::CharLiteral& literal) {
    llvm::APInt llvmInt(CHAR_BIT_SIZE, (uint32_t) literal.value, true /* signed */);
    return llvm::ConstantInt::get(*context, llvmInt);
}

// Generate a call to a function. Currently assumes it takes exactly one argument and the result is dropped because that
// is all a "Hello World!" program with putchar() requires.
llvm::CallInst* Generator::generate(const AST::FunctionCall& call) {
    llvm::Function* func = module->getFunction(call.callee);

    if (!func) throw UndeclaredException("Function \"" + call.callee + "\" not declared in this scope.");

    if (func->arg_size() != 1) {
        throw TypeException("Function \"" + call.callee + "\" wants " + std::to_string(func->arg_size())
            + " arguments, expected 1.");
    }

    llvm::Value* arg = call.argument->generate(*this);
    if (!arg) throw AssertionException("Unable to generate argument to function call: " + call.callee);
    std::vector<llvm::Value*> arguments({ arg });

    return builder.CreateCall(func, arguments);
}