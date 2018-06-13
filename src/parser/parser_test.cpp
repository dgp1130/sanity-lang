#include <gtest/gtest.h>

#include <queue>
#include "parser.h"
#include "../models/exceptions.h"
#include "../models/token.h"
#include "../models/token_builder.h"
#include "../utils/queue_utils.h"

typedef Exceptions::ParseException ParseException;

TEST(Parser, ParsesEmptyFile) {
    const Token* tokens[] = {};
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 0 /* length */);

    ASSERT_NO_THROW(Parser(input).parse());
}

TEST(Parser, ParsesSingleFunctionCall) {
    const Token* tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").charLiteral().build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 5 /* length */);

    ASSERT_NO_THROW(Parser(input).parse());
}

TEST(Parser, ParsesMultipleFunctionCalls) {
    const Token* tokens[] = {
        TokenBuilder("test1").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").charLiteral().build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),

        TokenBuilder("test2").build(),
        TokenBuilder("(").build(),
        TokenBuilder("b").charLiteral().build(),
        TokenBuilder(")").build(),
        TokenBuilder(";").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 10 /* length */);

    ASSERT_NO_THROW(Parser(input).parse());
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingName) {
    const Token* tokens[] = {
        TokenBuilder("(").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 1 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingOpenParen) {
    const Token* tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder(";").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 2 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingArgument) {
    const Token* tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder(")").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 3 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnFunctionCallMissingCloseParen) {
    const Token* tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").charLiteral().build(),
        TokenBuilder(";").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 4 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnStatementMissingSemicolon) {
    const Token* tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        TokenBuilder("a").charLiteral().build(),
        TokenBuilder(")").build(),
        TokenBuilder("foo").build(),
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 5 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}

TEST(Parser, ThrowsParseExceptionOnUnexpectedEOF) {
    const Token* tokens[] = {
        TokenBuilder("test").build(),
        TokenBuilder("(").build(),
        // EOF
    };
    std::queue<const Token*> input = QueueUtils::queueify(tokens, 2 /* length */);

    ASSERT_THROW(Parser(input).parse(), ParseException);
}