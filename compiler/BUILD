package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "compiler",
    srcs = ["main.cpp"],
    deps = [
        "//compiler/generator",
        "//compiler/lexer",
        "//compiler/models:globals",
        "//compiler/parser",
        "//compiler/utils:file",
        "@gflags",
        "@llvm",
    ],
)
