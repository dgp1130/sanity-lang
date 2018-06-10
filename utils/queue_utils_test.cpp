#include <gtest/gtest.h>
#include <queue>
#include "queue_utils.h"

TEST(QueueUtils, QueueifiesStrings) {
    std::queue<char> q = QueueUtils::queueify("abc");

    ASSERT_EQ(3, q.size());

    ASSERT_EQ('a', q.front());
    q.pop();
    ASSERT_EQ('b', q.front());
    q.pop();
    ASSERT_EQ('c', q.front());
    q.pop();
}