#include <gtest/gtest.h>

#include "lexer/lexer_test.cpp"
#include "lexer/stream_test.cpp"
#include "models/ast_test.cpp"
#include "models/exceptions_test.cpp"
#include "models/token_test.cpp"
#include "parser/parser_test.cpp"
#include "utils/file_utils_test.cpp"
#include "utils/queue_utils_test.cpp"

/**
 * Starts the GoogleTest runner by executing all included tests.
 */
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}