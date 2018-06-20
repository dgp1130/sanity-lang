#include "lexer.h"
#include "stream.h"
#include "../models/token.h"
#include "../models/token_builder.h"
#include "../models/exceptions.h"
#include <memory>
#include <queue>
#include <regex>

typedef Exceptions::AssertionException AssertionException;
typedef Exceptions::SyntaxException SyntaxException;

// Given a 1-character normal string, it will return it unchanged as a character. If that one character is a literal
// which should be escaped, an error is thrown. Given a 2-character string with a leading backslash, it will convert
// that two character escape sequence into the one character it represents.
char escapeCharacter(const std::string& unescaped, const Stream* stream) {
    if (unescaped.size() > 2) throw AssertionException("Got more than two characters to escape: " + unescaped);
    if (unescaped.size() == 1 && unescaped[0] == '\\') {
        throw AssertionException("Got only a backslash to escape: " + unescaped);
    }

    // Nothing to escape, verify this isn't a weird character literal.
    if (unescaped.size() == 1) {
        const char character = unescaped[0];
        switch (character) {
            case '\n': stream->throwException(R"(Unexpected newline, use '\n' instead.)");
            case '\r': stream->throwException(R"(Unexpected carriage return, use '\r' instead.)");
            case '\t': stream->throwException(R"(Unexpected tab, use '\t' instead.)");
            case '\'': stream->throwException(R"(Unexpected single quote, use '\'' instead.)");
            case '\"': stream->throwException(R"(Unexpected double quote, use '\"' instead.)");
            case '\\': stream->throwException(R"(Unexpected backslash, use '\\' instead.)");
            default: return character;
        }
    }

    // Escape the character into the actual literal.
    const char controlChar = unescaped[1];
    switch (controlChar) {
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case '\'': return '\'';
        case '\"': return '\"';
        case '\\': return '\\';
        default: stream->throwException("Unexpected escape character: \\" + controlChar);
    }
}

std::queue<std::shared_ptr<const Token>> Lexer::tokenize(std::queue<char>& chars) {
    auto tokens = std::queue<std::shared_ptr<const Token>>();

    auto stream = std::make_unique<Stream>(Stream(chars));
    std::shared_ptr<const Token> token;
    do {
        token = stream->repeat(std::regex("^[ \t\n\r]"), 1, [](Stream* stream) {
            stream->ignore(1 /* space */, true /* updateStartLineNumbers */);
        })->match(std::regex("^[a-zA-Z_]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[a-zA-Z0-9_]"), 1)->returnToken();
        })->match(std::regex("^[0-9]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[0-9]"), 1)->returnToken();
        })->match(std::regex("^\'"), 1, [](Stream* stream) {
            stream->ignore(/* open quote */)->match(std::regex("^\\\\"), 1, [](Stream* stream) { // Then
                stream->consume(/* backslash */)->consume(/* control character */);
            }, [](Stream* stream) { // Else
                stream->consume(/* char */);
            })->match(std::regex("^\'"), 1, [](Stream* stream) { // Then
                stream->ignore(/* close quote */)->returnToken([&stream](const std::string& source) {
                    const char escapedSource = escapeCharacter(source, stream);
                    return TokenBuilder(std::string(1, escapedSource)).setCharLiteral(true);
                });
            }, [](Stream* stream) { // Else
                stream->throwException("Expected character to end with a close quote.");
            });
        })->match(std::regex("^->"), 2, [](Stream* stream) {
            stream->consume(2 /* -> */)->returnToken();
        })->match(std::regex("^[^a-zA-Z0-9]"), 1, [](Stream* stream) {
            stream->consume(/* punctuation */)->returnToken();
        })->extractResult();
        if (token != nullptr) tokens.push(token);
    } while (token != nullptr);

    return tokens;
}