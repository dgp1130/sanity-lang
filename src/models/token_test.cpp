#include <gtest/gtest.h>
#include <sstream>
#include "token.h"

TEST(Token, SavesSourceText) {
    const auto token = Token("test", false /* isCharLiteral */);

    ASSERT_EQ("test", token.source);
}

TEST(Token, PrintsSourceText) {
    const auto token = Token("test", false /* isCharLiteral */);
    auto stream = std::ostringstream();

    stream << token;

    ASSERT_EQ("test", stream.str());
}

TEST(Token, SavesCharLiteralState) {
    const auto charToken = Token("a", true /* isCharLiteral */);
    ASSERT_TRUE(charToken.isCharLiteral);

    const auto nonCharToken = Token("test", false /* isCharLiteral */);
    ASSERT_FALSE(nonCharToken.isCharLiteral);
}