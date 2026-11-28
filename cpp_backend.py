"""Runs the C++ port of the compiler, `c6502cpp`, for compile.py, nm.py,
objdump.py, the simulator harness, and the tests.

`compile_source` sends `c6502cpp backend` the source text, the
preprocessor's arguments, and the file name, and the C++ side runs the
whole compile: the preprocessor, the parser, under `-O` the unroller,
the three passes that resolve names, string lifting, and the type
checker, then `c99_to_tac`, under `-O` the inliner,
`return_in_zero_page`, and the TAC optimizer's passes, the plan of zero
page, code generation, the asm back end's region (or the unoptimized
peephole fixed point and long branches), the work after it that reads
the asm program, `asm_to_asm2`, and emission, and returns the `-S` text.
Under `C6502_CHECK_IR=1`, the request's
`check_ir` turns on the IR checks (cpp/src/passes/ir_check.h), which
check the IR after each pass, and a failed check comes back as the
AssertionError of an internal error, with the message of
`IRCheckError`. Without `emit`, the C++ side stops before `asm_to_asm2`
and returns the finished asm program with its slot symbols, which the
simulator harness assembles.

`lex_source` sends the same request in the mode `lex`, which runs only
the preprocessor and the lexer and returns the text `compile.py --lex`
prints, and `stage_source` sends it in the modes `parse`, `resolve`, and
`tac`, which return the tree `compile.py --parse`, `--resolve`, and
`--tac` print.

`run_tool` runs compile.py's whole command line in `c6502cpp compile`
(cpp/src/compile_command.h), which reads it with compile.py's argparse
and runs the stage it names in one process, and nm.py's and
objdump.py's in `c6502cpp nm` and `c6502cpp objdump`.

`helper_program` and `helper_footprints` give the runtime helpers,
which are built in C++ (cpp/src/rtlib/helpers.h), as `c6502cpp
helper-dump` and `c6502cpp helper-footprints` write them, for the
simulator, which assembles them at $E000, and for the tests.

The response carries what the preprocessor wrote to stderr and the
warnings it issued, which `compile_source` writes to `sys.stderr` and
issues again through `diagnostics.warn`, and an error that a pass of
the compile raised, which it raises as the Python exception of the same
class and message, from errors.py.

Each call starts c6502cpp (cpp/src/backend.h describes the request and
the response), found at the path in `C6502_CPP`, or in cpp/build/bin
(`_cpp_cli`). The C++ process inherits the current directory, so the
paths in the preprocessor's messages are relative to the caller's.
"""
from __future__ import annotations

import functools
import json
import os
import subprocess
import sys

# The directory of c6502's own headers, which the request gives the C++
# preprocessor, found beside this module.
_INCLUDE_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "include")


def _cpp_cli() -> str | None:
    """The path of c6502cpp: the path in `C6502_CPP`, or else the
    executable in cpp/build/bin, where cpp/README.md's build puts it, or
    None when neither is there."""
    env = os.environ.get("C6502_CPP")
    if env:
        return env
    root = os.path.dirname(os.path.abspath(__file__))
    for name in ("c6502cpp.exe", "c6502cpp"):
        path = os.path.join(root, "cpp", "build", "bin", name)
        if os.path.exists(path):
            return path
    return None

def _require_cli() -> str:
    """The path of c6502cpp (`_cpp_cli`), or a RuntimeError that says
    to build it when it isn't built."""
    cli = _cpp_cli()
    if cli is None:
        raise RuntimeError(
            "c6502cpp isn't built (cpp/README.md has the build), and "
            "the compiler runs in it",
        )
    return cli


# The module of each exception class of the compiler that the C++ side
# reports by name, for the classes the builtins don't have. errors.py
# holds all but `RuntimeLayoutError`.
_ERROR_MODULES = {
    name: "errors" for name in (
        "AbiSelectionError", "AssemblerError", "ConstantExpressionError",
        "ElfError", "FrameSizeError", "IdentifierResolutionError",
        "LabelResolutionError", "LarkError", "LayoutError", "LexError",
        "LinkError", "LoopLabelingError", "NumberError", "ParserError",
        "PreprocessorError", "RecursionSaveError", "RegallocError",
        "ScriptError", "TypeCheckError", "UnexpectedCharacters",
        "UnexpectedEOF", "UnexpectedInput", "UnexpectedToken",
        "UnrollError", "ZpGlobalError", "ZpLocalAllocationError",
        "ZpSlotAllocationError",
    )
}
_ERROR_MODULES["RuntimeLayoutError"] = "passes.runtime_layout"
# `re.error`, which `re.compile` raises for an invalid pattern of
# `--passthru-includes`.
_ERROR_MODULES["error"] = "re"


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
    `message`. Builtins such as KeyError and UnicodeDecodeError don't
    take their message as their one argument,
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
    cli = _require_cli()
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
    `lex_source` does for "lex". For the other three it is the `pretty`
    text (cpp/generated/, written by asdlgen) of the c99 program after the
    parser, of the program after the passes that resolve names, or of
    the TAC after `c99_to_tac` (and under `-O`, `optimize`, after the
    inliner, unless `inline` is false, and the TAC optimizer). With the
    IR checks on, the TAC optimizer's driver checks the TAC after each
    pass."""
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
    the finished asm program and its slot symbols: those of the zp_abi
    slots under `-O`, or of the pinned slots of the routines with fixed
    addresses without it, with the zero-page globals and the runtime's
    names.

    What the preprocessor wrote to stderr goes to `sys.stderr`, and its
    warnings to the caller's `diagnostics.collecting()` block, before
    the error of a rejected program is raised. With the IR checks on
    (`_ir_check_enabled`), the C++ side checks the IR after each pass,
    and a failed check raises AssertionError."""
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


