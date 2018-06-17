#include <gtest/gtest.h>
#include "generator.h"
#include "../models/ast.h"
#include "../models/token_builder.h"
#include "../models/globals.h"

TEST(Generator, GeneratesFunctionPrototype) {
    const AST::FunctionPrototype proto("test", std::vector<llvm::Type*>({ llvm::Type::getInt32Ty(*context) }));
    const llvm::Function* func = Generator().generate(proto);

    ASSERT_EQ("test", func->getName().str());
    ASSERT_EQ(1, func->arg_size());
    ASSERT_EQ(llvm::Type::getInt32Ty(*context), func->getArgumentList().front().getType());
}

TEST(Generator, GeneratesMainFromFile) {
    const auto proto = std::make_shared<const AST::FunctionPrototype>(
            AST::FunctionPrototype("test", std::vector<llvm::Type*>({ llvm::Type::getInt32Ty(*context) })));

    const std::shared_ptr<const Token> nameToken1 = TokenBuilder("test").build();
    const std::shared_ptr<const Token> charToken1 = TokenBuilder("a").setCharLiteral(true).build();
    const auto arg1 = std::make_shared<const AST::CharLiteral>(charToken1);
    const auto expr1 = std::make_shared<const AST::FunctionCall>(AST::FunctionCall(nameToken1, arg1));
    const auto stmt1 = std::make_shared<const AST::Statement>(AST::Statement(expr1));

    const std::shared_ptr<const Token> nameToken2 = TokenBuilder("test").build();
    const std::shared_ptr<const Token> charToken2 = TokenBuilder("b").setCharLiteral(true).build();
    const auto arg2 = std::make_shared<const AST::CharLiteral>(charToken2);
    const auto expr2 = std::make_shared<const AST::FunctionCall>(AST::FunctionCall(nameToken2, arg2));
    const auto stmt2 = std::make_shared<const AST::Statement>(AST::Statement(expr2));

    const AST::File file(std::vector<std::shared_ptr<const AST::FunctionPrototype>>({ proto }),
        std::vector<std::shared_ptr<const AST::Statement>>({ stmt1, stmt2 }));
    const llvm::Function* main = Generator().generate(file);

    ASSERT_EQ("main", main->getName());
    ASSERT_EQ(0, main->arg_size());
}

TEST(Generator, GeneratesCharLiteral) {
    const std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    const auto charLiteral = AST::CharLiteral(token);
    auto generated = (llvm::ConstantInt*) Generator().generate(charLiteral);

    ASSERT_EQ((int64_t) 'a', generated->getValue().getSExtValue());
}

TEST(Generator, GeneratesFunctionCall) {
    Generator generator;
    const auto externDecl = AST::FunctionPrototype("test",
        std::vector<llvm::Type*>({ llvm::Type::getInt32Ty(*context) }));
    generator.generate(externDecl);

    const std::shared_ptr<const Token> name = TokenBuilder("test").build();
    const std::shared_ptr<const Token> literal = TokenBuilder("a").setCharLiteral(true).build();
    const auto argument = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(literal));

    const auto func = AST::FunctionCall(name, argument);
    const llvm::CallInst* call = generator.generate(func);

    // ASSERT_EQ("test", call->getName().str()); Name is always empty string?
    ASSERT_EQ(1, call->getNumArgOperands());
    ASSERT_EQ((int64_t) 'a', ((llvm::ConstantInt*) call->getArgOperand(0))->getValue().getSExtValue());
}