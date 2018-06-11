#include <gtest/gtest.h>
#include "stream.h"
#include "../utils/queue_utils.h"

typedef ::testing::Test TestFixture;

class StreamTestFixture : public TestFixture {
protected:
    Stream* stream;

    void SetUp() override {
        std::queue<char> q = QueueUtils::queueify("abc123");
        stream = new Stream(q);
    }

    void TearDown() override {
        delete stream;
    }
};

TEST_F(StreamTestFixture, IgnoresCharacters) {
    this->stream->ignore(3)->returnToken();
    const Token* token = this->stream->extractResult();

    ASSERT_EQ("", token->source);

    delete token;
}

TEST_F(StreamTestFixture, ConsumesCharacters) {
    this->stream->consume(3)->returnToken();
    const Token* token = this->stream->extractResult();

    ASSERT_EQ("abc", token->source);

    delete token;
}

TEST_F(StreamTestFixture, ConsumesWhileCharacterMatchRegex) {
    this->stream->consumeWhile(std::regex("^[a-z]"), 1)->returnToken();
    const Token* token = this->stream->extractResult();

    ASSERT_EQ("abc", token->source);

    delete token;
}

TEST_F(StreamTestFixture, MatchInvokesCallbackForMatchingRegex) {
    bool called = false;
    this->stream->match(std::regex("^[a-z]"), 1, [&called](Stream* stream) {
        called = true;
    });

    ASSERT_EQ(true, called);
}

TEST_F(StreamTestFixture, MatchDoesNotInvokeCallbackForNonMatchingRegex) {
    this->stream->match(std::regex("^[0-9]"), 1, [](Stream* stream) {
       FAIL();
    });

    SUCCEED();
}

TEST_F(StreamTestFixture, RepeatInvokesCallbackWhileRegexMatches) {
    this->stream->repeat(std::regex("^[a-z]"), 1, [](Stream* stream) {
        stream->consume();
    })->returnToken();
    const Token* token = this->stream->extractResult();

    ASSERT_EQ("abc", token->source);

    delete token;
}

TEST_F(StreamTestFixture, RepeatDoesNotInvokeCallbackIfNoMatch) {
    this->stream->repeat(std::regex("^[0-9]"), 1, [](Stream* stream) {
        FAIL();
    });

    SUCCEED();
}

TEST_F(StreamTestFixture, CallsAfterReturnTokenAreIgnoredUntilNextRun) {
    std::queue<const Token*> tokens;
    const Token* token;
    do {
        token = this->stream->match(std::regex("^[a-z]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[a-z]"), 1)->returnToken();
        })->match(std::regex("^[0-9]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[0-9]"), 1)->returnToken();
        })->extractResult();
        if (token != nullptr) tokens.push(token);
    } while (token != nullptr);

    ASSERT_EQ(2, tokens.size());

    const Token* first = tokens.front();
    ASSERT_EQ("abc", first->source);
    delete first;
    tokens.pop();

    const Token* second = tokens.front();
    ASSERT_EQ("123", second->source);
    delete second;
    tokens.pop();
}