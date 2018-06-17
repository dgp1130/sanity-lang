#include <gtest/gtest.h>
#include "ast.h"
#include "globals.h"
#include "token.h"
#include "token_builder.h"
#include "llvm/Support/raw_ostream.h"

TEST(AST, FunctionPrototypePrints) {
    const auto proto = AST::FunctionPrototype("test", std::vector<llvm::Type*>({ llvm::Type::getInt32Ty(*context) }));

    std::string str;
    llvm::raw_string_ostream ss(str);
    proto.print(ss);
    ASSERT_EQ("extern test(i32);", ss.str());
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
    std::shared_ptr<const Token> decl = TokenBuilder("putchar").build();
    const auto externDecl = std::make_shared<const AST::FunctionPrototype>(AST::FunctionPrototype("putchar",
        std::vector<llvm::Type*>({ llvm::Type::getInt32Ty(*context) })));

    std::shared_ptr<const Token> char1Token = TokenBuilder("a").setCharLiteral(true).build();
    const auto char1 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(char1Token));
    const auto firstStmt = std::make_shared<const AST::Statement>(AST::Statement(char1));

    std::shared_ptr<const Token> char2Token = TokenBuilder("b").setCharLiteral(true).build();
    const auto char2 = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(char2Token));
    const auto secondStmt = std::make_shared<const AST::Statement>(AST::Statement(char2));

    const std::vector<std::shared_ptr<const AST::FunctionPrototype>> externDecls = {externDecl};
    const std::vector<std::shared_ptr<const AST::Statement>> stmts = {firstStmt, secondStmt};
    const auto file = AST::File(externDecls, stmts);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file.print(ss);
    ASSERT_EQ("extern putchar(i32);\n\'a\';\n\'b\';\n", ss.str());
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

TEST(AST, FunctionCallPrints) {
    const std::shared_ptr<const Token> callee = TokenBuilder("test").build();
    const std::shared_ptr<const Token> charLiteral = TokenBuilder("a").setCharLiteral(true).build();
    const auto argument = std::make_shared<AST::CharLiteral>(AST::CharLiteral(charLiteral));

    const auto func = AST::FunctionCall(callee, argument);

    std::string str;
    llvm::raw_string_ostream ss(str);
    func.print(ss);
    ASSERT_EQ("test(\'a\')", ss.str());
}