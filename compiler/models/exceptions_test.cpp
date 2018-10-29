#include <gtest/gtest.h>
#include "exceptions.h"

typedef Exceptions::FileNotFoundException FileNotFoundException;
typedef Exceptions::IllegalStateException IllegalStateException;
typedef Exceptions::ParseException ParseException;
typedef Exceptions::RedeclaredException RedeclaredException;
typedef Exceptions::SyntaxException SyntaxException;
typedef Exceptions::TypeException TypeException;
typedef Exceptions::UndeclaredException UndeclaredException;

TEST(Exceptions, FileNotFoundExceptionExists) {
    FileNotFoundException ex("file.txt");
    SUCCEED(); // If this compiles and executes, then we're good.
}

TEST(Exceptions, IllegalStateExceptionExists) {
    IllegalStateException("Bad state");
    SUCCEED(); // If this compiles and executes, then we're good.
}

TEST(Exceptions, ParseExceptionExists) {
    ParseException("Parsing error");
    SUCCEED(); // If this compiles and executes, then we're good.
}

TEST(Exceptions, RedeclaredExceptionExists) {
    RedeclaredException("Symbol redeclared");
    SUCCEED(); // If this compiles and executes, then we're good.
}

TEST(Exceptions, SyntaxExceptionExists) {
    SyntaxException("Syntax error", 1 /* line */, 2 /* startCol */, 3 /* endCol */);
    SUCCEED(); // If this compiles and executes, then we're good.
}

TEST(Exceptions, TypeException) {
    TypeException("Type error");
    SUCCEED(); // If this compiles and executes, then we're good.
}

TEST(Exceptions, UndeclaredException) {
    UndeclaredException("Undeclared symbol");
    SUCCEED(); // If this compiles and executes, then we're good.
}