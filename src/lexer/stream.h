#ifndef SANITY_STREAM_H
#define SANITY_STREAM_H

#include <deque>
#include <queue>
#include <memory>
#include <regex>
#include <functional>
#include "../models/token.h"
#include "../models/token_builder.h"

class Stream {
private:
    std::deque<char> chars;
    std::deque<char> buffer;
    std::shared_ptr<const Token> result;
    int line = 1;
    int startCol = 1;
    int currentCol = 1;

    void advanceChars(int numChars = 1, bool updateStartColumn = false);

public:
    explicit Stream(std::queue<char>& chars);

    /**
     * Ignores the next amount of characters in the stream. If updateStartColumn is true, then the Stream will assume
     * that the values ignored are not associated with any Token, and the line numbers will be updated to skip over
     * these characters.
     */
    Stream* ignore(int numChars = 1, bool updateStartColumn = false);

    /**
     * Consumes the next amount of characters by including them in the next Token.
     */
    Stream* consume(int numChars = 1);

    /**
     * Consumes the characters for as a long as the regex matches up to the limit number of characters.
     */
    Stream* consumeWhile(const std::regex& matcher, int limit);

    /**
     * If the current state of the stream matches the given regex up to the limit number of characters, then the
     * callback is invoked.
     */
    Stream* match(const std::regex& matcher, int limit, std::function<void (Stream* stream)> callback);

    /**
     * As long as the current state of the stream matches the given regex up to the limit number of characters, then
     * the callback is invoked.
     */
    Stream* repeat(const std::regex& matcher, int limit, std::function<void (Stream*)> callback);

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
    void throwException(const std::string& message);

    /**
     * Takes the token previously saved by returnToken() and returns it while resetting the current state of the stream
     * to continue processing the input.
     * @see #returnToken()
     */
    std::shared_ptr<const Token> extractResult();

private:
    /**
     * Returns whether or not this Stream is active and able to be lexically analyzed. The stream is not active if
     * returnToken() has been called, but the Token has not yet been extracted by extractResult(). The stream is also
     * not active if all input characters have been read and processed.
     */
    bool active();
};

#endif //SANITY_STREAM_H