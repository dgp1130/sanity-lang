#include "lexer.h"
#include "stream.h"
#include "../models/token.h"
#include <queue>
#include <regex>

std::queue<const Token*> Lexer::tokenize(std::queue<char>& chars) {
    auto tokens = std::queue<const Token*>();

    auto stream = new Stream(chars);
    const Token* token;
    do {
        token = stream->repeat(std::regex("^[ \t\n\r]"), 1, [](Stream* stream) {
            stream->ignore();
        })->match(std::regex("^[a-zA-Z_]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[a-zA-Z0-9_]"), 1)->returnToken();
        })->match(std::regex("^[0-9]"), 1, [](Stream* stream) {
            stream->consumeWhile(std::regex("^[0-9]"), 1)->returnToken();
        })->match(std::regex("^[^a-zA-Z0-9]"), 1, [](Stream* stream) {
            stream->consume()->returnToken();
        })->extractResult();
        if (token != nullptr) tokens.push(token);
    } while (token != nullptr);
    delete stream;

    return tokens;
}