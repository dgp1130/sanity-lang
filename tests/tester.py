"""Tests a Sanity binary by executing it and asserting the result.

This module is compiled with the expected parameter substituted in by the test_sanity_prog() BUILD macro. It is not
intended to be executed as is in source.
"""

from argparse import ArgumentParser
import os
from subprocess import Popen, PIPE
import sys

# Parse arguments
parser = ArgumentParser(description="Executes the given binary and asserts its outputs.")
parser.add_argument("binary", help="The binary-under-test to invoke.")
parser.add_argument("--provided_stdin", help="The string provided to stdin.", default="")
parser.add_argument("--expected_status", help="The status code expected on exit.", default=0, type=int)
parser.add_argument("--expected_stdout",
                    help="The string expected to be printed to stdout.",
                    default="")
parser.add_argument("--expected_stderr",
                    help="The string expected to be printed to stderr.",
                    default="")
args = parser.parse_args()


def get_binary_path(root_dir):
    """Returns the path to the binary under test within this directory.

    The binary under test could be at any location in the build hierarchy. However, only the Python environment and the
    binary should be there, so just look for a non-Python executable binary.
    """
    for root, dirs, files in os.walk(root_dir):
        for file in files:
            if file == args.binary:
                return os.path.join(root, file)


# Execute the binary in the runfiles directory of this test.
binary_path = get_binary_path(os.getcwd())
if not binary_path:
    sys.stderr.write("Could not find binary file.\n")
    sys.exit(1)
print("Executing: %s" % binary_path)
proc = Popen([binary_path], stdin=PIPE, stdout=PIPE, stderr=PIPE)
stdout, stderr = proc.communicate(args.provided_stdin.encode())

# Assert the result.
print("Expected exit status code: %s" % args.expected_status)
print("Actual exit status code: %s" % proc.returncode)
assert proc.returncode == args.expected_status

actual_stdout = stdout.decode("utf-8")
print("Expected stdout: %s" % args.expected_stdout)
print("Actual stdout: %s" % actual_stdout)
assert stdout.decode("utf-8") == args.expected_stdout

actual_stderr = stderr.decode("utf-8")
print("Expected stderr: %s" % args.expected_stderr)
print("Actual stderr: %s" % actual_stderr)
assert stderr.decode("utf-8") == args.expected_stderr
