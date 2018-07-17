#include <gtest/gtest.h>
#include "generator.h"
#include "../models/ast.h"
#include "../models/token_builder.h"
#include "../models/globals.h"

// Extend Generator to get access to protected constructor.
class GeneratorUnderTest : public Generator { };

TEST(Generator, GeneratesAddOpExpression) {
    const auto leftValue = TokenBuilder("1").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("2").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto addition = AST::AddOpExpression(leftExpr, rightExpr);

    const llvm::Value* sum = GeneratorUnderTest().generate(addition);

    ASSERT_EQ((int64_t) 3, ((llvm::ConstantInt*) sum)->getSExtValue());
}

TEST(Generator, GeneratesSubOpExpression) {
    const auto leftValue = TokenBuilder("2").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("1").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto subtraction = AST::SubOpExpression(leftExpr, rightExpr);

    const llvm::Value* difference = GeneratorUnderTest().generate(subtraction);

    ASSERT_EQ((int64_t) 1, ((llvm::ConstantInt*) difference)->getSExtValue());
}

TEST(Generator, GeneratesMulOpExpression) {
    const auto leftValue = TokenBuilder("2").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("3").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto multiplication = AST::MulOpExpression(leftExpr, rightExpr);

    const llvm::Value* product = GeneratorUnderTest().generate(multiplication);

    ASSERT_EQ((int64_t) 6, ((llvm::ConstantInt*) product)->getSExtValue());
}

TEST(Generator, GeneratesDivOpExpression) {
    const auto leftValue = TokenBuilder("6").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("3").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto division = AST::DivOpExpression(leftExpr, rightExpr);

    const llvm::Value* quotient = GeneratorUnderTest().generate(division);

    ASSERT_EQ((int64_t) 2, ((llvm::ConstantInt*) quotient)->getSExtValue());
}

TEST(Generator, GeneratesFlooredDivExpression) {
    const auto leftValue = TokenBuilder("8").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("3").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto division = AST::DivOpExpression(leftExpr, rightExpr);

    const llvm::Value* quotient = GeneratorUnderTest().generate(division);

    ASSERT_EQ((int64_t) 2, ((llvm::ConstantInt*) quotient)->getSExtValue());
}

TEST(Generator, GeneratesIntegerType) {
    const llvm::IntegerType* integer = GeneratorUnderTest().generate(AST::IntegerType());

    ASSERT_EQ(32, integer->getBitWidth());
}

TEST(Generator, GeneratesStringType) {
    const llvm::PointerType* string = GeneratorUnderTest().generate(AST::StringType());

    ASSERT_EQ(true, string->isPointerTy());
}

TEST(Generator, GeneratesFunctionPrototype) {
    const auto integer = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ integer, integer });
    const AST::FunctionPrototype proto(params, integer /* returnType */);
    const llvm::FunctionType* func = GeneratorUnderTest().generate(proto);

    ASSERT_EQ(2, func->getNumParams());
    ASSERT_EQ(llvm::IntegerType::getInt32Ty(*context), func->params()[0]);
    ASSERT_EQ(llvm::IntegerType::getInt32Ty(*context), func->params()[1]);
    ASSERT_EQ(llvm::IntegerType::getInt32Ty(*context), func->getReturnType());
}

TEST(Generator, GeneratesFunction) {
    const auto integer = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ integer, integer });
    const auto proto = std::make_shared<const AST::FunctionPrototype>(AST::FunctionPrototype(
            params, integer /* returnType */));
    const AST::Function function("test", proto);
    const llvm::Function* func = GeneratorUnderTest().generate(function);

    ASSERT_EQ("test", func->getName().str());

    ASSERT_EQ(2, func->arg_size());
    auto argument = func->arg_begin();
    ASSERT_EQ(llvm::IntegerType::getInt32Ty(*context), argument->getType());
    argument++;
    ASSERT_EQ(llvm::IntegerType::getInt32Ty(*context), argument->getType());
    argument++;

    ASSERT_EQ(llvm::IntegerType::getInt32Ty(*context), func->getReturnType());
}

