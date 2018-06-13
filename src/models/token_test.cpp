#include <gtest/gtest.h>
#include <sstream>
#include "token.h"
#include "token_builder.h"

TEST(Token, SavesSourceText) {
    const auto token = TokenBuilder("test").build();

    ASSERT_EQ("test", token->source);
}

TEST(Token, PrintsSourceText) {
    const auto token = TokenBuilder("test").build();
    auto stream = std::ostringstream();

    stream << *token;

    ASSERT_EQ("test", stream.str());
}

TEST(Token, SavesLineAndColumnState) {
    const auto token = TokenBuilder("a")
        .setLine(1)
        .setStartCol(2)
        .setEndCol(3)
    .build();

    ASSERT_EQ(1, token->line);
    ASSERT_EQ(2, token->startCol);
    ASSERT_EQ(3, token->endCol);
}

TEST(Token, SavesCharLiteralState) {
    const auto charToken = TokenBuilder("a")
        .setCharLiteral(true)
    .build();
    ASSERT_TRUE(charToken->isCharLiteral);

    const auto nonCharToken = TokenBuilder("test").build();
    ASSERT_FALSE(nonCharToken->isCharLiteral);
}