# Generates LLVM IR for Sanity source code.

package(default_visibility = ["//compiler:__subpackages__"])

cc_library(
    name = "generator",
    srcs = ["generator.cpp"],
    hdrs = ["generator.h"],
    deps = [
        "//compiler/models:ast",
        "//compiler/models:exceptions",
        "//compiler/models:globals",
        "@llvm",
    ],
)

cc_test(
    name = "generator_test",
    srcs = ["generator_test.cpp"],
    deps = [
        ":generator",
        "//compiler/models:globals",
        "//compiler/models:token_builder",
        "@gtest//:gtest_main",
        "@llvm",
    ],
)