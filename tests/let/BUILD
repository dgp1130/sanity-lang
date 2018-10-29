load("//build_defs:sanity.bzl", "sanity_binary")
load("//tests:tester.bzl", "test_sanity_prog")

sanity_binary(
    name = "let_int",
    src = "let_int.sane",
)

test_sanity_prog(
    name = "let_int_test",
    binary = ":let_int",
    expected_stdout = "foo = 0",
)

sanity_binary(
    name = "let_multi",
    src = "let_multi.sane",
)

test_sanity_prog(
    name = "let_multi_test",
    binary = ":let_multi",
    expected_stdout = """
foo = 1;
bar = 3;
baz = 7;
""",
)

sanity_binary(
    name = "let_string",
    src = "let_string.sane",
)

test_sanity_prog(
    name = "let_string_test",
    binary = ":let_string",
    expected_stdout = "Hello",
)
