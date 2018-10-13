load("//build_defs:sanity.bzl", "sanity_binary")
load("//tests:tester.bzl", "test_sanity_prog")

sanity_binary(
    name = "add",
    src = "add.sane",
    deps = [
        "//stdlib:input",
        "//stdlib:stringify",
    ],
)

test_sanity_prog(
    name = "add_test_positives",
    binary = ":add",
    expected_stdout = "3",
    provided_stdin = "1\n2",
)

test_sanity_prog(
    name = "add_test_single_zero",
    binary = ":add",
    expected_stdout = "5",
    provided_stdin = "0\n5",
)

test_sanity_prog(
    name = "add_test_double_zero",
    binary = ":add",
    expected_stdout = "0",
    provided_stdin = "0\n0",
)

test_sanity_prog(
    name = "add_test_negatives",
    binary = ":add",
    expected_stdout = "-8",
    provided_stdin = "-5\n-3",
)

test_sanity_prog(
    name = "add_test_mixed_sign",
    binary = ":add",
    expected_stdout = "-3",
    provided_stdin = "-5\n2",
)

sanity_binary(
    name = "sub",
    src = "sub.sane",
    deps = [
        "//stdlib:input",
        "//stdlib:stringify",
    ],
)

test_sanity_prog(
    name = "sub_test_positives",
    binary = ":sub",
    expected_stdout = "1",
    provided_stdin = "3\n2",
)

test_sanity_prog(
    name = "sub_test_single_zero",
    binary = ":sub",
    expected_stdout = "5",
    provided_stdin = "5\n0",
)

test_sanity_prog(
    name = "sub_test_double_zero",
    binary = ":sub",
    expected_stdout = "0",
    provided_stdin = "0\n0",
)

test_sanity_prog(
    name = "sub_test_negatives",
    binary = ":sub",
    expected_stdout = "-2",
    provided_stdin = "-5\n-3",
)

test_sanity_prog(
    name = "sub_test_mixed_sign",
    binary = ":sub",
    expected_stdout = "-5",
    provided_stdin = "-3\n2",
)

sanity_binary(
    name = "mul",
    src = "mul.sane",
    deps = [
        "//stdlib:input",
        "//stdlib:stringify",
    ],
)

test_sanity_prog(
    name = "mul_test_positives",
    binary = ":mul",
    expected_stdout = "6",
    provided_stdin = "3\n2",
)

test_sanity_prog(
    name = "mul_test_single_zero",
    binary = ":mul",
    expected_stdout = "0",
    provided_stdin = "5\n0",
)

test_sanity_prog(
    name = "mul_test_double_zero",
    binary = ":mul",
    expected_stdout = "0",
    provided_stdin = "0\n0",
)

test_sanity_prog(
    name = "mul_test_negatives",
    binary = ":mul",
    expected_stdout = "15",
    provided_stdin = "-5\n-3",
)

test_sanity_prog(
    name = "mul_test_mixed_sign",
    binary = ":mul",
    expected_stdout = "-10",
    provided_stdin = "-5\n2",
)

sanity_binary(
    name = "div",
    src = "div.sane",
    deps = [
        "//stdlib:input",
        "//stdlib:stringify",
    ],
)

test_sanity_prog(
    name = "div_test_positives",
    binary = ":div",
    expected_stdout = "2",
    provided_stdin = "4\n2",
)

test_sanity_prog(
    name = "div_test_zero_dividend",
    binary = ":div",
    expected_stdout = "0",
    provided_stdin = "0\n5",
)

test_sanity_prog(
    name = "div_test_dividend_equals_divisor",
    binary = ":div",
    expected_stdout = "1",
    provided_stdin = "5\n5",
)

test_sanity_prog(
    name = "div_test_negatives",
    binary = ":div",
    expected_stdout = "5",
    provided_stdin = "-5\n-1",
)

test_sanity_prog(
    name = "div_test_remainder",
    binary = ":div",
    expected_stdout = "2",
    provided_stdin = "5\n2",
)

test_sanity_prog(
    name = "div_test_divide_by_zero",
    binary = ":div",
    expected_status = -8,  # Happens to exit with -8, not explicitly defined by Sanity
    provided_stdin = "5\n0",
)
