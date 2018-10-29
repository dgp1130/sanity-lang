#include <gtest/gtest.h>

#include <queue>
#include "parser.h"
#include "compiler/models/ast.h"
#include "compiler/models/exceptions.h"
#include "compiler/models/token.h"
#include "compiler/models/token_builder.h"
#include "compiler/utils/queue_utils.h"
#include "llvm/Support/raw_ostream.h"

typedef Exceptions::ParseException ParseException;

TEST(Parser, ParsesEmptyFile) {
    const std::vector<std::shared_ptr<const Token>> tokens = {};
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    ASSERT_TRUE(file->statements.empty());
}

TEST(Parser, ParsesExtern) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("extern").build(),
        TokenBuilder("test").build(),
        TokenBuilder(":").build(),
        TokenBuilder("(").build(),
        TokenBuilder("int").build(),
        TokenBuilder(",").build(),
        TokenBuilder("int").build(),
        TokenBuilder(")").build(),
        TokenBuilder("->").build(),
        TokenBuilder("int").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("extern test: (int, int) -> int;\n", ss.str());
}

TEST(Parser, ParsesIntegerType) {
    // Currently, the int type can only be used in an extern which must be a function.
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("extern").build(),
        TokenBuilder("test").build(),
        TokenBuilder(":").build(),
        TokenBuilder("(").build(),
        TokenBuilder(")").build(),
        TokenBuilder("->").build(),
        TokenBuilder("int").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("extern test: () -> int;\n", ss.str());
}

TEST(Parser, ParsesStringType) {
    // Currently, the string type can only be used in an extern which must be a function.
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("extern").build(),
        TokenBuilder("test").build(),
        TokenBuilder(":").build(),
        TokenBuilder("(").build(),
        TokenBuilder(")").build(),
        TokenBuilder("->").build(),
        TokenBuilder("string").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("extern test: () -> string;\n", ss.str());
}

TEST(Parser, ThrowsParseExceptionOnExternsUnexpectedEOF) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("extern").build(),
        TokenBuilder("test").build(),
        TokenBuilder(":").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnExternsStatementWithNoType) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("extern").build(),
        TokenBuilder("test").build(),
        TokenBuilder(":").build(),
        TokenBuilder("blarg").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ParsesAdditionOperationLeftToRight) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder("+").build(),
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder("+").build(),
        TokenBuilder("3").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("((1) + (2)) + (3);\n", ss.str());
}

TEST(Parser, ParsesSubtractionOperationLeftToRight) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("3").setIntegerLiteral(true).build(),
        TokenBuilder("-").build(),
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder("-").build(),
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("((3) - (2)) - (1);\n", ss.str());
}

TEST(Parser, ParsesMultiplicationOperationLeftToRight) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder("*").build(),
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder("*").build(),
        TokenBuilder("3").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("((1) * (2)) * (3);\n", ss.str());
}

TEST(Parser, ParsesDivisionOperationLeftToRight) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("3").setIntegerLiteral(true).build(),
        TokenBuilder("/").build(),
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder("/").build(),
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("((3) / (2)) / (1);\n", ss.str());
}

TEST(Parser, RespectsOrderOfOperations) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder("+").build(),
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder("*").build(),
        TokenBuilder("3").setIntegerLiteral(true).build(),
        TokenBuilder("-").build(),
        TokenBuilder("4").setIntegerLiteral(true).build(),
        TokenBuilder("/").build(),
        TokenBuilder("(").build(),
        TokenBuilder("5").setIntegerLiteral(true).build(),
        TokenBuilder("+").build(),
        TokenBuilder("6").setIntegerLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("((1) + ((2) * (3))) - ((4) / ((5) + (6)));\n", ss.str());
}

TEST(Parser, ParsesSingleFunctionCall) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(",").build(),
        TokenBuilder("b").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("test(\'a\', \'b\');\n", ss.str());
}

TEST(Parser, ParsesFunctionCallWithEmptyArguments) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("test();\n", ss.str());
}

TEST(Parser, ParsesSingleCharLiteralStatement) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("\'a\';\n", ss.str());
}

TEST(Parser, ParsesSingleIntegerLiteralStatement) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("1234").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("1234;\n", ss.str());
}

TEST(Parser, ParsesSingleStringLiteralStatement) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("abc123").setStringLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("\"abc123\";\n", ss.str());
}

TEST(Parser, ParsesMultipleStatements) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("test1").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),

        TokenBuilder("b").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("test1(\'a\');\n\'b\';\n", ss.str());
}

TEST(Parser, ParsesStatementLet) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("let").build(),
        TokenBuilder("foo").build(),
        TokenBuilder(":").build(),
        TokenBuilder("int").build(),
        TokenBuilder("=").build(),
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("let foo: int = 1;\n", ss.str());
}

TEST(Parser, ParsesIdentifierUsageInExpression) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("foo").build(),
        TokenBuilder("+").build(),
        TokenBuilder("bar").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("(foo) + (bar);\n", ss.str());
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingName) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("(").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingCloseParen) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnStatementMissingSemicolon) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder("foo").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnUnexpectedEOF) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        // EOF
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnUnmatchedParen) {
    const std::vector<std::shared_ptr<const Token>> tokens = {
        TokenBuilder("(").build(),
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens);

    ASSERT_THROW(Parser::parse(input), ParseException);
}