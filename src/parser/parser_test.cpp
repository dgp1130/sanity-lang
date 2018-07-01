#include <gtest/gtest.h>

#include <queue>
#include "parser.h"
#include "../models/ast.h"
#include "../models/exceptions.h"
#include "../models/token.h"
#include "../models/token_builder.h"
#include "../utils/queue_utils.h"
#include "llvm/Support/raw_ostream.h"

typedef Exceptions::ParseException ParseException;

TEST(Parser, ParsesEmptyFile) {
    std::shared_ptr<const Token> tokens[] = {};
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 0 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    ASSERT_TRUE(file->statements.empty());
}

TEST(Parser, ParsesExtern) {
    std::shared_ptr<const Token> tokens[] = {
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
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 11 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("extern test: (int, int) -> int;\n", ss.str());
}

TEST(Parser, ThrowsParseExceptionOnExternsUnexpectedEOF) {
    std::shared_ptr<const Token> tokens[] = {
            TokenBuilder("extern").build(),
            TokenBuilder("test").build(),
            TokenBuilder(":").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 3 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnExternsStatementWithNoType) {
    std::shared_ptr<const Token> tokens[] = {
            TokenBuilder("extern").build(),
            TokenBuilder("test").build(),
            TokenBuilder(":").build(),
            TokenBuilder("blarg").build(),
            TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 5 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ParsesAdditionOperation) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder("+").build(),
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 4 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("1 + 2;\n", ss.str());
}

TEST(Parser, ParsesSubtractionOperation) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("2").setIntegerLiteral(true).build(),
        TokenBuilder("-").build(),
        TokenBuilder("1").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 4 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("2 - 1;\n", ss.str());
}

TEST(Parser, ParsesSingleFunctionCall) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(",").build(),
        TokenBuilder("b").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 7 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("test(\'a\', \'b\');\n", ss.str());
}

TEST(Parser, ParsesFunctionCallWithEmptyArguments) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 4 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("test();\n", ss.str());
}

TEST(Parser, ParsesSingleCharLiteralStatement) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 2 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("\'a\';\n", ss.str());
}

TEST(Parser, ParsesSingleIntegerLiteralStatement) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("1234").setIntegerLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 2 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("1234;\n", ss.str());
}

TEST(Parser, ParsesMultipleStatements) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test1").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),

        TokenBuilder("b").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 7 /* length */);

    std::shared_ptr<const AST::File> file = Parser::parse(input);

    std::string str;
    llvm::raw_string_ostream ss(str);
    file->print(ss);
    ASSERT_EQ("test1(\'a\');\n\'b\';\n", ss.str());
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingName) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("(").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 1 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingOpenParen) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 2 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingCloseParen) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 4 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnStatementMissingSemicolon) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder("foo").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 5 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnUnexpectedEOF) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        // EOF
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 2 /* length */);

    ASSERT_THROW(Parser::parse(input), ParseException);
}