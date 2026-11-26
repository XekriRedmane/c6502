"""Compiles a C source through the C++ port, for compile.py and the
simulator harness under `C6502_CPP_BACKEND=1`.

`compile_source` sends `c6502cpp backend` the source text, the
preprocessor's arguments, and the file name, and the C++ side runs the
whole compile: the front end of `compile._compile_c` (`preprocess`,
`parse`, under `-O` `unroll_program`, the three passes that resolve
names, string lifting, and the type checker), then `c99_to_tac`, under
`-O` the inliner, `return_in_zero_page`, and the TAC passes of
`compile._optimized_tac`, `plan_zero_page`, code generation, the asm
back end's region (or the unoptimized peephole fixed point and long
branches), the work after it that reads the asm program, `asm_to_asm2`,
and `asm_emit`, and returns the `-S` text. Python reads no program back.
Under `C6502_CHECK_IR=1`, or after `ir_check.enable()` or
`enable_ir_check()`, the request's
`check_ir` turns on the C++ port of `ir_check` (cpp/src/passes/ir_check.h),
which checks the IR at each place the Python pipeline checks it, and a
failed check comes back as the AssertionError of an internal error, with
the message Python's `IRCheckError` would have. Without `emit`, the C++ side stops where
`compile.optimized_asm` (or the harness's unoptimized path) does, and
returns the finished asm program with its slot symbols, which the
simulator harness assembles.

`lex_source` sends the same request in the mode `lex`, which runs only
the preprocessor and the lexer and returns the text `compile.py --lex`
prints, and `stage_source` sends it in the modes `parse`, `resolve`, and
`tac`, which return the tree `compile.py --parse`, `--resolve`, and
`--tac` print with pretty.py.

`tac_main`, `tac_results`, `tac_first_changing_pass`, and
`tac_falls_off_end` run the TAC interpreter of the C++ port on a source,
for `tests.test_sim_chapters._run_program`, fuzz/reduce.py, and
fuzz/tac_passes.py under the variable, so that those import none of the
Python front end, `c99_to_tac`, the TAC optimizer, or tac_sim.py.

`run_tool` runs compile.py's whole command line in `c6502cpp compile`
(cpp/src/compile_command.h), which reads it with compile.py's argparse
and runs the stage it names in one process, and nm.py's and
objdump.py's in `c6502cpp nm` and `c6502cpp objdump`.

The response carries what the preprocessor wrote to stderr and the
warnings it issued, which `compile_source` writes to `sys.stderr` and
issues again through `diagnostics.warn`, as the Python preprocessor
does, and an error that a pass of the compile raised, which it raises as
the Python exception of the same class and message.

Each call starts `c6502cpp backend` (cpp/src/backend.h describes the
request and the response), found as tools/check_captures.py finds it
(`_cpp_cli`): at the path in `C6502_CPP`, or in cpp/build/bin. The C++ process inherits
the current directory, so the paths in the preprocessor's messages come
out as Python's do.

compile.py imports this module only when the variable is set, so a
compile without it imports nothing new. A compile with it imports none
of the parser, Lark, pcpp, or the front end's passes, except for the
class of the exception a rejected program raises.
"""
from __future__ import annotations

import json
import os
import subprocess
import sys

# The directory of c6502's own headers, as preprocessor.py's
# `_BUNDLED_INCLUDE_DIR` spells it, since this module is beside it.
_INCLUDE_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "include")


def _cpp_cli() -> str | None:
    """The path of c6502cpp, found as tools/check_captures.py's
    `cpp_cli` finds it: the path in `C6502_CPP`, or cpp/build/bin. It is
    found here so that compile.py's start doesn't import that module and
    its argparse."""
    env = os.environ.get("C6502_CPP")
    if env:
        return env
    root = os.path.dirname(os.path.abspath(__file__))
    for name in ("c6502cpp.exe", "c6502cpp"):
        path = os.path.join(root, "cpp", "build", "bin", name)
        if os.path.exists(path):
            return path
    return None

