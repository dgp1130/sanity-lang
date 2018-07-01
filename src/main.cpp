#include <gflags/gflags.h>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>
#include "utils/file_utils.h"
#include "generator/generator.h"
#include "lexer/lexer.h"
#include "models/ast.h"
#include "models/exceptions.h"
#include "models/globals.h"
#include "parser/parser.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

// Declared in globals.h
std::unique_ptr<llvm::LLVMContext> context = llvm::make_unique<llvm::LLVMContext>();
llvm::IRBuilder<> builder(*context);
std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("Sanity", *context);
std::map<std::string, llvm::Value*> namedValues;

typedef Exceptions::FileNotFoundException FileNotFoundException;
typedef Exceptions::ParseException ParseException;
typedef Exceptions::SyntaxException SyntaxException;
typedef Exceptions::TypeException TypeException;
typedef Exceptions::UndeclaredException UndeclaredException;

DEFINE_string(input, "-", "Path to a file of Sanity source code to compile or \"-\" to use stdin.");

int main(int argc, char* argv[]) {
    const auto progName = std::string(argv[0]);
    gflags::SetUsageMessage("Compiles Sanity source code to LLVM IR.\n$ cat <source>.sane | " + progName + " | lli");
    gflags::ParseCommandLineFlags(&argc, &argv, true /* remove flags from argv */);

    const auto inputFile = FLAGS_input != "-" ? FLAGS_input : "/dev/stdin";

    // Read file into a queue of characters.
    std::queue<char> chars;
    try {
        chars = FileUtils::readFileChars(inputFile);
    } catch (const FileNotFoundException& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // Tokenize the characters.
    std::queue<std::shared_ptr<const Token>> tokens;
    try {
        tokens = Lexer::tokenize(chars);
    } catch (const SyntaxException& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // Parse the tokens.
    std::shared_ptr<const AST::File> file;
    try {
        file = Parser::parse(tokens);
    } catch (const ParseException& ex) {
        std::cerr << "ParseException: " << ex.what() << std::endl;
        return 1;
    }

    // Generate the LLVM IR.
    try {
        Generator().generate(*file);
    } catch (const TypeException& ex) {
        std::cerr << "TypeException: " << ex.what() << std::endl;
        return 1;
    } catch (const UndeclaredException& ex) {
        std::cerr << "UndeclaredException: " << ex.what() << std::endl;
        return 1;
    }

    // Verify the IR output.
    llvm::verifyModule(*module);

    // Just print the IR output for now.
    module->print(llvm::outs(), nullptr);

    // Completed successfully.
    return 0;
}