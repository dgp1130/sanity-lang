#include "lexer.h"
#include "stream.h"
#include "compiler/models/token.h"
#include "compiler/models/token_builder.h"
#include "compiler/models/exceptions.h"
#include <memory>
#include <queue>
#include <regex>

typedef Exceptions::AssertionException AssertionException;
typedef Exceptions::SyntaxException SyntaxException;

// Returns the given control char (the one that comes after the backslash, so the "n" in "\n") as its escaped value.
char escapeCharacter(const char controlChar, const Stream* stream) {
    switch (controlChar) {
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case '\'': return '\'';
        case '\"': return '\"';
        case '\\': return '\\';
        default: stream->throwException("Unexpected escape character: \\" + std::string(1, controlChar));
    }

    throw AssertionException("Unreachable statement");
}

std::queue<std::shared_ptr<const Token>> Lexer::tokenize(std::queue<char>& chars) {
    auto tokens = std::queue<std::shared_ptr<const Token>>();

    auto stream = std::make_unique<Stream>(Stream(chars));
    std::experimental::optional<std::shared_ptr<const Token>> token;
    do {
        token = stream->repeatWhile(std::regex("^([ \t\n\r]|//|/\\*)"), 2, [](Stream* stream) {
            stream->repeatWhile(std::regex("^[ \t\n\r]"), 1, [](Stream* stream) {
                stream->ignore(1 /* whitespace */, true /* updateStartColumn */);
            })->match(std::regex("^//"), 2, [](Stream* stream) {
                stream->ignore(2 /* slashes */, true /* updateStartColumn */);
                stream->ignoreUntil(std::regex("^\\n"), 1, true /* updateStartColumn */);
                stream->ignore(1 /* newline */, true /* updateStartColumn */);
            })->match(std::regex("^/\\*"), 2, [](Stream* stream) {
                stream->ignore(2 /* open comment */, true /* updateStartColumn */);
                stream->ignoreUntil(std::regex("^\\*/"), 2, true /* updateStartColumn */,
                        std::string("Unexpected EOF in block comment."));
                stream->ignore(2 /* close comment */, true /* updateStartColumn */);
            });
        })->match(std::regex("^[a-zA-Z_]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[a-zA-Z0-9_]"), 1)->returnToken();
        })->match(std::regex("^[0-9]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[0-9]"), 1)->returnToken([](const std::string& source) {
                return TokenBuilder(source).setIntegerLiteral(true);
            });
        })->match(std::regex("^\""), 1, [](Stream* stream) { // Then
            stream->ignore(/* open double quote */)->repeatUntil(std::regex("^[\"\n\t\r]"), 1, [](Stream* stream) {
                stream->match(std::regex("^\\\\"), 1, [](Stream* stream) { // Then, escaped char
                    // Ignore in a separate statement so it occurs before the stream->front() call.
                    stream->ignore(/* backslash */);
                    stream->consume(escapeCharacter(stream->front(), stream))->ignore(/* control character */);
                }, [](Stream* stream) { // Else, non-escaped character
                    stream->match(std::regex("^\'"), 1, [](Stream* stream) { // Then, single quote in string
                        stream->throwException(R"(Cannot use ' in a string literal, use \' instead.)");
                    }, [](Stream* stream) { // Else, non single quote character
                        stream->consume(/* char */);
                    });
                });
            }, std::string("Unexpected EOF"))->match(std::regex("^\""), 1, [](Stream* stream) { // Then
                stream->ignore(/* close double quote */)->returnToken([](const std::string& source) {
                    return TokenBuilder(source).setStringLiteral(true);
                });
            }, [](Stream* stream) { // Else
                stream->throwException("Illegal character in string.");
            });
        })->match(std::regex("^\'"), 1, [](Stream* stream) {
            stream->ignore(/* open quote */)->match(std::regex("^\\\\"), 1, [](Stream* stream) { // Then, escaped char
                // Ignore in a separate statement so it occurs before the stream->front() call.
                stream->ignore(/* backslash */);
                stream->consume(escapeCharacter(stream->front(), stream))->ignore(/* control character */);
            }, [](Stream* stream) { // Else, non-escaped character
                if (std::regex_match(std::string(1, stream->front()), std::regex("[\n\t\r\'\"]"))) {
                    stream->throwException("Invalid character in character literal");
                } else {
                    stream->consume(/* char */);
                }
            })->match(std::regex("^\'"), 1, [](Stream* stream) { // Then
                stream->ignore(/* close quote */)->returnToken([](const std::string& source) {
                    return TokenBuilder(source).setCharLiteral(true);
                });
            }, [](Stream* stream) { // Else
                stream->throwException("Expected character literal to end with a closing single quote.");
            });
        })->match(std::regex("^->"), 2, [](Stream* stream) {
            stream->consume(2 /* -> */)->returnToken();
        })->match(std::regex("^[^a-zA-Z0-9]"), 1, [](Stream* stream) {
            stream->consume(/* punctuation */)->returnToken();
        })->extractResult();

        if (token) tokens.push(token.value());
    } while (token);

    return tokens;
}