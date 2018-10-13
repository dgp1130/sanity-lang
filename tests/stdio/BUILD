load("//build_defs:sanity.bzl", "sanity_binary")
load("//tests:tester.bzl", "test_sanity_prog")

sanity_binary(
    name = "cat",
    src = "cat.sane",
)

test_sanity_prog(
    name = "cat_test",
    binary = ":cat",
    expected_stdout = "Hello World!",
)

sanity_binary(
    name = "printf",
    src = "printf.sane",
)

test_sanity_prog(
    name = "printf_test",
    binary = ":printf",
    expected_stdout = "Hello World!",
)

sanity_binary(
    name = "putchar",
    src = "putchar.sane",
)

test_sanity_prog(
    name = "putchar_test",
    binary = ":putchar",
    expected_stdout = "Hello",
)

sanity_binary(
    name = "puts",
    src = "puts.sane",
)

test_sanity_prog(
    name = "puts_test",
    binary = ":puts",
    expected_stdout = "Hello World!\n",
)
