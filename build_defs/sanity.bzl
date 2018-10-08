"""Build definitions for Sanity."""

load("@llvm//:llvm.bzl", "llc")

def sanity_binary(name, src, deps = []):
    """Compiles a binary for the Sanity language.

    Outputs:
      <name>: A binary which executes the given src file.

    Args:
      name: Name of this rule.
      src: The source file to compile.
      deps: Dependencies to compile this source file with.
    """

    # Use the compiler to generate some LLVM IR.
    llvm_ir = "%s.ll" % name
    native.genrule(
        name = "%s_compile" % name,
        srcs = [src],
        outs = [llvm_ir],
        cmd = """
            $(location //compiler) < "$<" > "$@"
        """,
        tools = ["//compiler"],
    )

    # Invoke llc to compile the LLVM IR to Assembly.
    assembly = "%s.s" % name
    llc(
        name = "%s_llc" % name,
        src = llvm_ir,
        out = assembly,
    )

    # Assemble into the final binary.
    native.cc_binary(
        name = name,
        srcs = [assembly],
        deps = deps,
        linkopts = ["-static"],
    )