def _ir_check_enabled() -> bool:
    """Whether the IR checks are on, which `C6502_CHECK_IR=1` asks for."""
    return os.environ.get("C6502_CHECK_IR") == "1"


def run_tool(name: str, argv: list[str], *, inline: bool = True) -> int:
    """Runs the C++ port of a tool, `c6502cpp <name>`, on the command
    line `argv` (the program's name first), as compile.py's, nm.py's,
    and objdump.py's `main` do, and returns
    its exit status. What it writes to stdout and stderr goes to
    `sys.stdout` and `sys.stderr`, so a caller that redirects them, as a
    test does, gets it. The C++ side wraps the usage and help as
    argparse does, to the width `shutil.get_terminal_size` gives here
    (`_terminal_columns`), since its own stdout isn't the terminal. A bug the C++ side reports (exit status
    3) is raised as an `AssertionError`.

    For "compile", `_run_compile` runs it, with `inline` for
    `compile._inlining()`. For nm and objdump, a bad command line ends
    with SystemExit(2), as argparse's does, and the help with a return
    of 0, where Python's argparse raises SystemExit(0). argparse writes its usage
    before every error, so an exit status of 2 whose stderr doesn't
    start with `usage: ` comes from the tool itself, as objdump.py's
    when nothing is asked for, and is returned."""
    cli = _require_cli()
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


@functools.lru_cache(maxsize=1)
def helper_program():
    """Returns the `asm_ast.Program` of the runtime helpers that have
    6502 code, in the order the runtime library lays them out. Every call
    returns the same object, which the caller must not change."""
    import ir_dump
    done = subprocess.run(
        [_require_cli(), "helper-dump"], check=True, capture_output=True,
    )
    return ir_dump.load(done.stdout.decode("utf-8"), "asm_ast")


def helper_names() -> frozenset[str]:
    """Returns the names of the runtime helpers that have 6502 code."""
    return frozenset(fn.name for fn in helper_program().top_level)


@functools.lru_cache(maxsize=1)
def helper_footprints() -> dict[str, int]:
    """Returns each runtime helper's HARGS footprint, the number of
    HARGS bytes it touches, counting the helpers it calls, by name, in
    the order of `helper_program`. Every call returns the same dict,
    which the caller must not change."""
    done = subprocess.run(
        [_require_cli(), "helper-footprints"], check=True,
        capture_output=True,
    )
    out: dict[str, int] = {}
    for line in done.stdout.decode("utf-8").splitlines():
        name, size = line.split()
        out[name] = int(size)
    return out


def cpu_run(image: bytes, segments: list[dict], *, stops, stop_at_brk: bool,
            max_cycles: int, max_instructions: int | None = None) -> list[dict]:
    """Runs the C++ CPU (cpp/src/sim/cpu_run.h) on the 64K `image` for
    each of `segments`, a dict of `pc`, `a`, `x`, `y`, `sp`, `p`,
    `cycles`, and `patches` (`[address, value]` pairs stored before the
    segment), until PC reaches one of `stops`, a BRK with `stop_at_brk`,
    `max_cycles`, or `max_instructions`. Returns a dict for each segment
    run, with `stop` ("stop", "brk", "cycles", "count", or "error"),
    `instructions`, `state` (the seven registers and the cycle count, as
    a tuple in that order), `last_writes` (the last instruction's
    `(address, value)` writes), `writes` (each address written and its
    last value), `digests` (the digest of each instruction, an
    array('I')), and for "error" `error`, its message."""
    import array
    args = json.dumps({
        "stops": sorted(stops), "stop_at_brk": stop_at_brk,
        "max_cycles": max_cycles, "max_instructions": max_instructions,
        "segments": segments,
    })
    request = (
        f"c6502-backend 3\nmode cpu-run\nargs {args}\n"
        f"image {len(image)}\n"
    ).encode("utf-8") + bytes(image)
    data = _send(request)
    out: list[dict] = []
    pos = 0

    def line() -> list[str]:
        nonlocal pos
        end = data.index(b"\n", pos)
        words = data[pos:end].decode("utf-8").split(" ")
        pos = end + 1
        return words

    def section(key: str) -> bytes:
        nonlocal pos
        words = line()
        if words[0] != key:
            raise RuntimeError(f"cpu-run's response has {words[0]} for {key}")
        n = int(words[1])
        body = data[pos:pos + n]
        pos += n
        return body

    while pos < len(data):
        _, _, stop, count = line()
        seg: dict = {"stop": stop, "instructions": int(count)}
        seg["state"] = tuple(int(v) for v in line()[1:])
        seg["last_writes"] = [
            (int(w[:4], 16), int(w[5:], 16)) for w in line()[1:]
        ]
        raw = section("writes")
        seg["writes"] = {
            raw[i] | raw[i + 1] << 8: raw[i + 2] for i in range(0, len(raw), 3)
        }
        digests = array.array("I")
        digests.frombytes(section("digests"))
        if sys.byteorder != "little":
            digests.byteswap()
        seg["digests"] = digests
        if stop == "error":
            words = line()
            seg["error"] = json.loads(" ".join(words[1:]))
        out.append(seg)
    return out


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
    `sys.stdin.read()`, so a test that patches `sys.stdin` gives the
    C++ side its text."""
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
