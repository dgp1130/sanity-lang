#include <gtest/gtest.h>
#include "ast.h"
#include "token.h"
#include "token_builder.h"

TEST(AST, StatementPrints) {
    std::shared_ptr<const Token> token = TokenBuilder("test").build();
    auto identifier = std::make_shared<const AST::Identifier>(AST::Identifier(token));
    auto stmt = AST::Statement(identifier);

    std::ostringstream ss;
    stmt.print(ss);
    ASSERT_EQ("test;", ss.str());
}

TEST(AST, BlockPrints) {
    std::shared_ptr<const Token> identifierToken = TokenBuilder("test").build();
    auto identifier = std::make_shared<const AST::Identifier>(AST::Identifier(identifierToken));
    auto firstStmt = std::make_shared<const AST::Statement>(AST::Statement(identifier));

    std::shared_ptr<const Token> charToken = TokenBuilder("a").setCharLiteral(true).build();
    auto charLiteral = std::make_shared<const AST::CharLiteral>(AST::CharLiteral(charToken));
    auto secondStmt = std::make_shared<const AST::Statement>(AST::Statement(charLiteral));

    const std::vector<std::shared_ptr<const AST::Statement>> stmts = {firstStmt, secondStmt};
    const auto block = AST::Block(stmts);

    std::ostringstream ss;
    block.print(ss);
    ASSERT_EQ("test;\n\'a\';\n", ss.str());
}

TEST(AST, IdentifierSavesToString) {
    std::shared_ptr<const Token> token = TokenBuilder("test").build();
    ASSERT_EQ("test", AST::Identifier(token).name);
}

TEST(AST, IdentifierPrints) {
    std::shared_ptr<const Token> token = TokenBuilder("test").build();
    const auto identifier = AST::Identifier(token);

    std::ostringstream ss;
    identifier.print(ss);
    ASSERT_EQ("test", ss.str());
}

TEST(AST, CharLiteralParsesToChar) {
    std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    ASSERT_EQ('a', AST::CharLiteral(token).value);
}

TEST(AST, CharLiteralPrints) {
    std::shared_ptr<const Token> token = TokenBuilder("a").setCharLiteral(true).build();
    const auto literal = AST::CharLiteral(token);

    std::ostringstream ss;
    literal.print(ss);
    ASSERT_EQ("\'a\'", ss.str());
}

TEST(AST, FunctionCallPrints) {
    std::shared_ptr<const Token> funcName = TokenBuilder("test").build();
    const auto callee = std::make_shared<AST::Identifier>(AST::Identifier(funcName));

    const std::shared_ptr<const Token> charLiteral = TokenBuilder("a").build();
    const auto argument = std::make_shared<AST::CharLiteral>(AST::CharLiteral(charLiteral));

    const auto func = AST::FunctionCall(callee, argument);

    std::ostringstream ss;
    func.print(ss);
    ASSERT_EQ("test(\'a\')", ss.str());
}