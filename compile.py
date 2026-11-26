"""Top-level c6502 compiler driver, with gcc's flags.

The compiler is written in C++ (cpp/), and this module is a shim over
its `c6502cpp compile` command, which reads the whole command line with
compile.py's own argparse and runs every stage in one process
(cpp/src/compile_command.h). The shim stays until step 9 of the port
(docs/cpp_port.md), since the suite, the simulator harness, and the
fuzzers call `main`, `_run_stage`, and `_run_source` in-process and
redirect their streams, and since the messages and the usage keep saying
`compile.py:` until then.

Each C input is preprocessed and then compiled up to the stage
requested by at most one of:

  --lex      stop after tokenization; one `line:col<tab>kind<tab>value`
             line per token
  --parse    stop after parsing; pretty-print the c99_ast tree
  --resolve  stop after name resolution (identifier resolution, label
             resolution, then loop labeling); pretty-print the
             rewritten c99_ast (user variables -> `@N.orig`, labels ->
             `@<funcname>.<orig>`, loops -> `.loop@N`). Type checking
             is *not* run for this stage so the rewritten AST surfaces
             cleanly even when the program would later be rejected by
             the type checker.
  --tac      stop after TAC translation; pretty-print the tac_ast tree.
             Type checking runs first.
  -S         go all the way to 6502 assembly text, by default in
             `<stem>.s` in the current directory (stdout for stdin).
             Type checking runs first.
  -c         assemble the text `-S` writes, or an `.asm` input, into a
             relocatable ELF object, by default `<stem>.o` in the
             current directory.

With none of them, every input (C, `.asm`, or `.o`) becomes an object
and the objects are linked into an ELF executable, by default `a.out`.
`-T script.ld` lays the program out by a linker script, `-Ttext=ADDR`
or `--origin ADDR` starts `.text` somewhere other than $0800 in a link
without one, and `--defsym NAME=VALUE` gives an address to a name no
object defines. The runtime helpers the program calls to multiply,
divide, and shift come from the runtime library, unless `--defsym`
gives their addresses.

`-O` (orthogonal to the stage flag) runs the optimizer pipeline:
TAC-level fixed-point opts (constant folding, strength reduction,
comparison-against-zero / jump fold, UCE, copy prop, DSE) followed by
the asm-level SSA round-trip with byte-granular copy-prop / backward
copy-prop / DCE, byte-granular regalloc, and late prologue / epilogue
synthesis. Effective for `--resolve`, `--tac`, `-S`, `-c`, and C inputs
to a link. Also enables the `[[c6502::zp_abi]]` calling-convention
optimization (frame elimination on annotated leaves).

The other text stages write to stdout by default, or to the file
named by `-o`, where `-` is stdout. With `-S`, the output file (if
any) must have a `.s` or `.asm` suffix.

Any flag not recognized by this driver is forwarded to the preprocessor,
which takes pcpp's command line (`-D`, `-U`, `-N`, `-I`, `--passthru-*`,
`--line-directive`, etc.). pcpp's own `-o` is not forwarded; this
driver's `-o` is for the final output.
"""

from __future__ import annotations

import sys


# Whether `-O` inlines the calls of small `static` functions. A test
# that checks a function's own code sets it to False for its compiles
# (tests/no_inlining.py).
inline_small_functions = True


def _inlining() -> bool:
    """Whether `-O` inlines the calls of small `static` functions, as
    `inline_small_functions` says. Every request to the C++ port
    carries it, since the C++ side runs the inliner."""
    return inline_small_functions


# The exception classes that report a problem with the program being
# compiled, by the names the C++ port gives them (`cpp_backend._exception`).
_SOURCE_ERROR_NAMES = (
    "LexError", "ParserError", "PreprocessorError", "LarkError",
    "IdentifierResolutionError", "LabelResolutionError",
    "LoopLabelingError", "UnrollError", "ConstantExpressionError",
    "TypeCheckError", "AbiSelectionError", "RegallocError",
    "ZpSlotAllocationError", "ZpLocalAllocationError", "FrameSizeError",
    "RecursionSaveError", "NotImplementedError",
)


