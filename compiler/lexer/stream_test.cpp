#include <gtest/gtest.h>
#include <memory>
#include "stream.h"
#include "compiler/utils/queue_utils.h"
#include "compiler/models/exceptions.h"

typedef ::testing::Test TestFixture;

typedef Exceptions::SyntaxException SyntaxException;

class StreamTestFixture : public TestFixture {
protected:
    std::unique_ptr<Stream> stream;

    void SetUp() override {
        std::queue<char> q = QueueUtils::queueify("abc123");
        stream = std::make_unique<Stream>(Stream(q));
    }
};

TEST_F(StreamTestFixture, FrontReturnsFirstCharacter) {
    const char c = this->stream->front();
    ASSERT_EQ('a', c);
}

TEST_F(StreamTestFixture, IgnoresCharacters) {
    this->stream->ignore(3)->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("", token.value()->source);
}

TEST_F(StreamTestFixture, ConsumesCharacters) {
    this->stream->consume(3)->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc", token.value()->source);
}

TEST_F(StreamTestFixture, ConsumesTheGivenCharacterWithoutAdvancing) {
    this->stream->consume('z')->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("z", token.value()->source);
    ASSERT_EQ('a', stream->front());
}

TEST_F(StreamTestFixture, ConsumesWhileCharacterMatchRegex) {
    this->stream->consumeWhile(std::regex("^[a-z]"), 1)->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc", token.value()->source);
}

TEST_F(StreamTestFixture, ConsumeWhileDoesNotThrowExceptionOnEofWhenCalledWithoutMessage) {
    this->stream->consumeWhile(std::regex("^."), 1)->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc123", token.value()->source);
}

TEST_F(StreamTestFixture, ConsumeWhileThrowsExceptionOnEofWhenCalledWithMessage) {
    ASSERT_THROW(this->stream->consumeWhile(std::regex("^."), 1, std::string("Unexpected EOF."))->returnToken(),
            SyntaxException);
}

TEST_F(StreamTestFixture, MatchInvokesThenCallbackForMatchingRegex) {
    bool thenCalled = false;
    bool elseCalled = false;
    this->stream->match(std::regex("^[a-z]"), 1, [&thenCalled](Stream* stream) {
        thenCalled = true;
    }, [&elseCalled](Stream* stream) {
        elseCalled = true;
    });

    ASSERT_TRUE(thenCalled);
    ASSERT_FALSE(elseCalled);
}

TEST_F(StreamTestFixture, MatchInvokesElseCallbackForNonMatchingRegex) {
    bool thenCalled = false;
    bool elseCalled = false;
    this->stream->match(std::regex("^[0-9]"), 1, [&thenCalled](Stream* stream) {
        thenCalled = true;
    }, [&elseCalled](Stream* stream) {
        elseCalled = true;
    });

    ASSERT_FALSE(thenCalled);
    ASSERT_TRUE(elseCalled);
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
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc", token.value()->source);
}

TEST_F(StreamTestFixture, RepeatDoesNotInvokeCallbackIfNoMatch) {
    this->stream->repeat(std::regex("^[0-9]"), 1, [](Stream* stream) {
        FAIL();
    });

    SUCCEED();
}

TEST_F(StreamTestFixture, RepeatDoesNotThrowExceptionOnEofWhenCalledWithoutMessage) {
    ASSERT_NO_THROW(this->stream->repeat(std::regex("^."), 1, [](Stream* stream) {
       stream->ignore();
    }));
}

TEST_F(StreamTestFixture, RepeatThrowsExceptionOnEofWhenCalledWithMessage) {
    ASSERT_THROW(this->stream->repeat(std::regex("^."), 1, [](Stream* stream) {
        stream->ignore();
    }, std::string("Unexpected EOF")), SyntaxException);
}

TEST_F(StreamTestFixture, CallsAfterReturnTokenAreIgnoredUntilNextRun) {
    std::queue<std::shared_ptr<const Token>> tokens;
    std::experimental::optional<std::shared_ptr<const Token>> token;
    do {
        token = this->stream->match(std::regex("^[a-z]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[a-z]"), 1)->returnToken();
        })->match(std::regex("^[0-9]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[0-9]"), 1)->returnToken();
        })->extractResult();
        if (token) tokens.push(token.value());
    } while (token);

    ASSERT_EQ(2, tokens.size());

    std::shared_ptr<const Token> first = tokens.front();
    ASSERT_EQ("abc", first->source);
    tokens.pop();

    std::shared_ptr<const Token> second = tokens.front();
    ASSERT_EQ("123", second->source);
    tokens.pop();
}