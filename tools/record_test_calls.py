"""Records the calls that Python unit tests make of the functions the
C++ port checks, for porting those tests to doctest (docs/cpp_port.md,
step 8, "The tests").

    uv run python tools/record_test_calls.py record --out C:/w/rec tests/test_a.py tests/test_b.py
    uv run python tools/record_test_calls.py cpp --out C:/w/rec \\
        --to cpp/tests/test_a_calls.cpp tests/test_a.py

The tests run under pytest in this process. Before they run, every
function that `c6502cpp check-captures` can run is wrapped, wherever a
loaded module holds it: the passes of `capture_passes.PASSES` and
`STAGE_PASSES`, and asm_emit.py's `emit_instruction`, `emit_function`,
and `emit_static_variable` with `apply_y_peephole`, which
cpp/tests/call_case.h checks by their lines. A wrapper records a call
only when its caller is a function of a test module (a module under
tests/), so a pass that another pass or a compile calls isn't
recorded, and the records are the calls each test makes itself.

A call of a pass or a stage call is recorded as tools/capture_passes.py
records it, in the capture format that `capture::parse` reads, with
the test's node ID on the `source` line. A call that raised is
recorded with an `output-error` section, `{"type": ..., "message":
...}`, and raised again. A call of an emission function is recorded as
its input's dump and the lines it returned, or the text of what it
raised.

Two more kinds of call are recorded. A call of `replace_function` or
`replace_function_bare_exit`, the per-function forms of two stage
calls (`_FUNCTION_FORMS`), is recorded as the program-level stage call
on a program of that one function, with the function's own arguments
under its name, once Python's program-level function has given the same
result. A call of tac_to_asm.py's `translate_instruction`,
`translate_function`, `translate_binary`, or `translate_val`
(`_TRANSLATOR`), each of which runs a new `Translator` with no symbol
table, is recorded as the dumps of its arguments and of its result,
which cpp/tests/translator_case.h checks on the C++ `Translator`.

While each test runs, a profile function also lists each other call
that the test's own code makes into a module of the compiler (passes/,
binutils/, rtlib/, and the root's modules other than the IR classes),
leaving out dunder methods and the end-to-end entry points
(`compile.main`, `_run_stage`, `_compile_c`, `nm.main`, and
`objdump.main`, with `preprocess`, whose output those take), which
`END_TO_END` lists separately. A test whose calls are all recorded is
checked by its recorded calls alone, and one with other direct calls
has to be ported by hand. A call whose input or arguments have no dump
or encoding, such as IR a test builds wrongly on purpose, is listed as
unrecordable.

`<out>/index.json` holds, for each test, its outcome, the keys of its
distinct recorded calls in the order it made them, its other direct
calls, and its end-to-end calls. `<out>/records/<key>.json` holds each
call, its kind (`capture`, `lines`, or `translator`), and its text,
lines, or dumps. A second
`record` run into the same directory adds to the index.

`cpp` writes the doctest cases of one module's tests (`write_cpp`),
which cpp/README.md describes ("The unit tests' recorded calls").
"""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
import sys
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))

import pytest  # noqa: E402

from tools import capture_passes as cp  # noqa: E402

# The IR classes and the modules that only read or print them, whose
# calls from a test only build the test's input.
_IR_ONLY = frozenset({
    "asm_ast", "asm2_ast", "tac_ast", "c99_ast", "ir_dump", "pretty",
})

# The root's modules of the compiler, besides passes/, binutils/, and
# rtlib/.
_ROOT_MODULES = frozenset({
    "compile", "asm_emit", "tac_to_asm", "c99_to_tac", "parser", "lexer",
    "preprocessor", "pp_numbers", "bit_precise", "builtin_functions",
    "diagnostics", "fp_arith", "lark_cache", "nm", "objdump",
})

END_TO_END = frozenset({
    "compile.main", "compile._run_stage", "compile._compile_c",
    "nm.main", "objdump.main", "preprocessor.preprocess",
})