def _source_errors() -> tuple[type[Exception], ...]:
    """The exception types that report a problem with the program being
    compiled, such as a misused attribute or a frame that's too large.
    `main` prints these as one-line errors, and anything else is a bug
    in the compiler. They are the classes that `cpp_backend._exception`
    raises for the C++ port's errors of those names. The parser raises
    NotImplementedError for a construct c6502 doesn't take, such as a
    variable-length array or a K&R parameter list."""
    import cpp_backend
    return tuple(cpp_backend._error_type(name) for name in _SOURCE_ERROR_NAMES)


# The stages that print text before code generation.
_TEXT_STAGES = ("lex", "parse", "resolve", "tac")


def _run_source(
    stage: str, source: str, optimize: bool = False,
    link_metadata: bool = False, pcpp_args=None,
    filename: str | None = None,
) -> str:
    """The output of `stage` ("lex", "parse", "resolve", "tac", or
    "codegen" for `-S`) for the C source text `source`, preprocessed
    with the arguments `pcpp_args` as the file `filename` (None for
    stdin), in one request to the C++ port. `link_metadata` gives the
    unoptimized `-S` text the link metadata block that `-c` assembles.
    The preprocessor's warnings go to the caller's
    `diagnostics.collecting()` block, and are dropped outside one, and
    what it writes to stderr goes to `sys.stderr`."""
    return _request(
        stage, source, optimize, link_metadata, pcpp_args or [], filename,
        preprocess=True,
    )


def _run_stage(
    stage: str, source: str, optimize: bool = False,
    link_metadata: bool = False,
) -> str:
    """The output of `stage` for `source`, which is the preprocessor's
    output already, as `_run_source` gives it for a source text."""
    return _request(
        stage, source, optimize, link_metadata, [], None, preprocess=False,
    )


def _request(
    stage: str, source: str, optimize: bool, link_metadata: bool,
    pcpp_args, filename: str | None, *, preprocess: bool,
) -> str:
    """Sends `c6502cpp backend` the request of `stage` for `source`
    (cpp_backend.py), and returns the text it gives."""
    import cpp_backend
    if stage == "codegen":
        return cpp_backend.compile_source(
            source, pcpp_args, filename, optimize=optimize,
            preprocess=preprocess, inline=_inlining(),
            link_metadata=link_metadata,
        )
    if stage in _TEXT_STAGES:
        return cpp_backend.stage_source(
            stage, source, pcpp_args, filename, optimize=optimize,
            preprocess=preprocess, inline=_inlining(),
        )
    raise AssertionError(f"unknown stage: {stage!r}")


def main(argv: list[str]) -> int:
    """Compiles, assembles, and links, as gcc does, on the command line
    `argv` (the program's name first), and returns the exit status.

    With `-c`, each input (C or assembly) becomes an object, by default
    `<stem>.o` in the current directory. With `-S`, each C input
    becomes assembly, by default `<stem>.s` in the current directory,
    or stdout for stdin; `-o` names a `.s` or `.asm` file, or `-` for
    stdout. With `--lex`, `--parse`, `--resolve`, or `--tac`, each C
    input stops at that stage, written to stdout unless `-o` names a
    file. With none of these, every input (C, assembly, or
    object) becomes an object, and the objects are linked into an
    executable, by default `a.out`.

    The C++ port does all of it, from the reading of the command line
    on (`c6502cpp compile`, through `cpp_backend.run_tool`). What it
    writes goes to `sys.stdout` and `sys.stderr` in its order, argparse's
    help and usage errors raise SystemExit, and a bug in the compiler
    raises its exception. The C++ side asks for stdin when a C input is
    `-`, which Python reads with `sys.stdin.read()`."""
    import cpp_backend
    return cpp_backend.run_tool("compile", argv, inline=_inlining())


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
