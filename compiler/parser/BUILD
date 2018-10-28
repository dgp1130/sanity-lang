# Performs parsing for Sanity.

package(default_visibility = ["//compiler:__subpackages__"])

cc_library(
    name = "parser",
    srcs = ["parser.cpp"],
    hdrs = ["parser.h"],
    deps = [
        "//compiler/models:ast",
        "//compiler/models:exceptions",
        "//compiler/models:globals",
        "//compiler/models:token",
    ],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cpp"],
    deps = [
        ":parser",
        "//compiler/models:token_builder",
        "//compiler/utils:queue",
        "@gtest//:gtest_main",
    ],
)
