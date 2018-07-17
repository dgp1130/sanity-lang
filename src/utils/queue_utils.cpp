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

std::queue<std::shared_ptr<const Token>> QueueUtils::queueify(const std::vector<std::shared_ptr<const Token>>& tokens) {
    std::queue<std::shared_ptr<const Token>> q;

    for (const auto& token : tokens) {
        q.push(token);
    }

    return q;
}