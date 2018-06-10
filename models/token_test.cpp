#include <gtest/gtest.h>
#include <sstream>
#include "token.h"

TEST(Token, PrintsSourceText) {
    const auto token = Token("test");
    auto stream = std::ostringstream();

    stream << token;

    ASSERT_EQ("test", stream.str());
}