# The module of each exception class of the compiler that the C++ side
# reports by name, for the classes the builtins don't have.
_ERROR_MODULES = {
    "AbiSelectionError": "passes.abi_selection",
    "AssemblerError": "binutils.assembler",
    "ConstantExpressionError": "passes.constant_expression",
    "ElfError": "binutils.elf",
    "FrameSizeError": "passes.asm_to_asm2",
    "IdentifierResolutionError": "passes.identifier_resolution",
    "LabelResolutionError": "passes.label_resolution",
    "LayoutError": "passes.link_layout",
    "LexError": "lexer",
    "LinkError": "binutils.ld",
    "LoopLabelingError": "passes.loop_labeling",
    "NumberError": "pp_numbers",
    "ParserError": "parser",
    "PreprocessorError": "preprocessor",
    "RecursionSaveError": "passes.recursion_saves",
    "RegallocError": "passes.optimization_asm.regalloc",
    "RuntimeLayoutError": "passes.runtime_layout",
    "ScriptError": "passes.link_script",
    "TypeCheckError": "passes.type_checking",
    "UnexpectedCharacters": "lark.exceptions",
    "UnexpectedEOF": "lark.exceptions",
    "UnexpectedInput": "lark.exceptions",
    "UnexpectedToken": "lark.exceptions",
    "UnrollError": "passes.optimization_ast.unroll",
    # `re.error`, which `re.compile` raises for an invalid pattern of
    # `--passthru-includes`.
    "error": "re",
    "ZpGlobalError": "passes.zp_globals",
    "ZpLocalAllocationError": "passes.zp_local_allocation",
    "ZpSlotAllocationError": "passes.zp_slot_allocation",
}

# The subclasses `_exception` made, by base class.
_WITH_MESSAGE: dict[type, type] = {}


def _error_type(name: str) -> type[BaseException]:
    """The Python exception class that the C++ `CompileError` of Python
    type `name` stands for, or RuntimeError for a name neither the
    compiler nor the builtins have. A name with a module in front, such
    as `struct.error`, which `elf.write` raises, is that module's class,
    since `re.error` has the same name."""
    import builtins
    import importlib
    if "." in name:
        module, _, attr = name.rpartition(".")
        return getattr(importlib.import_module(module), attr)
    if name in _ERROR_MODULES:
        return getattr(importlib.import_module(_ERROR_MODULES[name]), name)
    found = getattr(builtins, name, None)
    if isinstance(found, type) and issubclass(found, BaseException):
        return found
    return RuntimeError


def _exception(name: str, message: str) -> BaseException:
    """The exception of the Python class `name` whose `str()` is
    `message`. Lark's syntax errors, and builtins such as KeyError and
    UnicodeDecodeError, don't take their message as their one argument,
    so for them it is an instance of a subclass of the same name whose
    `str()` returns the message, which an `except` of the class still
    catches. The C++ preprocessor reports argparse's exit as a
    SystemExit whose message is the status, which it has already written
    the usage for."""
    cls = _error_type(name)
    if cls is SystemExit:
        return SystemExit(int(message) if message.isdigit() else message)
    try:
        e = cls(message)
        if str(e) == message:
            return e
    except Exception:  # noqa: BLE001 - a constructor of other arguments
        pass
    sub = _WITH_MESSAGE.get(cls)
    if sub is None:
        def init(self, msg):
            BaseException.__init__(self, msg)
            self._c6502_message = msg

        sub = type(cls.__name__, (cls,), {
            "__init__": init,
            "__str__": lambda self: self._c6502_message,
            "__module__": cls.__module__,
        })
        _WITH_MESSAGE[cls] = sub
    return sub(message)


def _send(request: str | bytes) -> bytes:
    """Sends `request` to `c6502cpp backend` and returns the response's
    bytes. A bug the C++ compiler reports (exit status 3) is raised as
    an `AssertionError` with its message, since that is what the C++
    `InternalError` stands for."""
    cli = _cpp_cli()
    if cli is None:
        raise RuntimeError(
            "C6502_CPP_BACKEND=1 is set, but c6502cpp isn't built "
            "(cpp/README.md has the build)",
        )
    done = subprocess.run(
        [str(cli), "backend"],
        input=request.encode("utf-8") if isinstance(request, str) else request,
        capture_output=True,
    )
    if done.returncode != 0:
        message = done.stderr.decode("utf-8", "replace").strip()
        if done.returncode == 3:
            raise AssertionError(message.removeprefix("internal error: "))
        raise RuntimeError(f"c6502cpp backend failed: {message}")
    return done.stdout