# The emission functions that cpp/tests/call_case.h checks by the lines
# they return, by the kind its `check_lines` takes.
_LINES = {
    "emit_instruction": ("asm_emit", "emit_instruction"),
    "emit_function": ("asm_emit", "emit_function"),
    "emit_static_variable": ("asm_emit", "emit_static_variable"),
    "y_peephole": ("passes.y_peephole", "apply_y_peephole"),
}

_TESTS: dict[str, dict] = {}
_CURRENT: str | None = None
_OUT: Path = Path()


def _is_compiler(module: str) -> bool:
    top = module.split(".")[0]
    if module in _IR_ONLY:
        return False
    return top in ("passes", "binutils", "rtlib") or module in _ROOT_MODULES


def _test_entry() -> dict | None:
    if _CURRENT is None:
        return None
    return _TESTS.setdefault(_CURRENT, {
        "outcome": None, "calls": [], "direct": [], "end_to_end": [],
    })


def _called_from_test() -> bool:
    caller = sys._getframe(2).f_globals.get("__name__", "")
    return caller.startswith("tests.") and _CURRENT is not None


def _save(kind: str, body: dict) -> None:
    text = json.dumps({"kind": kind, **body}, sort_keys=True)
    key = hashlib.sha1(text.encode("utf-8")).hexdigest()
    path = _OUT / "records" / f"{key}.json"
    if not path.exists():
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(text.encode("utf-8"))
    entry = _test_entry()
    if key not in entry["calls"]:
        entry["calls"].append(key)


def _record_text(name: str, args_json: str, context: str, input_section: str,
                 output_section: str, mutated: str | None) -> str:
    extra = "" if mutated is None else f"mutated {mutated}\n"
    return (
        f"c6502-capture 2\npass {name}\nsource {_CURRENT}\n"
        f"args {args_json}\ncontext {context}\n{extra}{input_section}"
        f"{output_section}"
    )


def _output_section(result, error) -> str:
    if error is not None:
        return "output-error\n" + json.dumps(
            {"type": type(error).__name__, "message": str(error)}) + "\n"
    if type(result).__module__ in cp._IR_MODULES:
        return "output\n" + cp.dump(result)
    return "output-json\n" + json.dumps(cp.encode(result)) + "\n"


def _input_section(value) -> str:
    if type(value).__module__ in cp._IR_MODULES:
        return "input\n" + cp.dump(value)
    return "input-json\n" + json.dumps(cp.encode(value)) + "\n"


def _unrecordable(name: str, e: Exception) -> None:
    """Notes that a test's call of `name` couldn't be recorded, because
    its input or result has no dump (a test that builds IR the dump
    format can't hold, such as a `StaticVariable` with one `IntInit` in
    place of a list)."""
    entry = _test_entry()
    note = f"{name}: {type(e).__name__}: {e}"
    if note not in entry.setdefault("unrecordable", []):
        entry["unrecordable"].append(note)


def _call(fn, args, kwargs):
    """Calls `fn` and returns its result and None, or None and the
    exception it raised."""
    try:
        return fn(*args, **kwargs), None
    except Exception as e:  # noqa: BLE001 - recorded and raised again
        return None, e


def _wrap_pass(name: str, fn):
    """Wraps a pass of `PASSES`, recorded as `capture_passes._wrap`
    records it."""
    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        try:
            rec_args, rec_kwargs = cp._RECORDED_ARGS.get(
                name, lambda a, k: (a, k))(args, kwargs)
            input_section = _input_section(rec_args[0])
            args_json = json.dumps({
                "args": [cp.encode(a) for a in rec_args[1:]],
                "kwargs": {k: cp.encode(v) for k, v in rec_kwargs.items()
                           if k != "memo"},
            })
            context = cp.context_json()
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(name, e)
            return fn(*args, **kwargs)
        result, error = _call(fn, args, kwargs)
        try:
            _save("capture", {"text": _record_text(
                name, args_json, context, input_section,
                _output_section(result, error), None)})
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(name, e)
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


