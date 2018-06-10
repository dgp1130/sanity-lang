#ifndef SANITY_QUEUE_UTILS_H
#define SANITY_QUEUE_UTILS_H

#include <string>
#include <queue>

namespace QueueUtils {
    std::queue<char> queueify(const std::string& str);
};

#endif //SANITY_QUEUE_UTILS_H