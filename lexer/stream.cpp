#include "stream.h"
#include <deque>
#include <queue>
#include <regex>
#include <functional>
#include "../models/token.h"
#include "../models/exceptions.h"

typedef Exceptions::IllegalStateException IllegalStateException;

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

Stream* Stream::ignore(const int numChars) {
    if (!this->active()) return this;

    for (int i = 0; i < numChars; ++i) {
        this->chars.pop_front();
    }

    return this;
}

Stream* Stream::consume(const int numChars) {
    if (!this->active()) return this;

    for (int i = 0; i < numChars; ++i) {
        buffer.push_back(this->chars.front());
        this->chars.pop_front();
    }

    return this;
}

Stream* Stream::consumeWhile(const std::regex matcher, const int limit) {
    if (!this->active()) return this;

    this->repeat(matcher, limit, [](Stream* stream) {
        stream->consume();
    });

    return this;
}

Stream* Stream::match(const std::regex matcher, const int limit, const std::function<void (Stream*)> callback) {
    if (!this->active()) return this;

    const std::string str = dequeToString(this->chars, (unsigned long) limit);

    // Match the regular expression and invoke the callback if a match is found.
    if (std::regex_search(str, matcher)) {
        callback(this);
    }

    return this;
}

Stream* Stream::repeat(const std::regex matcher, const int limit, const std::function<void(Stream*)> callback) {
    if (!this->active()) return this;

    bool matched;
    do {
        const std::string str = dequeToString(this->chars, (unsigned long) limit);
        matched = std::regex_search(str, matcher);
        if (matched) callback(this);
    } while (matched && this->active());

    return this;
}

void Stream::returnToken() {
    this->result = new Token(dequeToString(this->buffer));
    this->buffer = std::deque<char>();
}

const Token* Stream::extractResult() {
    const Token* token = this->result;

    // If no token to return and there is still data to process, then something has gone wrong.
    if (token == nullptr && !this->chars.empty()) {
        throw IllegalStateException("returnToken() not properly called before extractToken()");
    }

    this->result = nullptr;

    return token;
}

bool Stream::active() {
    // Skip processing if we already have a result, or if there are no characters left to analyze.
    return this->result == nullptr && !this->chars.empty();
}