def _wrap_stage(name: str, fn):
    """Wraps a call of `STAGE_PASSES`, recorded as
    `capture_passes._wrap_stage` records it, with the arguments it
    changes in place on the `mutated` line."""
    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        rest = args[1:]
        try:
            context = cp.context_json()
            input_text = _input_section(args[0])
            before = cp._encode_call(rest, kwargs)
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(name, e)
            return fn(*args, **kwargs)
        if name in cp._COPIES_INPUT:
            result, error = _call(fn, (copy.deepcopy(args[0]), *rest), kwargs)
        else:
            result, error = _call(fn, args, kwargs)
        try:
            mutated = None
            if error is None:
                after = cp._encode_call(rest, kwargs)
                changed_args = {
                    str(i): a
                    for i, (a, b) in enumerate(zip(after[0], before[0]))
                    if a != b
                }
                changed_kwargs = {
                    k: v for k, v in after[1].items() if v != before[1][k]
                }
                input_after = None
                if _input_section(args[0]) != input_text:
                    input_after = json.dumps(cp.encode(args[0]))
                mutated = cp._mutated_json(changed_args, changed_kwargs,
                                           input_after)
            _save("capture", {"text": _record_text(
                name, cp._call_json(*before), context, input_text,
                _output_section(result, error), mutated)})
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(name, e)
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


def _wrap_lines(kind: str, fn):
    """Wraps an emission function of `_LINES`, recorded as its input and
    the lines it returned or the text of what it raised."""
    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        try:
            node = args[0]
            if kind == "y_peephole":
                given = {"lines": list(node)}
            else:
                given = {"input": cp.dump(node)}
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(kind, e)
            return fn(*args, **kwargs)
        result, error = _call(fn, args, kwargs)
        if error is not None:
            expected = [f"exception: {type(error).__name__}: {error}\n"]
        else:
            expected = list(result)
        _save("lines", {"name": kind, **given, "expected": expected})
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


# The per-function forms of two stage calls, which `check-captures` runs
# only on a whole program: each name with its module, the stage call,
# and the program-level function. The tests call them on one function.
_FUNCTION_FORMS = {
    "replace_function": (
        "passes.replace_pseudoregisters", "replace_pseudoregisters",
        "replace_program"),
    "replace_function_bare_exit": (
        "passes.replace_pseudoregisters", "replace_pseudoregisters_bare_exit",
        "replace_program_bare_exit"),
}

# The arguments of the per-function forms, each with the keyword of the
# program-level function that takes it and whether that keyword holds a
# dict by function name. They are in the order of the program-level
# function's parameters, which is the order of the record's `kwargs`.
_PROGRAM_ARGUMENTS = (
    ("statics", "extra_statics", False),
    ("symbols", "symbols", False),
    ("types", "types", False),
    ("coloring", "colorings", True),
    ("param_layout", "param_layouts", True),
    ("private_pool_addrs", "local_pools", True),
    ("address_taken_zp", "address_taken_assignments", True),
    ("address_taken_symbols", "address_taken_symbols", True),
)


def _program_kwargs(fn_name: str, given: dict) -> dict:
    """The keyword arguments of the program-level call that gives the
    function `fn_name` the arguments `given` of its per-function form.
    An argument a test left at its default is left out, as step 4's part
    F2 recorded these calls (cpp/tests/test_frames_stage_calls.cpp)."""
    out = {}
    for name, keyword, by_function in _PROGRAM_ARGUMENTS:
        if name not in given:
            continue
        value = given[name]
        if name == "private_pool_addrs":
            if not value:
                continue
            value = sorted(value)
        elif value is None or (name == "statics" and not value):
            continue
        out[keyword] = {fn_name: value} if by_function else value
    return out


