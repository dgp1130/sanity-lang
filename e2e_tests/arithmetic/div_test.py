from test_utils import run_test_case, STDLIB

PROG_DIV = """
extern printf: (string) -> int;
extern stringify: (int) -> string;
extern readInt: () -> int;

printf(stringify(readInt() / readInt()));
"""


def test_div1():
    status, stdout, stderr = run_test_case(
        source=PROG_DIV,
        deps=STDLIB,
        stdin="4\n2",
    )

    assert status == 0
    assert stdout == "2"
    assert stderr == ""


def test_div2():
    status, stdout, stderr = run_test_case(
        source=PROG_DIV,
        deps=STDLIB,
        stdin="0\n5",
    )

    assert status == 0
    assert stdout == "0"
    assert stderr == ""


def test_div3():
    status, stdout, stderr = run_test_case(
        source=PROG_DIV,
        deps=STDLIB,
        stdin="5\n5",
    )

    assert status == 0
    assert stdout == "1"
    assert stderr == ""


def test_div4():
    status, stdout, stderr = run_test_case(
        source=PROG_DIV,
        deps=STDLIB,
        stdin="-5\n-1",
    )

    assert status == 0
    assert stdout == "5"
    assert stderr == ""


def test_div5():
    status, stdout, stderr = run_test_case(
        source=PROG_DIV,
        deps=STDLIB,
        stdin="5\n2",
    )

    assert status == 0
    assert stdout == "2"
    assert stderr == ""


def test_div6():
    status, stdout, stderr = run_test_case(
        source=PROG_DIV,
        deps=STDLIB,
        stdin="5\n0",
    )

    assert status != 0