def _section(name: str, text: str) -> str:
    """A section of the request: its name and its length in bytes on a
    line, then its text."""
    return f"{name} {len(text.encode('utf-8'))}\n{text}"


def _sections(data: bytes) -> tuple[dict[str, str], dict[str, str]]:
    """Splits a response into its lines by key and its sections by name
    (cpp/src/backend.h)."""
    lines: dict[str, str] = {}
    sections: dict[str, str] = {}
    pos = 0
    while pos < len(data):
        end = data.index(b"\n", pos)
        key, _, value = data[pos:end].decode("utf-8").partition(" ")
        pos = end + 1
        if key in ("stderr", "asm", "text"):
            n = int(value)
            sections[key] = data[pos:pos + n].decode("utf-8")
            pos += n
        else:
            lines[key] = value
    return lines, sections


def _replay_front(lines: dict[str, str], sections: dict[str, str]) -> None:
    """Writes what the preprocessor wrote to stderr to `sys.stderr`,
    issues the warnings to the caller's `diagnostics.collecting()` block,
    and raises the error of a rejected program as its Python exception,
    from the response of a compile of one source."""
    import diagnostics
    if sections["stderr"]:
        sys.stderr.write(sections["stderr"])
    for where, message in json.loads(lines["warnings"]):
        diagnostics.warn(where, message)
    if "error" in lines:
        error = json.loads(lines["error"])
        raise _exception(error["type"], error["message"])


def lex_source(
    source: str, pcpp_args, filename: str | None, *, preprocess: bool = True,
) -> str:
    """Returns what `compile.py --lex` prints for the C source `source`,
    which came from the file `filename` (None for stdin): the tokens of
    the text that `preprocess` with the arguments `pcpp_args` gives, or
    of `source` itself without `preprocess`, one `<line>:<col>`, tab,
    kind, tab, value line each. The preprocessor's stderr and warnings
    and the error of a rejected program come back as `compile_source`
    gives them."""
    return stage_source(
        "lex", source, pcpp_args, filename, optimize=False,
        preprocess=preprocess,
    )


def stage_source(
    stage: str, source: str, pcpp_args, filename: str | None, *,
    optimize: bool, preprocess: bool = True, inline: bool = True,
) -> str:
    """Returns what `compile.py` prints for the C source `source` at
    `stage`, one of "lex", "parse", "resolve", and "tac", as
    `lex_source` does for "lex". For the other three it is pretty.py's
    text of the c99 program after the parser, of the program after
    `compile._resolved`, or of the TAC after `c99_to_tac` (and under
    `-O`, `optimize`, after `inline_functions`, unless `inline` is
    false, and the TAC optimizer). With the IR checks on, the C++ TAC
    optimizer's driver checks the TAC after each pass, as Python's
    does."""
    args = json.dumps({
        "pcpp_args": list(pcpp_args or []), "filename": filename,
        "include_dir": _INCLUDE_DIR, "preprocess": preprocess,
        "optimize": optimize, "inline": inline,
        "check_ir": stage == "tac" and _ir_check_enabled(),
    })
    request = (
        f"c6502-backend 3\nmode {stage}\nargs {args}\n"
        f"{_section('source', source)}"
    )
    lines, sections = _sections(_send(request))
    _replay_front(lines, sections)
    return sections["text"]