def _wrap_function_form(name: str, fn):
    """Wraps a per-function form of `_FUNCTION_FORMS`. A call is recorded
    as the program-level stage call on a program of that one function,
    with the result Python's program-level function gives, which is
    checked to be the per-function result first. A call whose two
    results differ is noted as unrecordable."""
    import importlib
    import inspect
    import asm_ast
    module, stage, program_name = _FUNCTION_FORMS[name]
    signature = inspect.signature(fn)

    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        try:
            bound = signature.bind(*args, **kwargs)
            func = bound.arguments["fn"]
            given = {k: v for k, v in bound.arguments.items() if k != "fn"}
            prog = asm_ast.Program(top_level=[func])
            prog_kwargs = _program_kwargs(func.name, given)
            context = cp.context_json()
            input_section = _input_section(prog)
            args_json = cp._call_json(*cp._encode_call([], prog_kwargs))
            program_fn = getattr(importlib.import_module(module), program_name)
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(name, e)
            return fn(*args, **kwargs)
        result, error = _call(fn, args, kwargs)
        prog_result, prog_error = _call(
            program_fn, (copy.deepcopy(prog),), copy.deepcopy(prog_kwargs))
        try:
            if error is None:
                if prog_error is not None:
                    raise ValueError(f"the program-level call raised {prog_error!r}")
                if stage == "replace_pseudoregisters":
                    same = prog_result.top_level == [result]
                else:
                    out_prog, dims = prog_result
                    same = (out_prog.top_level == [result[0]]
                            and dims == {func.name: result[1]})
                if not same:
                    raise ValueError("the program-level call gives another result")
            elif prog_error is None or (type(prog_error), str(prog_error)) != (
                    type(error), str(error)):
                raise ValueError("the program-level call doesn't raise the same error")
            _save("capture", {"text": _record_text(
                stage, args_json, context, input_section,
                _output_section(prog_result, prog_error), None)})
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(name, e)
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


# tac_to_asm.py's module-level functions that run one method of a new
# `Translator` with no symbol table, which cpp/tests/translator_case.h
# runs on the C++ `Translator`.
_TRANSLATOR = (
    "translate_instruction", "translate_function", "translate_binary",
    "translate_val",
)


def _wrap_translator(name: str, fn):
    """Wraps a function of `_TRANSLATOR`, recorded as the dumps of its
    arguments and of its result's instructions (or its one operand or
    function), or the text of what it raised."""
    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        try:
            if kwargs:
                raise TypeError("keyword arguments")
            inputs = [cp.dump(a) for a in args]
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(name, e)
            return fn(*args, **kwargs)
        result, error = _call(fn, args, kwargs)
        try:
            if error is not None:
                expected = [f"exception: {type(error).__name__}: {error}\n"]
            elif isinstance(result, list):
                expected = [cp.dump(i) for i in result]
            else:
                expected = [cp.dump(result)]
            _save("translator", {"name": name, "inputs": inputs,
                                 "expected": expected})
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(name, e)
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


def _targets() -> dict[int, tuple[object, object]]:
    """The functions to wrap, by id, each with its wrapper."""
    import importlib
    out: dict[int, tuple[object, object]] = {}
    tac_to_asm = importlib.import_module("tac_to_asm")
    for name in _TRANSLATOR:
        fn = getattr(tac_to_asm, name)
        out.setdefault(id(fn), (fn, _wrap_translator(name, fn)))
    for name, (module, _stage, _program) in _FUNCTION_FORMS.items():
        fn = getattr(importlib.import_module(module), name)
        out.setdefault(id(fn), (fn, _wrap_function_form(name, fn)))
    for name in cp.PASSES:
        module, attr = cp.resolve(cp.PASSES[name])
        fn = getattr(module, attr)
        out.setdefault(id(fn), (fn, _wrap_pass(name, fn)))
    for name, target in cp.STAGE_PASSES.items():
        module, attr = cp.resolve(target)
        fn = getattr(module, attr)
        out.setdefault(id(fn), (fn, _wrap_stage(name, fn)))
    for kind, (module, attr) in _LINES.items():
        fn = getattr(importlib.import_module(module), attr)
        out.setdefault(id(fn), (fn, _wrap_lines(kind, fn)))
    return out


def install() -> None:
    """Puts the wrappers in place of the functions in every loaded
    module that holds one."""
    targets = _targets()
    for module in list(sys.modules.values()):
        try:
            items = list(vars(module).items())
        except TypeError:
            continue
        for attr, value in items:
            hit = targets.get(id(value))
            if hit is not None and hit[0] is value:
                setattr(module, attr, hit[1])


