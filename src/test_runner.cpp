#include <gtest/gtest.h>

#include "generator/generator_test.cpp"
#include "lexer/lexer_test.cpp"
#include "lexer/stream_test.cpp"
#include "models/ast_test.cpp"
#include "models/exceptions_test.cpp"
#include "models/globals.h"
#include "models/token_test.cpp"
#include "parser/parser_test.cpp"
#include "utils/file_utils_test.cpp"
#include "utils/queue_utils_test.cpp"

// Declared in globals.h
std::unique_ptr<llvm::LLVMContext> context = llvm::make_unique<llvm::LLVMContext>();
llvm::IRBuilder<> builder(*context);
std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("SanityTest", *context);
std::map<std::string, llvm::Value*> namedValues;

/**
 * Starts the GoogleTest runner by executing all included tests.
 */
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}