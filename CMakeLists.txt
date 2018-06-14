cmake_minimum_required(VERSION 3.10)
project(Sanity)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# See https://www.boost.org/doc/libs/1_67_0/more/getting_started/unix-variants.html to install
include_directories(/usr/local/boost_1_67_0)

# GoogleTest build system hacked from https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time by invoking CMake on the other configuration file.
configure_file(CMakeLists_googletest.txt ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download
)

# Verify googletest downloaded successfully.
if(result)
    message(FATAL_ERROR "Download step for googletest failed: ${result}")
endif()

# Build googletest.
execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download
)

# Verify googletest built successfully.
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to Sanity's build. This defines the gtest and gtest_main targets.
add_subdirectory(
    ${CMAKE_BINARY_DIR}/googletest-src
    ${CMAKE_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL
)

# Include the header files to link against.
include_directories("${gtest_SOURCE_DIR}/include")

# Compile each Sanity source file on its own as a library.
add_library(ast src/models/ast.cpp src/models/ast.h)
target_link_libraries(ast token)

add_library(ast_test src/models/ast_test.cpp)
target_link_libraries(ast_test ast token token_builder)

add_library(exceptions src/models/exceptions.cpp src/models/exceptions.h)

add_library(exceptions_test src/models/exceptions_test.cpp)
target_link_libraries(exceptions_test exceptions)

add_library(file_utils src/utils/file_utils.cpp src/utils/file_utils.h)
target_link_libraries(file_utils exceptions)

add_library(file_utils_test src/utils/file_utils_test.cpp)
target_link_libraries(file_utils_test file_utils)

add_library(lexer src/lexer/lexer.cpp src/lexer/lexer.h)
target_link_libraries(lexer exceptions stream token token_builder)

add_library(lexer_test src/lexer/lexer_test.cpp)
target_link_libraries(lexer_test lexer queue_utils)

add_library(parser src/parser/parser.cpp src/parser/parser.h)
target_link_libraries(parser ast exceptions token)

add_library(parser_test src/parser/parser.cpp)
target_link_libraries(parser_test ast exceptions parser token token_builder queue_utils)

add_library(stream src/lexer/stream.cpp src/lexer/stream.h)
target_link_libraries(stream token)

add_library(stream_test src/lexer/stream_test.cpp)
target_link_libraries(stream_test stream)

add_library(token src/models/token.cpp src/models/token.h)

add_library(token_test src/models/token_test.cpp)
target_link_libraries(token_test token)

add_library(token_builder src/models/token_builder.cpp src/models/token_builder.h)
target_link_libraries(token_builder exceptions token)

add_library(queue_utils src/utils/queue_utils.cpp src/utils/queue_utils.h)
target_link_libraries(queue_utils token)

add_library(queue_utils_test src/utils/queue_utils_test.cpp)
target_link_libraries(queue_utils_test queue_utils)

# Build the Sanity executable and link it against its dependencies.
add_executable(Sanity src/main.cpp)
target_link_libraries(Sanity ast exceptions file_utils lexer parser)

# Build the Sanity test executable with all test files and link it against its dependencies.
add_executable(SanityTest src/test_runner.cpp)
target_link_libraries(
    SanityTest
    gtest
    gtest_main
    ast_test
    exceptions_test
    file_utils_test
    lexer_test
    parser_test
    stream_test
    token_test
    queue_utils_test
)