def _profile(frame, event, arg):
    if event != "call" or _CURRENT is None:
        return
    caller = frame.f_back
    if caller is None:
        return
    if not caller.f_globals.get("__name__", "").startswith("tests."):
        return
    module = frame.f_globals.get("__name__", "")
    if not _is_compiler(module):
        return
    code = frame.f_code
    name = code.co_qualname
    if code.co_name.startswith("__") or code.co_name == "<lambda>":
        return
    full = f"{module}.{name}"
    entry = _test_entry()
    bucket = "end_to_end" if full in END_TO_END else "direct"
    if full not in entry[bucket]:
        entry[bucket].append(full)


class _Plugin:
    def pytest_collection_finish(self, session):
        install()

    @pytest.hookimpl(tryfirst=True)
    def pytest_runtest_setup(self, item):
        global _CURRENT
        _CURRENT = item.nodeid
        _test_entry()
        sys.setprofile(_profile)

    def pytest_runtest_teardown(self, item):
        global _CURRENT
        sys.setprofile(None)
        _CURRENT = None

    def pytest_runtest_logreport(self, report):
        entry = _TESTS.setdefault(report.nodeid, {
            "outcome": None, "calls": [], "direct": [], "end_to_end": [],
        })
        if report.when == "call" or report.outcome != "passed":
            if entry["outcome"] in (None, "passed"):
                entry["outcome"] = report.outcome


# --- Writing the doctest cases ---

# The longest piece of text in one C++ string literal. MSVC refuses a
# literal of more than about 16 KB, and a concatenation of more than
# 64 KB, so a record's text is split into pieces that
# `c6502::testing::text` joins.
_LITERAL_LINE = 4000
_PIECE = 30000


def _c_literals(text: str) -> list[str]:
    """The C++ string literals whose concatenation is `text`, one per
    line of it, with a long line split."""
    out = []
    data = text.encode("utf-8")
    line = bytearray()

    def flush():
        if line:
            out.append('"' + line.decode("ascii") + '"')
            line.clear()

    for b in data:
        c = chr(b)
        if c == "\\":
            line += b"\\\\"
        elif c == '"':
            line += b'\\"'
        elif c == "\n":
            line += b"\\n"
            flush()
            continue
        elif 32 <= b < 127 and c != "?":
            line.append(b)
        else:
            # An octal escape takes at most three digits, so the
            # character after it can't extend it (and `?` can't start a
            # trigraph).
            line += f"\\{b:03o}".encode("ascii")
        if len(line) >= _LITERAL_LINE:
            flush()
    flush()
    return out


def _text_expr(text: str, indent: str) -> str:
    """A C++ expression whose value is `text`: adjacent literals, or a
    call of `text` on pieces when it is longer than one concatenation
    can be."""
    literals = _c_literals(text)
    pieces: list[list[str]] = [[]]
    size = 0
    for lit in literals:
        if size + len(lit) > _PIECE and pieces[-1]:
            pieces.append([])
            size = 0
        pieces[-1].append(lit)
        size += len(lit)
    if len(pieces) == 1:
        return f"\n{indent}".join(pieces[0])
    inner = indent + "    "
    parts = [f"\n{inner}".join(p) for p in pieces]
    return ("c6502::testing::text({\n" + inner
            + f",\n{inner}".join(parts) + "})")


_LITERAL_RE = None


def _cpp_case_texts(cpp_dir: Path, exclude: Path) -> list[str]:
    """The text of the string literals of each TEST_CASE in the C++
    tests, decoded and joined, for finding a recorded call that a case
    already checks."""
    import re
    global _LITERAL_RE
    if _LITERAL_RE is None:
        _LITERAL_RE = re.compile(r'R"([^(\s]*)\((.*?)\)\1"|"((?:[^"\\\n]|\\.)*)"',
                                 re.S)
    out = []
    for path in sorted(cpp_dir.glob("*.cpp")):
        if path.resolve() == exclude.resolve():
            continue
        src = path.read_text(encoding="utf-8")
        for body in src.split("TEST_CASE(")[1:]:
            title = body.split('"', 2)[1]
            chunks = []
            for m in _LITERAL_RE.finditer(body):
                if m.group(2) is not None:
                    chunks.append(m.group(2))
                else:
                    chunks.append(_decode_c(m.group(3)))
            out.append((f"{path.name}: {title}", "".join(chunks)))
    return out


