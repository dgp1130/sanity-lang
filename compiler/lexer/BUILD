# Performs lexical analysis for Sanity.

package(default_visibility = ["//compiler:__subpackages__"])

cc_library(
    name = "lexer",
    srcs = ["lexer.cpp"],
    hdrs = ["lexer.h"],
    copts = ["--std=c++1y"], # For experimental
    deps = [
        ":stream",
        "//compiler/models:exceptions",
        "//compiler/models:token",
        "//compiler/models:token_builder",
    ],
)

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cpp"],
    deps = [
        ":lexer",
        "//compiler/utils:queue",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "stream",
    srcs = ["stream.cpp"],
    hdrs = ["stream.h"],
    copts = ["--std=c++1y"], # For experimental
    deps = [
        "//compiler/models:exceptions",
        "//compiler/models:token_builder",
    ],
)

cc_test(
    name = "stream_test",
    srcs = ["stream_test.cpp"],
    copts = ["--std=c++1y"], # For experimental
    deps = [
        ":stream",
        "//compiler/utils:queue",
        "@gtest//:gtest_main",
    ],
)