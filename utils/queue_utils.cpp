#include "queue_utils.h"
#include <string>
#include <queue>

std::queue<char> QueueUtils::queueify(const std::string& str) {
    std::queue<char> q;

    for (auto c : str) {
        q.push(c);
    }

    return q;
}