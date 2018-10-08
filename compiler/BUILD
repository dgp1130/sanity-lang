package(default_visibility = ["//:__pkg__"])

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