def compile_source(
    source: str, pcpp_args, filename: str | None, *, optimize: bool,
    preprocess: bool = True, inline: bool = True,
    link_metadata: bool = False, emit: bool = True,
):
    """Returns the `-S` text of the C source `source`, which came from
    the file `filename` (None for stdin), preprocessed with the
    arguments `pcpp_args`, or taken as the preprocessor's output already
    without `preprocess`. `optimize` is `-O`, `inline` is false when
    `-O` doesn't inline the calls of small `static` functions
    (tests/no_inlining.py), and `link_metadata` gives the unoptimized
    text the link metadata block `-c` assembles. Without `emit`, returns
    the finished asm program and its slot symbols, `{**slot_symbols,
    **zp.equs()}` under `-O` and `{**pinned_slot_symbols(fixed_abi),
    **zp.equs()}` without it, as the simulator harness computes them.

    What the preprocessor wrote to stderr goes to `sys.stderr`, and its
    warnings to the caller's `diagnostics.collecting()` block, before
    the error of a rejected program is raised. With the IR checks on
    (`_ir_check_enabled`), the C++ side checks the IR at each place the
    Python pipeline does, and a failed check raises AssertionError."""
    args = json.dumps({
        "pcpp_args": list(pcpp_args or []), "filename": filename,
        "include_dir": _INCLUDE_DIR, "preprocess": preprocess,
        "optimize": optimize, "inline": inline,
        "check_ir": _ir_check_enabled(), "link_metadata": link_metadata,
    })
    mode = "source-text" if emit else "source"
    request = (
        f"c6502-backend 3\nmode {mode}\nargs {args}\n"
        f"{_section('source', source)}"
    )
    lines, sections = _sections(_send(request))
    _replay_front(lines, sections)
    if not emit:
        import asm_ast
        import ir_dump
        out = ir_dump.load(sections["asm"], asm_ast)
        return out, dict(json.loads(lines["symbols"]))
    return sections["text"]


class TacSimError(Exception):
    """The error of the C++ TAC interpreter (cpp/src/tac_sim.h) that
    tac_sim.py raises as its own `TacSimError`. The class stands in for
    that one, so that a caller of the functions below under
    `C6502_CPP_BACKEND=1` loads neither tac_sim.py nor the type checker
    it imports. Its name and message are Python's."""


def _tac_request(mode: str, source: str, **args) -> tuple[dict, dict]:
    """Sends the TAC interpreter's `mode` (cpp/src/tac_sim_modes.h) the
    source `source`, preprocessed as `preprocess(source)` preprocesses
    it, and returns the response's lines and sections. What the
    preprocessor wrote to stderr goes to `sys.stderr` and its warnings
    to `diagnostics.warn`, and the error of a rejected program is raised
    as its Python exception. With the IR checks on (`_ir_check_enabled`),
    the TAC optimizer's driver checks the TAC after each pass, as
    Python's does, and a failed check comes back as an AssertionError."""
    kwargs = json.dumps({
        "pcpp_args": [], "filename": None, "include_dir": _INCLUDE_DIR,
        "preprocess": True, "check_ir": _ir_check_enabled(), **args,
    })
    request = (
        f"c6502-backend 3\nmode {mode}\nargs {kwargs}\n"
        f"{_section('source', source)}"
    )
    lines, sections = _sections(_send(request))
    _replay_front(lines, sections)
    return lines, sections


def _tac_value(text: str):
    """The value of a `result` or `optimized` line, or the raise of the
    exception that the interpreter raised, as its Python class."""
    outcome = json.loads(text)
    if "error" in outcome:
        error = outcome["error"]
        if error["type"] == "TacSimError":
            raise TacSimError(error["message"])
        raise _exception(error["type"], error["message"])
    return outcome["value"]


def tac_main(source: str) -> int | None:
    """Returns what `main` of the C source `source` returns on its
    unoptimized TAC, as `tests.test_sim_chapters._run_program` computes
    it with the Python front end, `c99_to_tac`, and tac_sim.py: an int,
    signed when `main`'s type is, or None. The front end's error and the
    interpreter's are raised with their Python classes, the latter's
    `TacSimError` being this module's."""
    lines, _ = _tac_request("tac-run", source)
    return _tac_value(lines["result"])


def tac_results(source: str) -> tuple:
    """Returns `main`'s result on the unoptimized TAC of `source` and on
    the TAC after the TAC optimizer (`optimize_program`), as
    fuzz/reduce.py's `_tac_results` computes them, raising as
    `tac_main` does."""
    lines, _ = _tac_request("tac-run", source, optimized=True)
    plain = _tac_value(lines["result"])
    return plain, _tac_value(lines["optimized"])


