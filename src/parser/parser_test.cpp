#include <gtest/gtest.h>

#include <queue>
#include "parser.h"
#include "../models/exceptions.h"
#include "../models/token.h"
#include "../models/token_builder.h"
#include "../utils/queue_utils.h"

typedef Exceptions::ParseException ParseException;

TEST(Parser, ParsesEmptyFile) {
    std::shared_ptr<const Token> tokens[] = {};
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 0 /* length */);

    ASSERT_NO_THROW(Parser(input).parse());
}

TEST(Parser, ParsesSingleFunctionCall) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 5 /* length */);

    ASSERT_NO_THROW(Parser(input).parse());
}

TEST(Parser, ParsesMultipleFunctionCalls) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test1").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),

        TokenBuilder("test2").build(),
        TokenBuilder("(").build(),
        TokenBuilder("b").setCharLiteral(true).build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 10 /* length */);

    ASSERT_NO_THROW(Parser(input).parse());
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingName) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("(").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 1 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingOpenParen) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 2 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingArgument) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder(")").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 3 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingCloseParen) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").setCharLiteral(true).build(),
        TokenBuilder(";").build(),
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 4 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
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

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnUnexpectedEOF) {
    std::shared_ptr<const Token> tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        // EOF
    };
    std::queue<std::shared_ptr<const Token>> input = QueueUtils::queueify(tokens, 2 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}