TEST(Generator, GeneratesMainFromFile) {
    const auto integer = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ integer });
    const auto proto = std::make_shared<const AST::FunctionPrototype>(
            AST::FunctionPrototype(params, integer /* returnType */));
    const auto func = std::make_shared<const AST::Function>("test2", proto);

    const std::shared_ptr<const Token> nameToken1 = TokenBuilder("test2").build();
    const std::shared_ptr<const Token> charToken1 = TokenBuilder("a").setCharLiteral(true).build();
    const auto arg1 = std::make_shared<const AST::CharLiteral>(charToken1);
    const auto arguments1 = std::vector<std::shared_ptr<const AST::Expression>>({ arg1 });
    const auto expr1 = std::make_shared<const AST::FunctionCall>(AST::FunctionCall(nameToken1, arguments1));
    const auto stmt1 = std::make_shared<const AST::Statement>(AST::Statement(expr1));

    const std::shared_ptr<const Token> nameToken2 = TokenBuilder("test2").build();
    const std::shared_ptr<const Token> charToken2 = TokenBuilder("b").setCharLiteral(true).build();
    const auto arg2 = std::make_shared<const AST::CharLiteral>(charToken2);
    const auto arguments2 = std::vector<std::shared_ptr<const AST::Expression>>({ arg2 });
    const auto expr2 = std::make_shared<const AST::FunctionCall>(AST::FunctionCall(nameToken2, arguments2));
    const auto stmt2 = std::make_shared<const AST::Statement>(AST::Statement(expr2));

    const AST::File file(std::vector<std::shared_ptr<const AST::Function>>({ func }),
        std::vector<std::shared_ptr<const AST::Statement>>({ stmt1, stmt2 }));
    const llvm::Function* main = GeneratorUnderTest().generate(file);

    ASSERT_EQ("main", main->getName());
    ASSERT_EQ(0, main->arg_size());
}

TEST(Generator, GeneratesCharLiteral) {
    const std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    const auto charLiteral = AST::CharLiteral(token);
    auto generated = (llvm::ConstantInt*) GeneratorUnderTest().generate(charLiteral);

    ASSERT_EQ((int64_t) 'a', generated->getValue().getSExtValue());
}

TEST(Generator, GeneratesIntegerLiteral) {
    const std::shared_ptr<const Token> token = TokenBuilder("1234").setIntegerLiteral(true).build();
    const auto integerLiteral = AST::IntegerLiteral(token);
    auto generated = (llvm::ConstantInt*) GeneratorUnderTest().generate(integerLiteral);

    ASSERT_EQ((int64_t) 1234, generated->getValue().getSExtValue());
}

TEST(Generator, GeneratesStringLiteral) {
    const std::shared_ptr<const Token> token = TokenBuilder("abc123").setStringLiteral(true).build();
    const auto stringLiteral = AST::StringLiteral(token);

    // Not sure how to verify the string's contents.
    ASSERT_NO_THROW((llvm::ConstantExpr*) GeneratorUnderTest().generate(stringLiteral));
}

TEST(Generator, GeneratesFunctionCall) {
    GeneratorUnderTest generator;
    const auto param1 = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto param2 = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ param1, param2 });
    const auto returnType = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto externProto = std::make_shared<const AST::FunctionPrototype>(
            AST::FunctionPrototype(params, returnType));
    const auto externDecl = AST::Function("test3", externProto);
    generator.generate(externDecl);

    const std::shared_ptr<const Token> name = TokenBuilder("test3").build();
    const std::shared_ptr<const Token> literal1 = TokenBuilder("a").setCharLiteral(true).build();
    const auto arg1 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(literal1));
    const std::shared_ptr<const Token> literal2 = TokenBuilder("b").setCharLiteral(true).build();
    const auto arg2 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(literal2));
    const auto args = std::vector<std::shared_ptr<const AST::Expression>>({ arg1, arg2 });

    const auto func = AST::FunctionCall(name, args);
    const llvm::CallInst* call = generator.generate(func);

    // ASSERT_EQ("test3", call->getName().str()); Name is always empty string?
    ASSERT_EQ(2, call->getNumArgOperands());
    ASSERT_EQ((int64_t) 'a', ((llvm::ConstantInt*) call->getArgOperand(0))->getValue().getSExtValue());
    ASSERT_EQ((int64_t) 'b', ((llvm::ConstantInt*) call->getArgOperand(1))->getValue().getSExtValue());
}