def tac_first_changing_pass(source: str):
    """Returns fuzz/tac_passes.py's `first_changing_pass` of `source`:
    `(function, pass, expected, got)` for the first step of the TAC
    optimizer after which `main` returns something other than on the
    unoptimized TAC, or None. A result is an int, None, or the text
    `ERR <type>: <message>` of what the interpreter raised."""
    lines, _ = _tac_request("tac-observe", source)
    found = json.loads(lines["found"])
    return None if found is None else tuple(found)


def tac_falls_off_end(source: str) -> bool:
    """Returns fuzz/reduce.py's `falls_off_end` of `source`: whether a
    run of `main` on the TAC reaches the end of a function, other than
    `main`, that returns a value. A program that the front end rejects
    gives False."""
    lines, _ = _tac_request("tac-falls-off", source)
    return lines["fell"] == "true"


# What `enable_ir_check` set, or None when nothing has called it.
_check_ir: bool | None = None


def enable_ir_check(on: bool = True) -> bool:
    """Turns the IR checks on, or off, for the compiles that follow, and
    returns whether they were on (`_ir_check_enabled`). Under
    `C6502_CPP_BACKEND=1` the requests carry the choice, and the Python
    `ir_check.enable` gets it too when passes/ir_check.py is loaded or
    the switch is off, since the Python pipeline checks the IR then. A
    test that needs the checks whatever `C6502_CHECK_IR` says calls it
    in place of `ir_check.enable`, which the shims don't have once the
    Python compiler is removed."""
    global _check_ir
    was = _ir_check_enabled()
    _check_ir = on
    if os.environ.get("C6502_CPP_BACKEND") != "1" or "passes.ir_check" in sys.modules:
        from passes import ir_check
        ir_check.enable(on)
    return was


def _ir_check_enabled() -> bool:
    """Whether the IR checks are on: what `enable_ir_check` set last, or
    else what `ir_check.enabled()` says, the value of `C6502_CHECK_IR`
    when passes/ir_check.py was imported or what `ir_check.enable()` set
    since, as the fuzzers set it. When nothing has imported the module,
    the result is the variable's value, since importing the module would
    load the IRs for every compile."""
    if _check_ir is not None:
        return _check_ir
    module = sys.modules.get("passes.ir_check")
    if module is not None:
        return module.enabled()
    return os.environ.get("C6502_CHECK_IR") == "1"


def run_tool(name: str, argv: list[str], *, inline: bool = True) -> int:
    """Runs the C++ port of a tool, `c6502cpp <name>`, on the command
    line `argv` (the program's name first), as compile.py's, nm.py's,
    and objdump.py's `main` do under `C6502_CPP_BACKEND=1`, and returns
    its exit status. What it writes to stdout and stderr goes to
    `sys.stdout` and `sys.stderr`, so a caller that redirects them, as a
    test does, gets it. The C++ side wraps the usage and help as
    argparse does, to the width `shutil.get_terminal_size` gives here
    (`_terminal_columns`), since its own stdout isn't the terminal. A bug the C++ side reports (exit status
    3) is raised as an `AssertionError`.

    For "compile", `_run_compile` runs it, with `inline` for
    `compile._inlining()`. For nm and objdump, a bad command line ends
    with SystemExit(2), as argparse's does, and the help with a return
    of 0, where Python's raises SystemExit(0). argparse writes its usage
    before every error, so an exit status of 2 whose stderr doesn't
    start with `usage: ` comes from the tool itself, as objdump.py's
    when nothing is asked for, and is returned."""
    cli = _cpp_cli()
    if cli is None:
        raise RuntimeError(
            "C6502_CPP_BACKEND=1 is set, but c6502cpp isn't built "
            "(cpp/README.md has the build)",
        )
    columns = _terminal_columns()
    if name == "compile":
        return _run_compile(cli, argv, columns, inline)
    env = {**os.environ, "COLUMNS": str(columns)}
    done = subprocess.run(
        [str(cli), name, *argv[1:]], capture_output=True, env=env,
    )
    if done.returncode == 3:
        message = done.stderr.decode("utf-8", "replace").strip()
        raise AssertionError(message.removeprefix("internal error: "))
    buffer = getattr(sys.stdout, "buffer", None)
    if buffer is None:
        # The help comes in text mode, with CR LF on Windows, which a
        # text stream such as a test's StringIO takes as LF.
        sys.stdout.write(done.stdout.decode("utf-8").replace("\r\n", "\n"))
    else:
        sys.stdout.flush()
        buffer.write(done.stdout)
        buffer.flush()
    sys.stderr.write(done.stderr.decode("utf-8"))
    if done.returncode == 2 and done.stderr.startswith(b"usage: "):
        raise SystemExit(2)
    return done.returncode


