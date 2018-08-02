from test_utils import run_test_case, STDLIB

PROG_SUB = """
extern printf: (string) -> int;
extern stringify: (int) -> string;
extern readInt: () -> int;

printf(stringify(readInt() - readInt()));
"""


def test_sub1():
    status, stdout, stderr = run_test_case(
        source=PROG_SUB,
        deps=STDLIB,
        stdin="3\n2",
    )

    assert status == 0
    assert stdout == "1"
    assert stderr == ""


def test_sub2():
    status, stdout, stderr = run_test_case(
        source=PROG_SUB,
        deps=STDLIB,
        stdin="5\n0",
    )

    assert status == 0
    assert stdout == "5"
    assert stderr == ""


def test_sub3():
    status, stdout, stderr = run_test_case(
        source=PROG_SUB,
        deps=STDLIB,
        stdin="0\n0",
    )

    assert status == 0
    assert stdout == "0"
    assert stderr == ""


def test_sub4():
    status, stdout, stderr = run_test_case(
        source=PROG_SUB,
        deps=STDLIB,
        stdin="-5\n-3",
    )

    assert status == 0
    assert stdout == "-2"
    assert stderr == ""


def test_sub5():
    status, stdout, stderr = run_test_case(
        source=PROG_SUB,
        deps=STDLIB,
        stdin="5\n2",
    )

    assert status == 0
    assert stdout == "3"
    assert stderr == ""
