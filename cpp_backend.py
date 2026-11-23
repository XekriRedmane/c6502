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
and `asm_emit`, and returns the `-S` text. Python reads no program back,
except under `C6502_CHECK_IR=1`, where the response holds the TAC that
code generation lowered and the finished asm and asm2 programs for
`ir_check`, which isn't ported. Without `emit`, the C++ side stops where
`compile.optimized_asm` (or the harness's unoptimized path) does, and
returns the finished asm program with its slot symbols, which the
simulator harness assembles.

`lex_source` sends the same request in the mode `lex`, which runs only
the preprocessor and the lexer and returns the text `compile.py --lex`
prints.

`build` sends the inputs of `compile.py -c` or of a link in one request:
each C source's text, each assembly file's text, and each object's
bytes, with a link's script, origin, `--defsym`s, and output. The C++
side compiles and assembles them in the same process, returns each
object of `-c`, and links and writes the executable of a link, so no
Python assembles or links. `Built` holds the response, which compile.py
replays input by input, so that what it writes and the files it leaves
are those of the Python path.

The response carries what the preprocessor wrote to stderr and the
warnings it issued, which `compile_source` writes to `sys.stderr` and
issues again through `diagnostics.warn`, as the Python preprocessor
does, and an error that a pass of the compile raised, which it raises as
the Python exception of the same class and message.

Each call starts `c6502cpp backend` (cpp/src/backend.h describes the
request and the response), found as tools/check_captures.py finds it: at
the path in `C6502_CPP`, or in cpp/build/bin. The C++ process inherits
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
    from tools.check_captures import cpp_cli
    cli = cpp_cli()
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
        if key in ("stderr", "tac", "asm", "asm2", "text"):
            n = int(value)
            sections[key] = data[pos:pos + n].decode("utf-8")
            pos += n
        else:
            lines[key] = value
    return lines, sections


def _check_tac(text: str) -> None:
    """Checks each function of the TAC that code generation lowered, as
    the Python TAC optimizer's driver checks each function after its
    last pass. The symbol table isn't given, since the C++ side keeps
    its own."""
    import ir_dump
    import tac_ast
    from passes import ir_check
    for tl in ir_dump.load(text, tac_ast).top_level:
        if isinstance(tl, tac_ast.Function):
            ir_check.check_tac(tl, after="the C++ TAC passes")


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
    args = json.dumps({
        "pcpp_args": list(pcpp_args or []), "filename": filename,
        "include_dir": _INCLUDE_DIR, "preprocess": preprocess,
        "optimize": False,
    })
    request = (
        f"c6502-backend 3\nmode lex\nargs {args}\n"
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
    the error of a rejected program is raised. Under `C6502_CHECK_IR=1`
    it checks the TAC that code generation lowered, the finished asm
    program, and the asm2 program of a text, as compile.py checks
    them."""
    check = _ir_check_enabled()
    args = json.dumps({
        "pcpp_args": list(pcpp_args or []), "filename": filename,
        "include_dir": _INCLUDE_DIR, "preprocess": preprocess,
        "optimize": optimize,
        "inline": inline, "check_ir": check, "link_metadata": link_metadata,
    })
    mode = "source-text" if emit else "source"
    request = (
        f"c6502-backend 3\nmode {mode}\nargs {args}\n"
        f"{_section('source', source)}"
    )
    lines, sections = _sections(_send(request))
    _replay_front(lines, sections)
    if check:
        _check_tac(sections["tac"])
    if not emit:
        import asm_ast
        import ir_dump
        from passes import ir_check
        out = ir_dump.load(sections["asm"], asm_ast)
        ir_check.check_asm(out, after="the C++ back end")
        return out, dict(json.loads(lines["symbols"]))
    if check:
        _check_programs(sections)
    return sections["text"]


def _check_programs(sections: dict[str, str]) -> None:
    """Checks the finished asm program and the asm2 program of a text
    in the response's `asm` and `asm2` sections."""
    import asm2_ast
    import asm_ast
    import ir_dump
    from passes import ir_check
    ir_check.check_asm(
        ir_dump.load(sections["asm"], asm_ast), after="the C++ back end",
    )
    ir_check.check_asm2(
        ir_dump.load(sections["asm2"], asm2_ast), after="asm_to_asm2",
    )


# The keys of the response's lines that a section of that many bytes
# follows.
_SECTION_KEYS = ("stderr", "tac", "asm", "asm2", "text", "object")


def _records(data: bytes) -> list[tuple[str, str, bytes | None]]:
    """Splits a response into its lines in order, each as its key, the
    rest of the line, and the bytes of the section that follows it
    (None for a line without one)."""
    out = []
    pos = 0
    while pos < len(data):
        end = data.index(b"\n", pos)
        key, _, value = data[pos:end].decode("utf-8").partition(" ")
        pos = end + 1
        body = None
        if key in _SECTION_KEYS:
            n = int(value)
            body = data[pos:pos + n]
            pos += n
        out.append((key, value, body))
    return out


class InputResult:
    """What the C++ side gave for one input of `build`: for a C input
    what the preprocessor wrote to stderr, the warnings, and the IR for
    `ir_check`, then the error that ended the response, the `struct.error`
    of writing the object, or the object's bytes."""

    def __init__(self) -> None:
        self.stderr = ""
        self.warnings: list = []
        self.sections: dict[str, str] = {}
        self.error: dict | None = None
        self.write_error: dict | None = None
        self.object: bytes | None = None

    def replay(self) -> None:
        """Writes what the preprocessor wrote to stderr, issues the
        warnings to the caller's `diagnostics.collecting()` block, runs
        `ir_check` on the IR under `C6502_CHECK_IR=1`, and raises the
        input's error as its Python exception, in the order the Python
        compile and assembly give them."""
        import diagnostics
        if self.stderr:
            sys.stderr.write(self.stderr)
        for where, message in self.warnings:
            diagnostics.warn(where, message)
        if self.error is not None:
            raise _exception(self.error["type"], self.error["message"])
        if "tac" in self.sections:
            _check_tac(self.sections["tac"])
            _check_programs(self.sections)

    def write(self, f) -> None:
        """Writes the object to the open file `f`, or raises the
        `struct.error` that Python's `elf.write` raises for it, after
        compile.py has opened the file."""
        if self.write_error is not None:
            raise _exception(self.write_error["type"], self.write_error["message"])
        f.write(self.object)


class Built:
    """The response to `build`: an `InputResult` for each input the C++
    side reached, in order, and for a link its result, one of
    `("link-error", error)`, `("write-error", error)`, `("output-error",
    {"errno", "strerror"})`, and `("executable", size)`, or None when no
    link ran."""

    def __init__(self, data: bytes, link: bool) -> None:
        self.inputs: list[InputResult] = []
        self.link: tuple[str, object] | None = None
        for key, value, body in _records(data):
            if key == "input":
                self.inputs.append(InputResult())
                continue
            # An input of a link has no `write-error`, since compile.py
            # writes no object, so one comes after the inputs.
            if key in ("link-error", "output-error") or (
                    key == "write-error" and link):
                self.link = (key, json.loads(value))
                continue
            if key == "executable":
                self.link = (key, int(value))
                continue
            current = self.inputs[-1]
            if key == "stderr":
                current.stderr = body.decode("utf-8")
            elif key == "warnings":
                current.warnings = json.loads(value)
            elif key in ("tac", "asm", "asm2"):
                current.sections[key] = body.decode("utf-8")
            elif key == "error":
                current.error = json.loads(value)
            elif key == "write-error":
                current.write_error = json.loads(value)
            elif key == "object":
                current.object = body
            elif key != "ok":
                raise RuntimeError(f"c6502cpp backend gave a line {key!r}")


def build(inputs: list[dict], pcpp_args, *, optimize: bool, inline: bool,
          link: dict | None = None) -> Built:
    """Compiles, assembles, and reads `inputs` in the C++ port, as
    `compile._object` does for `-c` (`link` None) or for a link, and
    links them. Each input is a dict with `kind` ("c", "asm", or
    "object"), `name` (the assembler's file name for the object, or an
    object's path), `filename` (a C input's file name for the
    preprocessor, None for stdin), `link_name` (the name the object
    goes by in the link), and `data` (the text, or an object's bytes).
    `link` holds `run_link`, `script`, `origin`, `defsyms` (pairs), and
    `output`. `optimize` and `inline` are as for `compile_source`. The
    C++ side writes the executable itself."""
    check = _ir_check_enabled()
    args = {
        "pcpp_args": list(pcpp_args or []), "include_dir": _INCLUDE_DIR,
        "optimize": optimize, "inline": inline, "check_ir": check,
        "inputs": [
            {k: i[k] for k in ("kind", "name", "filename", "link_name")}
            for i in inputs
        ],
        **(link or {}),
    }
    mode = "objects" if link is None else "link"
    parts = [f"c6502-backend 3\nmode {mode}\nargs {json.dumps(args)}\n".encode("utf-8")]
    for i in inputs:
        data = i["data"]
        if isinstance(data, str):
            data = data.encode("utf-8")
        parts.append(f"input {len(data)}\n".encode("utf-8") + data)
    return Built(_send(b"".join(parts)), link is not None)



def _ir_check_enabled() -> bool:
    """Whether the IR checks are on, as `ir_check.enabled()` says: the
    value of `C6502_CHECK_IR` when passes/ir_check.py was imported, or
    what `ir_check.enable()` set since, as the fuzzers set it. When
    nothing has imported the module, the result is the variable's
    value, since importing the module would load the IRs for every
    compile."""
    module = sys.modules.get("passes.ir_check")
    if module is not None:
        return module.enabled()
    return os.environ.get("C6502_CHECK_IR") == "1"


def run_tool(name: str, argv: list[str]) -> int:
    """Runs the C++ port of a tool, `c6502cpp <name>`, on the command
    line `argv` (the program's name first), as nm.py's `main` does under
    `C6502_CPP_BACKEND=1`, and returns its exit status. What it writes to
    stdout and stderr goes to `sys.stdout` and `sys.stderr`, so a caller
    that redirects them, as a test does, gets it. The C++ side wraps the
    usage and help as argparse does, to the width `shutil` gives here,
    since its own stdout isn't the terminal. A bad command line ends with
    SystemExit(2), as argparse's does, and the help with a return of 0,
    where Python's raises SystemExit(0). argparse writes its usage before
    every error, so an exit status of 2 whose stderr doesn't start with
    `usage: ` comes from the tool itself, as objdump.py's when nothing is
    asked for, and is returned. A bug the C++ side reports (exit status
    3) is raised as an `AssertionError`."""
    import shutil
    from tools.check_captures import cpp_cli
    cli = cpp_cli()
    if cli is None:
        raise RuntimeError(
            "C6502_CPP_BACKEND=1 is set, but c6502cpp isn't built "
            "(cpp/README.md has the build)",
        )
    env = {**os.environ, "COLUMNS": str(shutil.get_terminal_size().columns)}
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
