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

TEST_F(StreamTestFixture, IgnoresWhileCharacterMatchesRegex) {
    this->stream->ignoreWhile(std::regex("^[a-z]"), 1)->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ('1', stream->front());
}

TEST_F(StreamTestFixture, IgnoreWhileStopsOnNonMatchingRegex) {
    this->stream->ignoreWhile(std::regex("^[0-9]"), 1)->returnToken();
    this->stream->extractResult();

    ASSERT_EQ('a', stream->front());
}

TEST_F(StreamTestFixture, IgnoreWhileDoesNotThrowExceptionOnEofWhenCalledWithoutMessage) {
    ASSERT_NO_THROW(this->stream->ignoreWhile(std::regex("^."), 1)->returnToken());
}

TEST_F(StreamTestFixture, IgnoreWhileThrowsExceptionOnEofWhenCalledWithMessage) {
    ASSERT_THROW(this->stream->ignoreWhile(
        std::regex("^."),
        1,
        true /* updateStartColumn */,
        std::string("Unexpected EOF.")
    )->returnToken(), SyntaxException);
}

TEST_F(StreamTestFixture, IgnoresUntilCharacterMatchesRegex) {
    this->stream->ignoreUntil(std::regex("^[0-9]"), 1)->returnToken();
    this->stream->extractResult();

    ASSERT_EQ('1', stream->front());
}

TEST_F(StreamTestFixture, IgnoreUntilStopsOnMatchingRegex) {
    this->stream->ignoreUntil(std::regex("^[a-z]"), 1)->returnToken();
    this->stream->extractResult();

    ASSERT_EQ('a', stream->front());
}

TEST_F(StreamTestFixture, IgnoreUntilDoesNotThrowExceptionOnEofWhenCalledWithoutMessage) {
    ASSERT_NO_THROW(this->stream->ignoreUntil(std::regex("^z"), 1)->returnToken());
}

TEST_F(StreamTestFixture, IgnoreUntilThrowsExceptionOnEofWhenCalledWithMessage) {
    ASSERT_THROW(this->stream->ignoreUntil(
        std::regex("^z"),
        1,
        true /* updateStartColumn */,
        std::string("Unexpected EOF.")
    )->returnToken(), SyntaxException);
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

TEST_F(StreamTestFixture, ConsumesWhileCharacterMatchesRegex) {
    this->stream->consumeWhile(std::regex("^[a-z]"), 1)->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc", token.value()->source);
}

TEST_F(StreamTestFixture, ConsumeWhileStopsOnNonMatchingRegex) {
    this->stream->consumeWhile(std::regex("^[0-9]"), 1)->returnToken();
    this->stream->extractResult();

    ASSERT_EQ('a', stream->front());
}

TEST_F(StreamTestFixture, ConsumeWhileDoesNotThrowExceptionOnEofWhenCalledWithoutMessage) {
    ASSERT_NO_THROW(this->stream->consumeWhile(std::regex("^."), 1)->returnToken());
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

TEST_F(StreamTestFixture, RepeatWhileInvokesCallbackWhileRegexMatches) {
    this->stream->repeatWhile(std::regex("^[a-z]"), 1, [](Stream* stream) {
        stream->consume();
    })->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc", token.value()->source);
}

TEST_F(StreamTestFixture, RepeatWhileDoesNotInvokeCallbackIfNoMatch) {
    this->stream->repeatWhile(std::regex("^[0-9]"), 1, [](Stream* stream) {
        FAIL();
    });

    SUCCEED();
}

TEST_F(StreamTestFixture, RepeatWhileDoesNotThrowExceptionOnEofWhenStreamContinues) {
    ASSERT_NO_THROW(this->stream->repeatWhile(std::regex("^."), 1, [](Stream* stream) {
       stream->ignore();
    }));
}

TEST_F(StreamTestFixture, RepeatWhileThrowsExceptionOnEofWhenStreamEnds) {
    ASSERT_THROW(this->stream->repeatWhile(std::regex("^."), 1, [](Stream* stream) {
        stream->ignore();
    }, std::string("Unexpected EOF")), SyntaxException);
}

TEST_F(StreamTestFixture, RepeatUntilInvokesCallbackUntilRegexMatches) {
    this->stream->repeatUntil(std::regex("^[0-9]"), 1, [](Stream* stream) {
        stream->consume();
    })->returnToken();
    std::experimental::optional<std::shared_ptr<const Token>> token = this->stream->extractResult();

    ASSERT_EQ("abc", token.value()->source);
}

TEST_F(StreamTestFixture, RepeatUntilDoesNotInvokeCallbackIfMatch) {
    this->stream->repeatUntil(std::regex("^[a-z]"), 1, [](Stream* stream) {
        FAIL();
    });

    SUCCEED();
}

TEST_F(StreamTestFixture, RepeatUntilDoesNotThrowExceptionOnEofWhenStreamContinues) {
    ASSERT_NO_THROW(this->stream->repeatUntil(std::regex("^z"), 1, [](Stream* stream) {
        stream->ignore();
    }));
}

TEST_F(StreamTestFixture, RepeatUntilThrowsExceptionOnEofWhenStreamEnds) {
    ASSERT_THROW(this->stream->repeatUntil(std::regex("^z"), 1, [](Stream* stream) {
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