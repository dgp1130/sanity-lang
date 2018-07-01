#include <gtest/gtest.h>
#include "ast.h"
#include "globals.h"
#include "token.h"
#include "token_builder.h"
#include "llvm/Support/raw_ostream.h"

TEST(AST, AddOpExpressionPrints) {
    const auto leftValue = TokenBuilder("1").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("2").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto addition = AST::AddOpExpression(leftExpr, rightExpr);

    std::string str;
    llvm::raw_string_ostream ss(str);
    addition.print(ss);
    ASSERT_EQ("1 + 2", ss.str());
}

TEST(AST, SubOpExpressionPrints) {
    const auto leftValue = TokenBuilder("2").setIntegerLiteral(true).build();
    const auto leftExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(leftValue));
    const auto rightValue = TokenBuilder("1").setIntegerLiteral(true).build();
    const auto rightExpr = std::make_shared<const AST::IntegerLiteral>(AST::IntegerLiteral(rightValue));
    const auto subtraction = AST::SubOpExpression(leftExpr, rightExpr);

    std::string str;
    llvm::raw_string_ostream ss(str);
    subtraction.print(ss);
    ASSERT_EQ("2 - 1", ss.str());
}

TEST(AST, IntegerTypePrints) {
    const auto integer = AST::IntegerType();

    std::string str;
    llvm::raw_string_ostream ss(str);
    integer.print(ss);
    ASSERT_EQ("int", ss.str());
}

TEST(AST, FunctionPrototypePrints) {
    const auto integer = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ integer, integer });
    const auto proto = AST::FunctionPrototype(params, integer);

    std::string str;
    llvm::raw_string_ostream ss(str);
    proto.print(ss);
    ASSERT_EQ("(int, int) -> int", ss.str());
}

TEST(AST, FunctionPrints) {
    const auto integer = std::make_shared<const AST::IntegerType>(AST::IntegerType());
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ integer, integer });
    const auto proto = std::make_shared<const AST::FunctionPrototype>(AST::FunctionPrototype(
            params, integer /* returnType */));
    const auto func = AST::Function("test", proto);

    std::string str;
    llvm::raw_string_ostream ss(str);
    func.print(ss);
    ASSERT_EQ("extern test: (int, int) -> int;", ss.str());
}

TEST(AST, StatementPrints) {
    std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    const auto identifier = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(token));
    auto stmt = AST::Statement(identifier);

    std::string str;
    llvm::raw_string_ostream ss(str);
    stmt.print(ss);
    ASSERT_EQ("\'a\';", ss.str());
}

TEST(AST, FilePrints) {
    const auto params = std::vector<std::shared_ptr<const AST::Type>>({ std::make_shared<const AST::IntegerType>(AST::IntegerType()) });
    const auto returnValue = std::make_shared<AST::IntegerType>(AST::IntegerType());
    const auto externDeclType = std::make_shared<const AST::FunctionPrototype>(AST::FunctionPrototype(params, returnValue));
    const auto externDecl = std::make_shared<const AST::Function>(AST::Function("putchar", externDeclType));

    std::shared_ptr<const Token> char1Token = TokenBuilder("a").setCharLiteral(true).build();
    const auto char1 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(char1Token));
    const auto firstStmt = std::make_shared<const AST::Statement>(AST::Statement(char1));

    std::shared_ptr<const Token> char2Token = TokenBuilder("b").setCharLiteral(true).build();
    const auto char2 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(char2Token));
    const auto secondStmt = std::make_shared<const AST::Statement>(AST::Statement(char2));

    const std::vector<std::shared_ptr<const AST::Function>> externDecls = {externDecl};
    const std::vector<std::shared_ptr<const AST::Statement>> stmts = {firstStmt, secondStmt};
    const auto file = AST::File(externDecls, stmts);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file.print(ss);
    ASSERT_EQ("extern putchar: (int) -> int;\n\'a\';\n\'b\';\n", ss.str());
}

TEST(AST, CharLiteralParsesToChar) {
    std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    ASSERT_EQ('a', AST::CharLiteral(token).value);
}

TEST(AST, CharLiteralPrints) {
    std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    const auto literal = AST::CharLiteral(token);

    std::string str;
    llvm::raw_string_ostream ss(str);
    literal.print(ss);
    ASSERT_EQ("\'a\'", ss.str());
}

TEST(AST, IntegerLiteralParsesToInteger) {
    std::shared_ptr<const Token> token = TokenBuilder("1234").setIntegerLiteral(true).build();
    ASSERT_EQ(1234, AST::IntegerLiteral(token).value);
}

TEST(AST, IntegerLiteralPrints) {
    std::shared_ptr<const Token> token = TokenBuilder("1234").setIntegerLiteral(true).build();
    const auto literal = AST::IntegerLiteral(token);

    std::string str;
    llvm::raw_string_ostream ss(str);
    literal.print(ss);
    ASSERT_EQ("1234", ss.str());
}

TEST(AST, FunctionCallPrints) {
    const std::shared_ptr<const Token> callee = TokenBuilder("test").build();
    const std::shared_ptr<const Token> charLiteral1 = TokenBuilder("a").setCharLiteral(true).build();
    const auto arg1 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(charLiteral1));
    const std::shared_ptr<const Token> charLiteral2 = TokenBuilder("b").setCharLiteral(true).build();
    const auto arg2 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(charLiteral2));
    const std::vector<std::shared_ptr<const AST::Expression>> args({ arg1, arg2 });

    const auto func = AST::FunctionCall(callee, args);

    std::string str;
    llvm::raw_string_ostream ss(str);
    func.print(ss);
    ASSERT_EQ("test(\'a\', \'b\')", ss.str());
}