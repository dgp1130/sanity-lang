from test_utils import run_test_case, STDLIB


def test_hello_printf():
    status, stdout, stderr = run_test_case("""
extern printf: (string) -> int;

printf("Hello World!");
    """)

    assert status == 0
    assert stdout == "Hello World!"
    assert stderr == ""


def test_hello_puts():
    status, stdout, stderr = run_test_case("""
extern puts: (string) -> int;

puts("Hello World!");
    """)

    assert status == 0
    assert stdout == "Hello World!\n"
    assert stderr == ""


def test_hello_char():
    status, stdout, stderr = run_test_case("""
extern putchar: (int) -> int;

putchar('H');
putchar('e');
putchar('l');
putchar('l');
putchar('o');
    """)

    assert status == 0
    assert stdout == "Hello"
    assert stderr == ""


def test_echo():
    status, stdout, stderr = run_test_case("""
extern printf: (string) -> int;
extern read: () -> string;

printf(read());
    """, STDLIB, [], "testing\ntesting\none two")

    assert status == 0
    assert stdout == "testing\ntesting\none two"
    assert stderr == ""
