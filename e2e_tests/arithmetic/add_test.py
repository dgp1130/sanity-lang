from test_utils import run_test_case, STDLIB

PROG_ADD = """
extern printf: (string) -> int;
extern stringify: (int) -> string;
extern readInt: () -> int;

printf(stringify(readInt() + readInt()));
"""


def test_add1():
    status, stdout, stderr = run_test_case(
        source=PROG_ADD,
        deps=STDLIB,
        stdin="1\n2",
    )

    assert status == 0
    assert stdout == "3"
    assert stderr == ""


def test_add2():
    status, stdout, stderr = run_test_case(
        source=PROG_ADD,
        deps=STDLIB,
        stdin="0\n5",
    )

    assert status == 0
    assert stdout == "5"
    assert stderr == ""


def test_add3():
    status, stdout, stderr = run_test_case(
        source=PROG_ADD,
        deps=STDLIB,
        stdin="0\n0",
    )

    assert status == 0
    assert stdout == "0"
    assert stderr == ""


def test_add4():
    status, stdout, stderr = run_test_case(
        source=PROG_ADD,
        deps=STDLIB,
        stdin="-5\n-3",
    )

    assert status == 0
    assert stdout == "-8"
    assert stderr == ""


def test_add5():
    status, stdout, stderr = run_test_case(
        source=PROG_ADD,
        deps=STDLIB,
        stdin="-5\n2",
    )

    assert status == 0
    assert stdout == "-3"
    assert stderr == ""
