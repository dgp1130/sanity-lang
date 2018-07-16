#include "stream.h"
#include <deque>
#include <queue>
#include <memory>
#include <regex>
#include <functional>
#include <experimental/optional>
#include "../models/token.h"
#include "../models/exceptions.h"
#include "../models/token_builder.h"

typedef Exceptions::IllegalStateException IllegalStateException;
typedef Exceptions::SyntaxException SyntaxException;

std::string dequeToString(std::deque<char>& chars, const unsigned long limit) {
    // Take the limit number of characters from the deque and put them into a string.
    char cString[limit + 1];
    for (unsigned long i = 0; i < limit; ++i) {
        cString[i] = chars.front();
        chars.pop_front();
    }
    cString[limit] = '\0'; // Add following null character.
    const std::string str(cString);

    // Put the removed characters back on the deque.
    for (long long i = (long long) limit - 1; i >= 0; --i) {
        chars.push_front(cString[i]);
    }

    return str;
}

std::string dequeToString(std::deque<char>& chars) {
    return dequeToString(chars, chars.size());
}

Stream::Stream(std::queue<char>& chars) {
    // Convert the provided queue into a deque for easier manipulation.
    this->chars = std::deque<char>();
    while (!chars.empty()) {
        this->chars.push_back(chars.front());
        chars.pop();
    }

    this->buffer = std::deque<char>();
    this->result = nullptr;
}

void Stream::advanceChars(const int numChars, const bool updateStartColumn) {
    for (int i = 0; i < numChars; ++i) {
        char c = this->chars.front();
        this->chars.pop_front();

        // Update line numbers
        if (c == '\n') {
            this->line++;
            this->currentCol = 1;

            // If no characters have been parsed, then we haven't started yet, update start positions.
            if (updateStartColumn && this->buffer.empty()) {
                this->startCol = 1;
            }
        } else {
            this->currentCol++;

            // If no characters have been parsed, then we haven't started yet, update start positions.
            if (updateStartColumn && this->buffer.empty()) this->startCol++;
        }
    }
}

char Stream::front() {
    if (!this->active()) throw IllegalStateException("No characters left in the Stream.");

    return this->chars.front();
}

Stream* Stream::ignore(const int numChars, const bool updateStartColumn) {
    if (!this->active()) return this;

    this->advanceChars(numChars, updateStartColumn);

    return this;
}

Stream* Stream::consume(const int numChars) {
    if (!this->active()) return this;

    for (int i = 0; i < numChars; ++i) {
        this->consume(this->chars.front());
        this->advanceChars(1 /* numChars */, true /* updateStartColumn */);
    }

    return this;
}

Stream* Stream::consume(const char character) {
    this->buffer.push_back(character);

    return this;
}

Stream* Stream::consumeWhile(const std::regex& matcher, const int limit,
        const std::experimental::optional<const std::string>& eofError) {
    if (!this->active()) return this;

    this->repeat(matcher, limit, [](Stream* stream) {
        stream->consume();
    }, eofError);

    return this;
}

Stream* Stream::match(const std::regex& matcher, const int limit, const std::function<void (Stream* stream)>& callback) {
    return this->match(matcher, limit, callback, [](Stream* stream) {
        // Do nothing on else.
    });
}

Stream* Stream::match(const std::regex& matcher, const int limit, const std::function<void (Stream* stream)>& thenCb,
        const std::function<void (Stream* stream)>& elseCb) {
    if (!this->active()) return this;

    const std::string str = dequeToString(this->chars, (unsigned long) limit);

    // Match the regular expression and invoke the callback if a match is found.
    if (std::regex_search(str, matcher)) {
        thenCb(this);
    } else {
        elseCb(this);
    }

    return this;
}

Stream* Stream::repeat(const std::regex& matcher, const int limit, const std::function<void (Stream*)>& callback,
        const std::experimental::optional<const std::string>& eofError) {
    if (!this->active()) return this;

    bool matched;
    do {
        const std::string str = dequeToString(this->chars, (unsigned long) limit);
        matched = std::regex_search(str, matcher);
        if (matched) callback(this);
    } while (matched && this->active());

    if (this->chars.empty() && eofError) {
        this->throwException(eofError.value());
    }

    return this;
}

void Stream::returnToken(const std::function<TokenBuilder (const std::string& source)>& tokenProducer) {
    this->result = tokenProducer(dequeToString(this->buffer))
        .setLine(this->line)
        .setStartCol(this->startCol)
        .setEndCol(this->currentCol)
    .build();
    this->buffer = std::deque<char>();

    this->startCol = this->currentCol;
}

void Stream::returnToken() {
    this->returnToken([](const std::string& source) {
        return TokenBuilder(source);
    });
}

void Stream::throwException(const std::string& message) const {
    throw SyntaxException(message, this->line, this->startCol, this->currentCol);
}

std::shared_ptr<const Token> Stream::extractResult() {
    std::shared_ptr<const Token> token = this->result;

    // If no token to return and there is still data to process, then something has gone wrong.
    if (token == nullptr && (!this->buffer.empty() || !this->chars.empty())) {
        throw IllegalStateException("returnToken() not properly called before extractToken()");
    }

    this->result = nullptr;

    return token;
}

bool Stream::active() {
    // Skip processing if we already have a result, or if there are no characters left to analyze.
    return this->result == nullptr && !this->chars.empty();
}