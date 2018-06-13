#include "queue_utils.h"
#include "../models/token.h"
#include <memory>
#include <string>
#include <queue>

std::queue<char> QueueUtils::queueify(const std::string& str) {
    std::queue<char> q;

    for (auto c : str) {
        q.push(c);
    }

    return q;
}

std::queue<std::shared_ptr<const Token>> QueueUtils::queueify(std::shared_ptr<const Token> tokens[], int length) {
    std::queue<std::shared_ptr<const Token>> q;

    for (int i = 0; i < length; ++i) {
        q.push(tokens[i]);
    }

    return q;
}