#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#ifndef SANITY_GLOBALS_H
#define SANITY_GLOBALS_H

/**
 * File for exporting all the global variables used by the Sanity compiler.
 */

// These LLVM context values should really be Generator's fields', but putting them there causes errors on destruction.
// These are initialized in main.cpp.
extern std::unique_ptr<llvm::LLVMContext> context;
extern llvm::IRBuilder<> builder;
extern std::unique_ptr<llvm::Module> module;
extern std::map<std::string, llvm::Value*> namedValues;

#endif //SANITY_GLOBALS_H