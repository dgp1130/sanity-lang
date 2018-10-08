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