def _terminal_columns() -> int:
    """Returns `shutil.get_terminal_size().columns`, as that function
    computes it, without importing shutil, which takes a tenth of
    compile.py's start: `COLUMNS` when it's a positive number, else the
    width of the terminal `sys.__stdout__` writes to, else 80."""
    try:
        columns = int(os.environ["COLUMNS"])
    except (KeyError, ValueError):
        columns = 0
    if columns <= 0:
        try:
            columns = os.get_terminal_size(sys.__stdout__.fileno()).columns
        except (AttributeError, ValueError, OSError):
            columns = 0
        columns = columns or 80
    return columns


def _run_compile(cli, argv: list[str], columns: int, inline: bool) -> int:
    """Runs `c6502cpp compile` on compile.py's command line `argv` in
    frames (cpp/src/compile_command.cpp, `framed`), and returns the exit
    status that compile.py's `main` returns. The C++ side does all that
    `main` does, and its frames give each write to stdout and stderr in
    order, which this writes to `sys.stdout` and `sys.stderr` as Python
    writes them, and at the end the status, the code of a SystemExit
    (argparse's help and usage errors), which this raises, or the
    exception `main` would raise, which this raises as its Python class.
    A C input `-` makes the C++ side ask for stdin, which this reads with
    `sys.stdin.read()`, as compile.py's `_read_source` does, so a test
    that patches `sys.stdin` gives the C++ side its text."""
    header = json.dumps({
        "include_dir": _INCLUDE_DIR, "columns": columns, "inline": inline,
        "check_ir": _ir_check_enabled(),
    })
    proc = subprocess.Popen(
        [str(cli), "compile", *argv[1:]], stdin=subprocess.PIPE,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        env={**os.environ, "C6502CPP_FRAMES": "1"},
    )
    end = None
    try:
        proc.stdin.write(header.encode("utf-8") + b"\n")
        proc.stdin.flush()
        while True:
            line = proc.stdout.readline()
            if not line:
                break
            key, _, value = line.decode("utf-8").rstrip("\n").partition(" ")
            if key == "stdin":
                _answer_stdin(proc.stdin)
            elif key in ("write-out", "write-err"):
                text = proc.stdout.read(int(value)).decode("utf-8")
                (sys.stdout if key == "write-out" else sys.stderr).write(text)
            else:
                end = (key, value)
                break
    finally:
        try:
            proc.stdin.close()
        except OSError:
            pass
        err = proc.stderr.read()
        proc.stdout.close()
        proc.stderr.close()
        proc.wait()
    message = err.decode("utf-8", "replace").strip()
    if proc.returncode == 3:
        raise AssertionError(message.removeprefix("internal error: "))
    if end is None or proc.returncode != 0:
        raise RuntimeError(f"c6502cpp compile failed: {message}")
    key, value = end
    if key == "status":
        return int(value)
    if key == "exit":
        raise SystemExit(int(value))
    error = json.loads(value)
    raise _exception(error["type"], error["message"])


def _answer_stdin(pipe) -> None:
    """Sends the C++ side what `sys.stdin.read()` returns, or the
    exception it raises, with an OSError's strerror, which compile.py's
    `_report` prints."""
    try:
        text = sys.stdin.read()
    except (OSError, UnicodeDecodeError) as e:
        error = {"type": type(e).__name__, "message": str(e),
                 "strerror": getattr(e, "strerror", None)}
        pipe.write(f"stdin-error {json.dumps(error)}\n".encode("utf-8"))
    else:
        data = text.encode("utf-8", "surrogatepass")
        pipe.write(f"stdin {len(data)}\n".encode("utf-8") + data)
    pipe.flush()
