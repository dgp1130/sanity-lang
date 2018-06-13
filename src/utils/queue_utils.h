#ifndef SANITY_QUEUE_UTILS_H
#define SANITY_QUEUE_UTILS_H

#include <string>
#include <queue>
#include "../models/token.h"

namespace QueueUtils {
    std::queue<char> queueify(const std::string& str);

    std::queue<const Token*> queueify(const Token* tokens[], int length);
};

#endif //SANITY_QUEUE_UTILS_H