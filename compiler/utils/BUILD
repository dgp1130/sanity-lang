# Set of utility modules for use throughout the codebase.

package(default_visibility = ["//compiler:__subpackages__"])

cc_library(
    name = "file",
    srcs = ["file_utils.cpp"],
    hdrs = ["file_utils.h"],
    deps = ["//compiler/models:exceptions"],
)

cc_test(
    name = "file_test",
    srcs = ["file_utils_test.cpp"],
    deps = [
        ":file",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "queue",
    srcs = ["queue_utils.cpp"],
    hdrs = ["queue_utils.h"],
    deps = ["//compiler/models:token"],
)

cc_test(
    name = "queue_test",
    srcs = ["queue_utils_test.cpp"],
    deps = [
        ":queue",
        "@gtest//:gtest_main",
    ],
)
