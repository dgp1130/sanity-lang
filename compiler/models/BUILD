# Set of model data for use throughout the codebase.

package(default_visibility = ["//compiler:__subpackages__"])

cc_library(
    name = "ast",
    srcs = ["ast.cpp"],
    hdrs = ["ast.h"],
    deps = [
        ":exceptions",
        ":globals",
        ":token",
        "@llvm",
    ],
)

cc_test(
    name = "ast_test",
    srcs = ["ast_test.cpp"],
    deps = [
        ":ast",
        ":globals",
        ":token_builder",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "exceptions",
    srcs = ["exceptions.cpp"],
    hdrs = ["exceptions.h"],
)

cc_test(
    name = "exceptions_test",
    srcs = ["exceptions_test.cpp"],
    deps = [
        ":exceptions",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "globals",
    hdrs = ["globals.h"],
    deps = ["@llvm"],
)

cc_library(
    name = "token",
    srcs = ["token.cpp"],
    hdrs = ["token.h"],
)

cc_library(
    name = "token_builder",
    srcs = ["token_builder.cpp"],
    hdrs = ["token_builder.h"],
    deps = [":token"],
)

cc_test(
    name = "token_test",
    srcs = ["token_test.cpp"],
    deps = [
        ":token_builder",
        "@gtest//:gtest_main",
    ],
)