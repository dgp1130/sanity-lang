cmake_minimum_required(VERSION 3.10)
project(Sanity)

set(CMAKE_CXX_STANDARD 17)

add_executable(Sanity main.cpp utils/file_utils.cpp utils/file_utils.h lexer/lexer.cpp lexer/lexer.h models/token.cpp models/token.h models/exceptions.cpp models/exceptions.h)

# See https://www.boost.org/doc/libs/1_67_0/more/getting_started/unix-variants.html to install
include_directories(/usr/local/boost_1_67_0)