def _decode_c(s: str) -> str:
    out = bytearray()
    i = 0
    simple = {"n": b"\n", "t": b"\t", "\\": b"\\", '"': b'"', "'": b"'",
              "?": b"?", "r": b"\r", "0": b"\0"}
    while i < len(s):
        c = s[i]
        if c != "\\":
            out += c.encode("utf-8")
            i += 1
            continue
        n = s[i + 1]
        if n in "01234567":
            j = i + 1
            while j < len(s) and j < i + 4 and s[j] in "01234567":
                j += 1
            out.append(int(s[i + 1:j], 8))
            i = j
        elif n == "x":
            j = i + 2
            while j < len(s) and s[j] in "0123456789abcdefABCDEF":
                j += 1
            out.append(int(s[i + 2:j], 16) & 0xFF)
            i = j
        else:
            out += simple.get(n, n.encode("utf-8"))
            i += 2
    return out.decode("utf-8", errors="replace")


def _record_parts(record: dict) -> list[str]:
    """The pieces of a record that a C++ case checking the same call
    holds among its literals: the function's name, its arguments, the input,
    the expected result, and the arguments it changed."""
    if record["kind"] == "lines":
        given = record.get("input")
        if given is None:
            given = "".join(record["lines"])
        return [record["name"], given, *record["expected"]]
    if record["kind"] == "translator":
        return [record["name"], *record["inputs"], *record["expected"]]
    text = record["text"]
    lines = text.split("\n")
    name = lines[1][len("pass "):]
    head, _, rest = text.partition("\ninput")
    if rest.startswith("-json\n"):
        rest = rest[len("-json\n"):]
    else:
        rest = rest[1:]
    for marker in ("output-error\n", "output-json\n", "output\n"):
        before, sep, after = rest.partition(marker)
        if sep:
            given, expected = before, after
            break
    if expected.endswith("\n") and marker != "output\n":
        expected = expected[:-1]
    parts = [name, lines[3][len("args "):], given, expected]
    mutated = [x for x in lines if x.startswith("mutated ")]
    if mutated:
        parts.append(mutated[0][len("mutated "):])
    return parts


def _case(test: str, n: int, record: dict) -> str:
    cls_test = test.split("::", 1)[1]
    module = Path(test.split("::")[0]).name
    if record["kind"] == "translator":
        name = record["name"]
        title = f"{name}: {module}::{cls_test} (call {n})"
        inputs = ",\n".join(
            "        " + _text_expr(x, "        ") for x in record["inputs"])
        expected = ",\n".join(
            "        " + _text_expr(x, "        ") for x in record["expected"])
        return (f'TEST_CASE("{title}") {{\n'
                f"  c6502::testing::check_translator(\n"
                f'      "{name}",\n'
                f"      {{\n{inputs}\n      }},\n"
                f"      {{\n{expected}\n      }});\n}}\n")
    if record["kind"] == "lines":
        name = record["name"]
        title = f"{name}: {module}::{cls_test} (call {n})"
        if name == "y_peephole":
            lines = ",\n".join(
                "        " + _text_expr(x, "        ") for x in record["lines"])
            expected = ",\n".join(
                "        " + _text_expr(x, "        ") for x in record["expected"])
            return (f'TEST_CASE("{title}") {{\n'
                    f"  c6502::testing::check_y_peephole(\n"
                    f"      {{\n{lines}\n      }},\n"
                    f"      {{\n{expected}\n      }});\n}}\n")
        expected = ",\n".join(
            "        " + _text_expr(x, "        ") for x in record["expected"])
        return (f'TEST_CASE("{title}") {{\n'
                f"  c6502::testing::check_lines(\n"
                f'      "{name}",\n'
                f"      {_text_expr(record['input'], '      ')},\n"
                f"      {{\n{expected}\n      }});\n}}\n")
    name = record["text"].split("\n")[1][len("pass "):]
    title = f"{name}: {module}::{cls_test} (call {n})"
    return (f'TEST_CASE("{title}") {{\n'
            f"  c6502::testing::check_record(\n"
            f"      {_text_expr(record['text'], '      ')});\n}}\n")


