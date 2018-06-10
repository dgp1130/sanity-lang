#include <gtest/gtest.h>
#include "lexer.h"
#include "../utils/queue_utils.h"
#include <queue>

TEST(Lexer, TokenizesWholeWords) {
    std::queue<char> q = QueueUtils::queueify("Hello");
    std::queue<const Token*> tokens = Lexer::tokenize(q);

    ASSERT_EQ(1, tokens.size());

    const Token* token = tokens.front();
    tokens.pop();
    ASSERT_EQ("Hello", token->source);
    delete token;
}

TEST(Lexer, TokenizesWholeWordsWithUnderscores) {
    std::queue<char> q = QueueUtils::queueify("_test test_one");
    std::queue<const Token*> tokens = Lexer::tokenize(q);

    ASSERT_EQ(2, tokens.size());

    const Token* first = tokens.front();
    tokens.pop();
    ASSERT_EQ("_test", first->source);
    delete first;

    const Token* second = tokens.front();
    tokens.pop();
    ASSERT_EQ("test_one", second->source);
    delete second;
}

TEST(Lexer, TokenizesWholeWordsWithNumbers) {
    std::queue<char> q = QueueUtils::queueify("test123");
    std::queue<const Token*> tokens = Lexer::tokenize(q);

    ASSERT_EQ(1, tokens.size());

    const Token* token = tokens.front();
    tokens.pop();
    ASSERT_EQ("test123", token->source);
    delete token;
}

TEST(Lexer, TokenizesNumbers) {
    std::queue<char> q = QueueUtils::queueify("123 test");
    std::queue<const Token*> tokens = Lexer::tokenize(q);

    ASSERT_EQ(2, tokens.size());

    const Token* first = tokens.front();
    tokens.pop();
    ASSERT_EQ("123", first->source);
    delete first;

    const Token* second = tokens.front();
    tokens.pop();
    ASSERT_EQ("test", second->source);
    delete second;
}

TEST(Lexer, TokenizesPunctuation) {
    std::queue<char> q = QueueUtils::queueify(".!?");
    std::queue<const Token*> tokens = Lexer::tokenize(q);

    ASSERT_EQ(3, tokens.size());

    const Token* first = tokens.front();
    tokens.pop();
    ASSERT_EQ(".", first->source);
    delete first;

    const Token* second = tokens.front();
    tokens.pop();
    ASSERT_EQ("!", second->source);
    delete second;

    const Token* third = tokens.front();
    tokens.pop();
    ASSERT_EQ("?", third->source);
    delete third;
}

TEST(Lexer, IgnoresWhitespace) {
    std::queue<char> q = QueueUtils::queueify("   \tabc  \n\t  123\n\t");
    std::queue<const Token*> tokens = Lexer::tokenize(q);

    ASSERT_EQ(2, tokens.size());

    const Token* first = tokens.front();
    tokens.pop();
    ASSERT_EQ("abc", first->source);
    delete first;

    const Token* second = tokens.front();
    tokens.pop();
    ASSERT_EQ("123", second->source);
    delete second;
}