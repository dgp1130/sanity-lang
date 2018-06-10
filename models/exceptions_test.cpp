#include <gtest/gtest.h>
#include "exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;

TEST(Exceptions, FileNotFoundExceptionExists) {
    FileNotFoundException ex("file.txt");
    SUCCEED(); // If this compiles and executes, then we're good.
}