def write_cpp(out: Path, module: str, target: Path, cpp_dir: Path,
              only: set[str] | None = None) -> dict:
    """Writes `target`, the doctest cases of the recorded calls of the
    tests of `module` (a path such as tests/test_carry_halve.py), or of
    those in `only`, that no other C++ case checks, and returns, for each
    test, its calls with the case that checks each."""
    index = json.loads((out / "index.json").read_text(encoding="utf-8"))
    existing = _cpp_case_texts(cpp_dir, target)
    cases = []
    written: dict[str, str] = {}
    report = {}
    for test, entry in index.items():
        if not test.startswith(module + "::"):
            continue
        if only is not None and test not in only:
            continue
        rows = []
        for n, key in enumerate(entry["calls"], 1):
            record = json.loads(
                (out / "records" / f"{key}.json").read_text(encoding="utf-8"))
            if key in written:
                rows.append((key, "same as " + written[key]))
                continue
            parts = _record_parts(record)
            hit = next((where for where, t in existing
                        if all(p in t for p in parts)), None)
            if hit is not None:
                rows.append((key, "checked by " + hit))
                continue
            text = _case(test, n, record)
            title = text.split('"', 2)[1]
            written[key] = title
            cases.append(text)
            rows.append((key, title))
        report[test] = {**entry, "cases": rows}
    header = (
        f"// The calls that the tests of {module} made of the asm passes,\n"
        "// code generation's stage calls, and emission, each with what the\n"
        "// Python function returned or raised, which the C++ port must\n"
        "// return or raise too. tools/record_test_calls.py recorded them\n"
        "// while the Python tests ran, before they were removed. A call\n"
        "// that another C++ case already checks isn't repeated here.\n"
        '#include "doctest.h"\n'
        '#include "record_case.h"\n'
    )
    if any("check_translator(" in c for c in cases):
        header += '#include "translator_case.h"\n'
    if cases:
        target.write_bytes((header + "\n" + "\n".join(cases)).encode("utf-8"))
    return report


def main(argv: list[str] | None = None) -> int:
    global _OUT
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="command", required=True)
    rec = sub.add_parser("record", help="run tests and record their calls")
    rec.add_argument("--out", required=True)
    rec.add_argument("tests", nargs="+")
    cpp = sub.add_parser("cpp", help="write the doctest cases of a module")
    cpp.add_argument("--out", required=True)
    cpp.add_argument("--to", required=True,
                     help="the .cpp file to write")
    cpp.add_argument("--report", help="where to write the JSON report")
    cpp.add_argument("--only", help="a file of the node IDs of the tests "
                     "to write, one to a line")
    cpp.add_argument("module", help="a test module, such as tests/test_x.py")
    args = ap.parse_args(argv)
    _OUT = Path(args.out)
    if args.command == "cpp":
        only = None
        if args.only:
            only = set(Path(args.only).read_text(encoding="utf-8").split())
        report = write_cpp(_OUT, args.module, Path(args.to),
                           _ROOT / "cpp" / "tests", only)
        if args.report:
            Path(args.report).write_bytes(
                json.dumps(report, indent=1).encode("utf-8"))
        for test, entry in report.items():
            print(test, entry["outcome"],
                  f"calls={len(entry['calls'])}",
                  f"direct={entry['direct']}" if entry["direct"] else "",
                  f"e2e={entry['end_to_end']}" if entry["end_to_end"] else "")
            for key, where in entry["cases"]:
                print("   ", key[:10], where)
        return 0
    _OUT.mkdir(parents=True, exist_ok=True)
    status = pytest.main(
        ["-q", "-p", "no:cacheprovider", "-p", "no:randomly", *args.tests],
        plugins=[_Plugin()],
    )
    index = _OUT / "index.json"
    old = json.loads(index.read_text(encoding="utf-8")) if index.exists() else {}
    old.update(_TESTS)
    index.write_bytes(json.dumps(old, indent=1).encode("utf-8"))
    return int(status)


if __name__ == "__main__":
    sys.exit(main())
