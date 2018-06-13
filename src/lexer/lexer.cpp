#include "lexer.h"
#include "stream.h"
#include "../models/token.h"
#include "../models/token_builder.h"
#include "../models/exceptions.h"
#include <memory>
#include <queue>
#include <regex>

typedef Exceptions::SyntaxException SyntaxException;

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
            stream->ignore(/* open quote */)->consume(/* char */)
                ->match(std::regex("^[^\']"), 1, [](Stream* stream) {
                    stream->throwException("Expected character to end with a close quote.");
                })->match(std::regex("^\'"), 1, [](Stream* stream) {
                    stream->ignore(/* close quote */)->returnToken([](const std::string& source) {
                        return TokenBuilder(source).setCharLiteral(true);
                    });
                })
            ;
        })->match(std::regex("^[^a-zA-Z0-9]"), 1, [](Stream* stream) {
            stream->consume(/* punctuation */)->returnToken();
        })->extractResult();
        if (token != nullptr) tokens.push(token);
    } while (token != nullptr);

    return tokens;
}