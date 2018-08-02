from test_utils import run_test_case, STDLIB

PROG_MUL = """
extern printf: (string) -> int;
extern stringify: (int) -> string;
extern readInt: () -> int;

printf(stringify(readInt() * readInt()));
"""


def test_mul1():
    status, stdout, stderr = run_test_case(
        source=PROG_MUL,
        deps=STDLIB,
        stdin="3\n2",
    )

    assert status == 0
    assert stdout == "6"
    assert stderr == ""


def test_mul2():
    status, stdout, stderr = run_test_case(
        source=PROG_MUL,
        deps=STDLIB,
        stdin="5\n0",
    )

    assert status == 0
    assert stdout == "0"
    assert stderr == ""


def test_mul3():
    status, stdout, stderr = run_test_case(
        source=PROG_MUL,
        deps=STDLIB,
        stdin="0\n0",
    )

    assert status == 0
    assert stdout == "0"
    assert stderr == ""


def test_mul4():
    status, stdout, stderr = run_test_case(
        source=PROG_MUL,
        deps=STDLIB,
        stdin="-5\n-3",
    )

    assert status == 0
    assert stdout == "15"
    assert stderr == ""


def test_mul5():
    status, stdout, stderr = run_test_case(
        source=PROG_MUL,
        deps=STDLIB,
        stdin="5\n2",
    )

    assert status == 0
    assert stdout == "10"
    assert stderr == ""
