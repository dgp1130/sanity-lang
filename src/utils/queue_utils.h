#ifndef SANITY_QUEUE_UTILS_H
#define SANITY_QUEUE_UTILS_H

#include <memory>
#include <string>
#include <queue>
#include "../models/token.h"

namespace QueueUtils {
    std::queue<char> queueify(const std::string& str);

    std::queue<std::shared_ptr<const Token>> queueify(std::shared_ptr<const Token> tokens[], int length);
};

#endif //SANITY_QUEUE_UTILS_H