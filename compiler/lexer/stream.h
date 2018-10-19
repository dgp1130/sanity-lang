#ifndef SANITY_STREAM_H
#define SANITY_STREAM_H

#include <deque>
#include <queue>
#include <memory>
#include <regex>
#include <functional>
#include <experimental/optional>
#include "../models/token.h"
#include "../models/token_builder.h"

class Stream {
private:
    std::deque<char> chars;
    std::deque<char> buffer;
    std::experimental::optional<std::shared_ptr<const Token>> result;
    int line = 1;
    int startCol = 1;
    int currentCol = 1;

    void advanceChars(int numChars = 1, bool updateStartColumn = false);

public:
    explicit Stream(std::queue<char>& chars);

    /**
     * Returns the first character in the sequence.
     */
    char front();

    /**
     * Ignores the next amount of characters in the stream. If updateStartColumn is true, then the Stream will assume
     * that the values ignored are not associated with any Token, and the line numbers will be updated to skip over
     * these characters.
     */
    Stream* ignore(int numChars = 1, bool updateStartColumn = false);

    /**
     * Ignores the characters for as long as the regex matches up to the limit number of characters. If
     * updateStartColumn is true, then the Stream will assume that the values are ignored and not associated with any
     * Token, and the line numbers will be updated to skip over these characters. If an eofError is provided, and the
     * repeat finds that the end of the file is reached, then it will throw an error with that message.
     */
    Stream* ignoreWhile(const std::regex& matcher, int limit, bool updateStartColumn = false,
            const std::experimental::optional<const std::string>& eofError = std::experimental::nullopt);

    /**
     * Ignores the characters until the regex matches up to the limit number of characters. If updateStartColumn is
     * true, then the Stream will assume that the values are ignored and not associated with any Token, and the line
     * numbers will be updated to skip over these characters. If an eofError is provided, and the repeat finds that the
     * end of the file is reached, then it will throw an error with that message.
     */
    Stream* ignoreUntil(const std::regex& matcher, int limit, bool updateStartColumn = false,
            const std::experimental::optional<const std::string>& eofError = std::experimental::nullopt);

    /**
     * Consumes the next amount of characters by including them in the next Token.
     */
    Stream* consume(int numChars = 1);

    /**
     * Consume the provided character by including it in the next Token. Does not advance the Stream.
     */
    Stream* consume(char character);

    /**
     * Consumes the characters for as a long as the regex matches up to the limit number of characters. If eofError is
     * provided and the end of file is detected, then an error is thrown with the given message.
     */
    Stream* consumeWhile(const std::regex& matcher, int limit,
            const std::experimental::optional<const std::string>& eofError = std::experimental::nullopt);

    /**
     * If the current state of the Stream matches the given regex up to the limit number of characters, then the
     * callback is invoked.
     */
    Stream* match(const std::regex& matcher, int limit, const std::function<void (Stream* stream)>& callback);

    /**
     * If the current state of the Stream matches the given regex up to the limit number of characters, then the
     * thenCb is invoked, otherwise the elseCb is invoked.
     */
    Stream* match(const std::regex& matcher, int limit, const std::function<void (Stream* stream)>& thenCb,
        const std::function<void (Stream* stream)>& elseCb);

    /**
     * As long as the current state of the stream matches the given regex up to the limit number of characters, then
     * the callback is invoked. If an eofError is provided, and the repeat finds that the end of the file is reached,
     * then it will throw an error with that message.
     */
    Stream* repeatWhile(const std::regex& matcher, int limit, const std::function<void (Stream*)>& callback,
            const std::experimental::optional<const std::string>& eofError = std::experimental::nullopt);

    /**
     * Until the current state of the stream matches the given regex up to the limit number of characters, then the
     * callback is invoked. If an eofError is provided, and the repeat finds that the end of the file is reached, then
     * it will throw an error with that message.
     */
    Stream* repeatUntil(const std::regex& matcher, int limit, const std::function<void (Stream*)>& callback,
            const std::experimental::optional<const std::string>& eofError = std::experimental::nullopt);

    /**
     * Saves the current state of the buffer and will return a Token as provided by the producer of this state when
     * extractResult() is called. All other functionality is blocked until the Token is extracted.
     * @see #extractResult()
     */
    void returnToken(const std::function<TokenBuilder (const std::string& source)>& tokenProducer);

    /**
     * Saves the current state of the buffer and will return a standard Token with no special options of this state when
     * extractResult() is called. All other functionality is blocked until the Token is extracted.
     * @see #extractResult()
     */
    void returnToken();

    /**
     * Throw an exception with the given message. Automatically attaches the current line and col numbers to it.
     * @throws SyntaxException
     */
    void throwException(const std::string& message) const;

    /**
     * Takes the token previously saved by returnToken() and returns it while resetting the current state of the stream
     * to continue processing the input. Returns std::experimental::nullopt if the input has reached EOF.
     * @see #returnToken()
     */
    std::experimental::optional<std::shared_ptr<const Token>> extractResult();

private:
    /**
     * Returns whether or not this Stream is active and able to be lexically analyzed. The stream is not active if
     * returnToken() has been called, but the Token has not yet been extracted by extractResult(). The stream is also
     * not active if all input characters have been read and processed.
     */
    bool active();

    Stream* repeat(const std::function<bool (const std::string&)>& condition, int limit,
            const std::function<void (Stream* stream)>& callback,
            const std::experimental::optional<const std::string>& eofError = std::experimental::nullopt);
};

#endif //SANITY_STREAM_H