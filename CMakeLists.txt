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
add_library(exceptions models/exceptions.cpp models/exceptions.h)
add_library(file_utils utils/file_utils.cpp utils/file_utils.h)
add_library(lexer lexer/lexer.cpp lexer/lexer.h)
add_library(stream lexer/stream.cpp lexer/stream.h)
add_library(token models/token.cpp models/token.h)
add_library(token_builder models/token_builder.cpp models/token_builder.h)
add_library(queue_utils utils/queue_utils.cpp utils/queue_utils.h)
set(SANITY_LIBS exceptions file_utils lexer stream token token_builder queue_utils)

# Build the Sanity executable and link it against its dependencies.
add_executable(Sanity main.cpp)
target_link_libraries(Sanity ${SANITY_LIBS})

# Build the Sanity test executable with all test files and link it against its dependencies.
add_executable(
    SanityTest
    lexer/lexer_test.cpp
    lexer/stream_test.cpp
    models/token_test.cpp
    models/exceptions_test.cpp
    utils/file_utils_test.cpp
    utils/queue_utils_test.cpp
)
target_link_libraries(SanityTest gtest gtest_main ${SANITY_LIBS})