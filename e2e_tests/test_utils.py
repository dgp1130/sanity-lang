from subprocess import Popen, PIPE
from tempfile import mkstemp
from os import close, remove

STDLIB = [
    "build/bin/stdlib/input.o",
    "build/bin/stdlib/stringify.o",
]


class CompilationException(Exception):
    def __init__(self, msg):
        self.msg = msg


class SanityCompileException(CompilationException):
    def __init__(self, msg):
        super().__init__("Failed to compile Sanity source:\n" + msg)


class LlvmCompileException(Exception):
    def __init__(self, msg):
        super().__init__("Failed to compile LLVM IR:\n" + msg)


class AssemblyException(Exception):
    def __init__(self, msg):
        super().__init__("Failed to assemble:\n" + msg)


class LinkException(Exception):
    def __init__(self, msg):
        super().__init__("Failed to link:\n" + msg)


COMPILER = "build/bin/Sanity"


def run_test_case(source, deps=[], args=[], stdin=""):
    """Compile and run the given Sanity source code.

    Args:
        source: The source code of Sanity to execute.
        deps: The list of .o files to link against.
        args: Arguments to be provided to the program when it executes.
        stdin: A string passed to the program's stdin.

    Returns:
        status: The exit status of the Sanity program.
        stdout: The stdout from the Sanity program.
        stderr: The stderr from the Sanity program.
    """
    global obj_code
    global binary

    try:
        llvm_ir = _compile_sanity(COMPILER, source)
        assembly = _compile_llvm_ir(llvm_ir)
        obj_code = _assemble(assembly)
        binary = _link([obj_code] + deps)
        return _execute(binary, args, stdin)
    finally:
        if obj_code: remove(obj_code)
        if binary: remove(binary)


def _compile_sanity(compiler, source):
    sanity_proc = Popen([compiler], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    llvm_ir, stderr = sanity_proc.communicate(source.encode())

    if sanity_proc.returncode != 0:
        raise SanityCompileException(stderr.decode('utf-8'))

    return llvm_ir


def _compile_llvm_ir(llvm_ir):
    llc = Popen(["llc"], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    assembly, stderr = llc.communicate(llvm_ir)

    if llc.returncode != 0:
        raise LlvmCompileException(stderr.decode("utf-8"))

    return assembly


def _assemble(assembly):
    handle, obj_code = mkstemp()
    assembler = Popen(["gcc", "-x", "assembler", "-", "-c", "-o", obj_code],
            stdin=PIPE, stderr=PIPE)
    _, stderr = assembler.communicate(assembly)
    close(handle)

    if assembler.returncode != 0:
        raise AssemblyException(stderr.decode("utf-8"))

    return obj_code


def _link(libs):
    handle, binary = mkstemp()

    linker = Popen(["gcc", "-o", binary] + libs, stderr=PIPE)
    _, stderr = linker.communicate()
    close(handle)

    if linker.returncode != 0:
        raise LinkException(stderr.decode("utf-8"))

    return binary


def _execute(binary, args = [], stdin = ""):
    proc = Popen([binary] + args, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    stdout, stderr = proc.communicate(stdin.encode())

    return proc.returncode, stdout.decode("utf-8"), stderr.decode("utf-8")