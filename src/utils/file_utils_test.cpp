#include <iostream>
#include <gtest/gtest.h>
#include "file_utils.h"
#include "../models/exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;

TEST(FileUtils, ReadFileCharsSuccess) {
    const auto filename = "hello.txt";
    const std::string content = "Hello World!";
    std::ofstream stream(filename);
    stream << content;
    stream.close();

    auto queue = FileUtils::readFileChars(filename);
    for (const char c : content) {
        ASSERT_EQ(c, queue.front());
        queue.pop();
    }
}

TEST(FileUtils, ReadFileCharsFileNotFound) {
    ASSERT_THROW(FileUtils::readFileChars("does_not_exist.txt"), FileNotFoundException);
}