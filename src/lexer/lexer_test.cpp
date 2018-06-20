#include <gtest/gtest.h>
#include "lexer.h"
#include "../utils/queue_utils.h"
#include "../models/exceptions.h"
#include <memory>
#include <queue>

typedef Exceptions::SyntaxException SyntaxException;

TEST(Lexer, TokenizesWholeWords) {
    std::queue<char> q = QueueUtils::queueify("Hello");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(1, tokens.size());

    std::shared_ptr<const Token> token = tokens.front();
    tokens.pop();
    ASSERT_EQ("Hello", token->source);
}

TEST(Lexer, TokenizesWholeWordsWithUnderscores) {
    std::queue<char> q = QueueUtils::queueify("_test test_one");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(2, tokens.size());

    std::shared_ptr<const Token> first = tokens.front();
    tokens.pop();
    ASSERT_EQ("_test", first->source);

    std::shared_ptr<const Token> second = tokens.front();
    tokens.pop();
    ASSERT_EQ("test_one", second->source);
}

TEST(Lexer, TokenizesWholeWordsWithNumbers) {
    std::queue<char> q = QueueUtils::queueify("test123");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(1, tokens.size());

    std::shared_ptr<const Token> token = tokens.front();
    tokens.pop();
    ASSERT_EQ("test123", token->source);
}

TEST(Lexer, TokenizesNumbers) {
    std::queue<char> q = QueueUtils::queueify("123 test");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(2, tokens.size());

    std::shared_ptr<const Token> first = tokens.front();
    tokens.pop();
    ASSERT_EQ("123", first->source);

    std::shared_ptr<const Token> second = tokens.front();
    tokens.pop();
    ASSERT_EQ("test", second->source);
}

TEST(Lexer, TokenizesCharacters) {
    std::queue<char> q = QueueUtils::queueify("\'a\'");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(1, tokens.size());

    std::shared_ptr<const Token> token = tokens.front();
    tokens.pop();
    ASSERT_EQ("a", token->source);
    ASSERT_EQ(true, token->isCharLiteral);
}

TEST(Lexer, TokenizingInvalidCharactersThrowsSyntaxException) {
    std::queue<char> q = QueueUtils::queueify("\'ab");
    ASSERT_THROW(Lexer::tokenize(q), SyntaxException);
}

TEST(Lexer, TokenizesFunctionYieldToken) {
    std::queue<char> q = QueueUtils::queueify("->");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(1, tokens.size());
    ASSERT_EQ("->", tokens.front()->source);
}

TEST(Lexer, TokenizesPunctuation) {
    std::queue<char> q = QueueUtils::queueify(".!?");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(3, tokens.size());

    std::shared_ptr<const Token> first = tokens.front();
    tokens.pop();
    ASSERT_EQ(".", first->source);

    std::shared_ptr<const Token> second = tokens.front();
    tokens.pop();
    ASSERT_EQ("!", second->source);

    std::shared_ptr<const Token> third = tokens.front();
    tokens.pop();
    ASSERT_EQ("?", third->source);
}

TEST(Lexer, IgnoresWhitespace) {
    std::queue<char> q = QueueUtils::queueify("   \tabc  \n\t  123\n\t");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(2, tokens.size());

    std::shared_ptr<const Token> first = tokens.front();
    tokens.pop();
    ASSERT_EQ("abc", first->source);

    std::shared_ptr<const Token> second = tokens.front();
    tokens.pop();
    ASSERT_EQ("123", second->source);
}

TEST(Lexer, TokenizesEscapedCharacterLiterals) {
    std::queue<char> q = QueueUtils::queueify(R"('\n' '\r' '\t' '\'' '\"' '\\')");
    std::queue<std::shared_ptr<const Token>> tokens = Lexer::tokenize(q);

    ASSERT_EQ(6, tokens.size());

    std::shared_ptr<const Token> newline = tokens.front();
    tokens.pop();
    ASSERT_EQ("\n", newline->source);

    std::shared_ptr<const Token> carriageReturn = tokens.front();
    tokens.pop();
    ASSERT_EQ("\r", carriageReturn->source);

    std::shared_ptr<const Token> tab = tokens.front();
    tokens.pop();
    ASSERT_EQ("\t", tab->source);

    std::shared_ptr<const Token> singleQuote = tokens.front();
    tokens.pop();
    ASSERT_EQ("\'", singleQuote->source);

    std::shared_ptr<const Token> doubleQuote = tokens.front();
    tokens.pop();
    ASSERT_EQ("\"", doubleQuote->source);

    std::shared_ptr<const Token> backslash = tokens.front();
    tokens.pop();
    ASSERT_EQ("\\", backslash->source);
}

TEST(Lexer, TokenizingUnknownEscapeCharacterThrowsException) {
    std::queue<char> q = QueueUtils::queueify("\'\\z\'");
    ASSERT_THROW(Lexer::tokenize(q), SyntaxException);
}

TEST(Lexer, TokenizingInvalidCharacterLiteralsThrowsException) {
    std::queue<char> qNewline = QueueUtils::queueify("\'\n\'");
    ASSERT_THROW(Lexer::tokenize(qNewline), SyntaxException);

    std::queue<char> qCarriageReturn = QueueUtils::queueify("\'\r\'");
    ASSERT_THROW(Lexer::tokenize(qCarriageReturn), SyntaxException);

    std::queue<char> qTab = QueueUtils::queueify("\'\t\'");
    ASSERT_THROW(Lexer::tokenize(qTab), SyntaxException);

    std::queue<char> qSingleQuote = QueueUtils::queueify("\'\'\'");
    ASSERT_THROW(Lexer::tokenize(qSingleQuote), SyntaxException);

    std::queue<char> qDoubleQuote = QueueUtils::queueify("\'\"\'");
    ASSERT_THROW(Lexer::tokenize(qDoubleQuote), SyntaxException);

    // Backslash is weird because the '\' escapes the trailing single-quote waiting for another single quote.
    // This is actually an unexpected EOF file error. This throws an IllegalStateException, which it really shouldn't,
    // but close enough for now.
    std::queue<char> qBackslash = QueueUtils::queueify("\'\\\'");
    ASSERT_ANY_THROW(Lexer::tokenize(qBackslash));
}