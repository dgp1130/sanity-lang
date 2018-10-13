"""Skylark build definitions for end to end testing a Sanity program."""

load("//:build_vars.bzl", "WORKSPACE_NAME")
load("//build_defs:sanity.bzl", "sanity_binary")

def test_sanity_prog(
        name,
        binary,
        provided_stdin = "",
        expected_status = 0,
        expected_stdout = "",
        expected_stderr = ""):
    """Tests that the given Sanity program, when executed, prints the given stdout.

    Note that this rule is intentionally NOT called sanity_test() because this uses Sanity as the code-user-test while
    that convention would apply to tests written in Sanity.

    Outputs:
      <name>: A py_test() which compiles and runs the given Sanity source code and asserts the result against the
          provided values.

    Args:
      name: The name of the test to create.
      binary: The sanity_binary() program to test.
      provided_stdin: The string provided to stdin. Defaults to "".
      expected_status: The status code expected for the Sanity program to exit with. Defaults to 0.
      expected_stdout: The string expected to be printed to stdout. Defaults to "". Watch out for trailing newlines!
      expected_stderr: The string expected to be printed to stderr. Defaults to "". Watch out for trailing newlines!
    """

    # Generate a test to verify the provided expectations.
    native.py_test(
        name = name,
        srcs = ["//tests:tester.py"],
        main = "tester.py",
        data = [binary],
        args = [
            binary.split(":")[1],  # Compiled binary to invoke
            # Wrap strings in proper arguments to correctly handle spaces and newlines.
            "--provided_stdin=\"%s\"" % provided_stdin,
            "--expected_status=\"%s\"" % expected_status,
            "--expected_stdout=\"%s\"" % expected_stdout,
            "--expected_stderr=\"%s\"" % expected_stderr,
        ],
    )
