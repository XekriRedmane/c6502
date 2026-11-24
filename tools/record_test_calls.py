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

Group G7 added the calls of the TAC side. A call of the `run` of a pass
of the TAC optimizer's driver, from a test or through the entry
functions of passes/optimization/, is recorded as capture_passes.py
records the driver's call (`_wrap_tac_run`), a module-level function
that does a pass's work itself as the run of its module's pass that
gives the same result (`_wrap_tac_function`), and a `RuleSet` of
jump-fold rules that the driver doesn't run as a `rules` record. The
stage calls of `TAC_STAGE_PASSES`, `to_ssa` and `from_ssa`, and the front
end's passes after the preprocessor are recorded as their captures are.
The calls a test makes of one `Translator` of c99_to_tac.py, or of one
of tac_to_asm.py's made with a symbol table, are one session record
(`c99_session`, `asm_session`), the dumps of each call's arguments and
results in order, which cpp/tests/c99_session_case.h and
translator_case.h replay on one C++ `Translator`. A test's replacement
of row_address_tables.py's `_saving` is recorded as the keyword `saving`
of the `row_address_tables` call.

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
call, its kind (`capture`, `lines`, `translator`, `rules`,
`c99_session`, or `asm_session`), and its text, lines, or dumps. A second
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


def _as_symbol_table(value):
    """`_as_table` of a plain dict of `Symbol`s, which a test gives a
    stage call in place of a symbol table, and any other value as it
    is."""
    if type(value) is dict and value:
        from passes.type_checking import Symbol
        if all(isinstance(v, Symbol) for v in value.values()):
            return _as_table(value)
    return value


def _replaced_saving():
    """The bytes a lookup saves when a test has replaced
    row_address_tables.py's `_saving` with a function of its own, which
    the tests make return one number whatever the row, or None when it's
    the pass's own. `row_address_tables`'s record then holds it as the
    keyword `saving`, which the C++ stage call takes in place of the
    estimate."""
    from passes.optimization import row_address_tables as rat
    if rat._saving is _ORIGINAL_SAVING[0]:
        return None
    values = {rat._saving(row) for row in range(1, 257)}
    if len(values) != 1:
        raise ValueError("the replaced `_saving` depends on the row")
    return values.pop()


_ORIGINAL_SAVING: list = []


def _with_defaults(name: str, args: tuple, kwargs: dict):
    """The arguments of a stage call as the compile gives them, for a
    test that leaves out one the C++ stage call takes. `translate_program`
    of c99_to_tac.py type-checks the program itself when it's given no
    symbol table, so that type check is made here, and recorded as a
    `type_checking` call, and the tables it gives are the call's. A call
    of the TAC optimizer's `optimize_function` without `symbols` is
    recorded with `symbols=None`, its default."""
    if name == "c99_to_tac" and len(args) < 2 and "symbols" not in kwargs:
        from passes.type_checking import check_program
        result, error, texts, failure = _capture_texts(
            lambda: cp._wrap_front("type_checking", check_program)(args[0]))
        _save_texts("type_checking", texts, failure)
        if error is not None:
            raise error
        _prog, symbols, types = result
        return (args[0], symbols, types), kwargs
    if name == "tac_optimize_function" and "symbols" not in kwargs:
        return args, {**kwargs, "symbols": None}
    return args, kwargs


def _wrap_stage(name: str, fn):
    """Wraps a call of `STAGE_PASSES`, recorded as
    `capture_passes._wrap_stage` records it, with the arguments it
    changes in place on the `mutated` line."""
    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        args = tuple(_as_symbol_table(a) for a in args)
        kwargs = {k: _as_symbol_table(v) for k, v in kwargs.items()}
        args, kwargs = _with_defaults(name, args, kwargs)
        rest = args[1:]
        try:
            context = cp.context_json()
            input_text = _input_section(args[0])
            before = cp._encode_call(rest, kwargs)
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(name, e)
            return fn(*args, **kwargs)
        saving = _replaced_saving() if name == "row_address_tables" else None
        if name in cp._COPIES_INPUT:
            result, error = _call(fn, (copy.deepcopy(args[0]), *rest), kwargs)
        else:
            result, error = _call(fn, args, kwargs)
        if saving is not None:
            before = (before[0], {**before[1], "saving": json.dumps(saving)})
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


def _capture_texts(call):
    """Runs `call()` with capture_passes.py's `_record` writing each
    record's text to a list in place of a file, and returns what `call`
    returned or raised, the texts, and the failure that capture_passes
    noted, if any. The texts are those `_record` writes, with the test's
    node ID on the `source` line."""
    texts: list[str] = []

    def record(name, input_section, args_json, result, mutated=None,
               context=None, error=None, warnings=None, stderr=None):
        if context is None:
            context = cp.context_json()
        if error is not None:
            output = "output-error\n" + json.dumps(
                {"type": type(error).__name__, "message": str(error)}) + "\n"
        elif type(result).__module__ in cp._IR_MODULES:
            output = "output\n" + cp.dump(result)
        else:
            output = "output-json\n" + json.dumps(cp.encode(result)) + "\n"
        extra = "" if mutated is None else f"mutated {mutated}\n"
        if warnings is not None:
            extra += f"warnings {warnings}\n"
        if stderr is not None:
            extra += f"stderr {json.dumps(stderr)}\n"
        texts.append(
            f"c6502-capture 2\npass {name}\nsource {_CURRENT}\n"
            f"args {args_json}\ncontext {context}\n{extra}{input_section}"
            f"{output}")

    saved = cp._record
    cp._record = record
    cp._FAILURES.clear()
    try:
        result, error = _call(call, (), {})
    finally:
        cp._record = saved
    failure = next(iter(cp._FAILURES.values()), None)
    cp._FAILURES.clear()
    return result, error, texts, failure


def _save_texts(name: str, texts: list[str], failure, kind: str = "capture",
                **extra) -> None:
    """Saves the records `texts` of a call of `name`, and notes the
    failure capture_passes.py met recording it."""
    for text in texts:
        _save(kind, {"text": text, **extra})
    if failure is not None:
        _unrecordable(name, ValueError(failure))


def _wrap_front(name: str, fn):
    """Wraps a front-end call of `capture_passes.FRONT`, recorded as
    `capture_passes._wrap_front` records it, with its warnings, what it
    wrote to stderr, and the exception it raised."""
    cp_wrapper = cp._wrap_front(name, fn)

    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        result, error, texts, failure = _capture_texts(
            lambda: cp_wrapper(*args, **kwargs))
        _save_texts(name, texts, failure)
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


def _called_through_tac_passes() -> bool:
    """Whether a pass's `run` was called by a test, either itself or
    through the entry functions and the framework of
    passes/optimization/, with no other recorded call between."""
    if _CURRENT is None:
        return False
    frame = sys._getframe(2)
    while frame is not None:
        module = frame.f_globals.get("__name__", "")
        if (not module.startswith("passes.optimization")
                and frame.f_code not in _TRANSPARENT):
            return module.startswith("tests.")
        frame = frame.f_back
    return False


# The code of the wrappers of `_tac_functions`, through which a test's
# call reaches a pass's `run` as if the test made it.
_TRANSPARENT: set = set()


# The jump-fold rules that a test runs in a `RuleSet` of its own, by the
# name of their `JumpFoldRule` in cpp/src/passes/optimization/
# fold_producer_jumps.h, which cpp/tests/tac_rules_case.h runs.
_JUMP_FOLD_RULES = (
    ("passes.optimization.cmp_zero_jump_fold", "CMP_ZERO_RULE", "kCmpZero"),
    ("passes.optimization.cmp_zero_jump_fold", "NARROW_VALUE_COMPARE_RULE",
     "kNarrowValueCompare"),
    ("passes.optimization.cmp_zero_jump_fold", "NARROW_JUMP_COMPARE_RULE",
     "kNarrowJumpCompare"),
    ("passes.optimization.and_zero_jump_fold", "AND_ZERO_RULE", "kAndZero"),
    ("passes.optimization.lnot_jump_fold", "LNOT_RULE", "kLnot"),
    ("passes.optimization.lnot_jump_fold", "LNOT_OF_ZERO_OR_ONE_RULE",
     "kLnotOfZeroOrOne"),
    ("passes.optimization.nonnegative_compare", "NONNEGATIVE_COMPARE_RULE",
     "kNonnegativeCompare"),
)


def _tac_pass_classes():
    """The classes of the TAC optimizer driver's passes, each with a
    function that gives the capture name of a call of one of its
    instances, or None when the instance is no pass of the driver: the
    driver's name for the pass, `tac_<name>`, and for a `RuleSet` of
    jump-fold rules that the driver doesn't run, `jumprules` and the
    rules' names, which `_case` writes as a `check_rules_case`."""
    import importlib
    from passes.optimization.framework.rules import RuleSet
    from passes.optimization.optimizer import _DRIVER
    rule_names = {
        id(getattr(importlib.import_module(m), attr)): enum
        for m, attr, enum in _JUMP_FOLD_RULES
    }
    driver_rules = {}
    classes = {}
    for slot in cp._DRIVER_SLOTS:
        for p in getattr(_DRIVER, slot):
            if isinstance(p, RuleSet):
                driver_rules[tuple(id(r) for r in p.rules)] = p.name
            else:
                classes[type(p)] = p.name

    def rule_set_name(p):
        key = tuple(id(r) for r in p.rules)
        if key in driver_rules:
            return f"{cp.TAC_PREFIX}{driver_rules[key]}"
        if all(k in rule_names for k in key):
            return "jumprules " + ",".join(rule_names[k] for k in key)
        return None

    out = {cls: (lambda p, n=name: f"{cp.TAC_PREFIX}{n}")
           for cls, name in classes.items()}
    out[RuleSet] = rule_set_name
    return out


def _as_table(symbols):
    """A `SymbolTable` over `symbols` when a test gives a pass a plain
    dict, which the passes read through the same methods, so that the
    record holds a symbol table. The table shares the dict, so the
    test sees what the pass adds."""
    if type(symbols) is not dict:
        return symbols
    from passes.type_checking import SymbolTable
    table = SymbolTable()
    table._table = symbols
    return table


def _wrap_tac_run(cls, name_of):
    """Wraps the `run` method of a TAC pass class. A call that a test
    makes, itself or through an entry function such as
    `constant_fold`, is recorded as `capture_passes._wrap_tac_pass`
    records the driver's call of the pass, with the symbols the call
    read, the SSA names, and the ones it changed."""
    run = cls.run

    def wrapper(self, fn, ctx):
        if not _called_through_tac_passes():
            return run(self, fn, ctx)
        name = name_of(self)
        if name is None:
            _unrecordable(f"{type(self).__name__}.run",
                          ValueError("no pass of the driver"))
            return run(self, fn, ctx)
        bound = run.__get__(self, cls)
        given = ctx.symbols
        ctx.symbols = _as_table(given)
        try:
            result, error, texts, failure = _capture_texts(
                lambda: cp._wrap_tac_pass(name, bound)(fn, ctx))
        finally:
            ctx.symbols = given
        if error is not None:
            _unrecordable(name, error)
            raise error
        if name.startswith("jumprules "):
            _save_texts(name, texts, failure, kind="rules",
                        rules=name.split(" ", 1)[1].split(","))
        else:
            _save_texts(name, texts, failure)
        for runs in _NESTED_RUNS.values():
            runs.append((fn, result))
        return result
    wrapper.__wrapped_pass__ = run
    return wrapper


def _wrap_tac_ssa(name: str, fn):
    """Wraps `to_ssa` or `from_ssa`, recorded as
    `capture_passes._wrap_tac_ssa` records the driver's calls of them."""
    cp_wrapper = cp._wrap_tac_ssa(name, fn)

    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        if len(args) > 1:
            args = (args[0], _as_table(args[1]), *args[2:])
        if "symbols" in kwargs:
            kwargs = {**kwargs, "symbols": _as_table(kwargs["symbols"])}
        result, error, texts, failure = _capture_texts(
            lambda: cp_wrapper(*args, **kwargs))
        if error is not None:
            _unrecordable(name, error)
            raise error
        _save_texts(name, texts, failure)
        return result
    wrapper.__wrapped_pass__ = fn
    return wrapper


# The parameters through which the module-level function of a pass may
# take what the driver gives the pass in its `PassContext`.
_CONTEXT_PARAMETERS = {"fn", "ctx", "symbols", "ssa_dsts", "ssa_names"}


def _tac_functions() -> dict[int, tuple[object, list]]:
    """The module-level functions of the driver's passes that do the
    pass's work themselves, `eliminate_dead_stores(fn, ssa_dsts=...)`
    for one, by id, each with the driver passes of its module, in the
    driver's order: the function and its list of (capture name, class)."""
    import importlib
    import inspect
    from passes.optimization.framework.rules import RuleSet
    from passes.optimization.optimizer import _DRIVER
    # The modules of the jump-fold rules and of strength reduction, whose
    # functions run a `RuleSet`, which `_wrap_tac_run` records.
    by_module: dict[str, list] = {
        m: [] for m, _attr, _enum in _JUMP_FOLD_RULES
    }
    by_module["passes.optimization.strength_reduction"] = []
    for slot in cp._DRIVER_SLOTS:
        for p in getattr(_DRIVER, slot):
            if isinstance(p, RuleSet):
                continue
            by_module.setdefault(type(p).__module__, []).append(
                (f"{cp.TAC_PREFIX}{p.name}", type(p)))
    out = {}
    for module_name, passes in by_module.items():
        module = importlib.import_module(module_name)
        for attr, f in vars(module).items():
            if (not inspect.isfunction(f) or f.__module__ != module_name
                    or attr.startswith("_")):
                continue
            params = list(inspect.signature(f).parameters)
            if params[:1] != ["fn"] or not set(params) <= _CONTEXT_PARAMETERS:
                continue
            out[id(f)] = (f, passes)
    return out


def _wrap_tac_function(fn, passes):
    """Wraps a function of `_tac_functions`. A call that a test makes is
    recorded as the driver's call of the first pass of the function's
    module whose `run`, on copies of the function and of the symbols and
    SSA names the call gives, returns what the function returned and
    changes the copies as the function changed the originals. A call
    that no pass reproduces is noted as unrecordable."""
    import inspect
    from passes.optimization.framework import PassContext
    signature = inspect.signature(fn)

    def wrapper(*args, **kwargs):
        if not _called_from_test():
            return fn(*args, **kwargs)
        bound = signature.bind(*args, **kwargs)
        bound.apply_defaults()
        given = bound.arguments
        if "ctx" in given:
            symbols, ssa = given["ctx"].symbols, given["ctx"].ssa_dsts
        else:
            symbols = given.get("symbols")
            ssa = given.get("ssa_dsts", given.get("ssa_names"))
        func = given["fn"]
        before = copy.deepcopy((func, symbols, ssa))
        runs = _NESTED_RUNS.setdefault(id(wrapper), [])
        runs.clear()
        result = fn(*args, **kwargs)
        _NESTED_RUNS.pop(id(wrapper), None)
        if runs:
            # The function called the `run` of a driver pass, which was
            # recorded. Its records check the function's whole work when
            # the one run took the function's input and gave its result.
            if len(runs) != 1 or runs[0][0] != before[0] or runs[0][1] != result:
                _unrecordable(fn.__name__, ValueError(
                    "its runs of a pass aren't its whole work"))
            return result
        try:
            after = json.dumps([cp.encode(_as_table(symbols)), cp.encode(ssa)])
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(fn.__name__, e)
            return result
        # The function may run the pass with every name it defines as
        # its SSA names, as `recognize_indirect_indexed` does.
        from passes.optimization.extend_add_chain import all_dsts
        ssa_forms = [False] if ssa is not None else [False, True]
        for (name, cls), every_dst in [(p, f) for p in passes for f in ssa_forms]:
            c_func, c_symbols, c_ssa = copy.deepcopy(before)
            if every_dst:
                c_ssa = all_dsts(c_func)
            ctx = PassContext(symbols=_as_table(c_symbols), ssa_dsts=c_ssa)
            got, error, texts, failure = _capture_texts(
                lambda: cp._wrap_tac_pass(name, cls().run)(c_func, ctx))
            if error is not None or got != result:
                continue
            got_ssa = ssa if every_dst else ctx.ssa_dsts
            if json.dumps([cp.encode(ctx.symbols), cp.encode(got_ssa)]) != after:
                continue
            _save_texts(name, texts, failure)
            return result
        _unrecordable(fn.__name__, ValueError("no pass of the driver gives its result"))
        return result
    wrapper.__wrapped_pass__ = fn
    _TRANSPARENT.add(wrapper.__code__)
    return wrapper


# For each wrapper of `_tac_functions` whose call is under way, the
# (input, result) of each recorded `run` that the call made.
_NESTED_RUNS: dict[int, list] = {}


# The methods of c99_to_tac.py's `Translator` that the tests call on
# c99 nodes they build, each with the dump class of its first argument
# (or None for none) and whether it takes the list of instructions.
_C99_METHODS = {
    "translate_exp": True,
    "translate_statement": True,
    "translate_declaration": True,
    "translate_block_item": True,
    "translate_function": False,
    "make_temporary_variable_name": False,
    "translate_program": False,
}

# The `Translator`s that the test under way calls, by id: each with the
# encoding of its symbol table when the test made it, the steps, and
# the translator itself, which keeps its id from being reused.
_SESSIONS: dict[int, dict] = {}


def _wrap_c99_method(name: str, method):
    """Wraps a method of `_C99_METHODS`. Each call a test makes is a step
    of the session of its `Translator`: the dump of its argument, of the
    instructions in the list before and after the call, and of its
    result, or the text of what it raised. `_flush_sessions` saves each
    session as one record, which cpp/tests/c99_session_case.h replays on
    a C++ `Translator`."""
    takes_list = _C99_METHODS[name]

    def wrapper(self, *args, **kwargs):
        if not _called_from_test():
            return method(self, *args, **kwargs)
        session = _SESSIONS.get(id(self))
        try:
            if session is None:
                session = {
                    "translator": self,
                    "symbols": json.dumps(cp.encode(self._symbols)),
                    "steps": [],
                }
            if kwargs or self._types is not None:
                raise TypeError("keyword arguments or a type table")
            arg = args[0] if args else None
            step = {"method": name,
                    "arg": None if arg is None else cp.dump(arg)}
            if takes_list:
                step["before"] = [cp.dump(i) for i in args[1]]
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(f"Translator.{name}", e)
            if session is not None:
                session["broken"] = True
            return method(self, *args, **kwargs)
        _SESSIONS[id(self)] = session
        result, error = _call(lambda: method(self, *args, **kwargs), (), {})
        try:
            if error is not None:
                step["result"] = f"exception: {type(error).__name__}: {error}\n"
            elif result is None or isinstance(result, str):
                step["result"] = result
            else:
                step["result"] = cp.dump(result)
            if takes_list:
                step["after"] = [cp.dump(i) for i in args[1]]
            session["steps"].append(step)
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(f"Translator.{name}", e)
            session["broken"] = True
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = method
    return wrapper


def _flush_sessions() -> None:
    """Saves each session of the test that ended: a `c99_session`
    record with the symbol table at the end, or an `asm_session`
    record."""
    for session in _SESSIONS.values():
        if session.get("broken") or not session["steps"]:
            continue
        if session.get("kind") == "asm_session":
            _save("asm_session", {"symbols": session["symbols"],
                                  "steps": session["steps"]})
            continue
        try:
            after = json.dumps(cp.encode(session["translator"]._symbols))
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable("Translator", e)
            continue
        _save("c99_session", {"symbols": session["symbols"],
                              "steps": session["steps"],
                              "symbols_after": after})
    _SESSIONS.clear()


# The methods of tac_to_asm.py's `Translator` that the tests call on a
# translator they make with a symbol table.
_ASM_METHODS = ("translate_instruction", "translate_binary")

# The attributes of tac_to_asm.py's `Translator` that a session's
# translator must have at their defaults, since the C++ case makes its
# `Translator` with only a symbol table.
_ASM_DEFAULTS = {
    "_types": None, "_bare_exit": False, "_abi": {}, "_bindings": {},
    "_pinned_copies": {}, "_slot_aliases": {},
}


def _wrap_asm_method(name: str, method):
    """Wraps a method of `_ASM_METHODS`. Each call a test makes is a
    step of the session of its `Translator`, the dumps of the call's
    arguments and of the instructions it returned, or the text of what
    it raised, which cpp/tests/translator_case.h's
    `check_translator_session` replays on one C++ `Translator`."""
    def wrapper(self, *args, **kwargs):
        if not _called_from_test():
            return method(self, *args, **kwargs)
        session = _SESSIONS.get(id(self))
        try:
            if session is None:
                for attr, default in _ASM_DEFAULTS.items():
                    if getattr(self, attr) != default:
                        raise TypeError(f"the translator's {attr} isn't its default")
                if getattr(self, "_recursion_saves", None):
                    raise TypeError("the translator has recursion saves")
                symbols = self._symbols
                session = {
                    "kind": "asm_session",
                    "translator": self,
                    "symbols": None if symbols is None
                    else json.dumps(cp.encode(symbols)),
                    "steps": [],
                }
            if kwargs:
                raise TypeError("keyword arguments")
            step = {"name": name, "inputs": [cp.dump(a) for a in args]}
        except Exception as e:  # noqa: BLE001 - noted, and the call made
            _unrecordable(f"tac_to_asm.Translator.{name}", e)
            if session is not None:
                session["broken"] = True
                _SESSIONS[id(self)] = session
            return method(self, *args, **kwargs)
        _SESSIONS[id(self)] = session
        result, error = _call(lambda: method(self, *args, **kwargs), (), {})
        try:
            if error is not None:
                step["expected"] = [f"exception: {type(error).__name__}: {error}\n"]
            else:
                step["expected"] = [cp.dump(i) for i in result]
            session["steps"].append(step)
        except Exception as e:  # noqa: BLE001 - noted
            _unrecordable(f"tac_to_asm.Translator.{name}", e)
            session["broken"] = True
        if error is not None:
            raise error
        return result
    wrapper.__wrapped_pass__ = method
    return wrapper


def _install_c99_methods() -> None:
    """Puts `_wrap_c99_method`'s wrappers in place of the methods of
    c99_to_tac.py's `Translator`."""
    from c99_to_tac import Translator
    for name in _C99_METHODS:
        method = vars(Translator)[name]
        if getattr(method, "__wrapped_pass__", None) is None:
            setattr(Translator, name, _wrap_c99_method(name, method))
    from tac_to_asm import Translator as AsmTranslator
    for name in _ASM_METHODS:
        method = vars(AsmTranslator)[name]
        if getattr(method, "__wrapped_pass__", None) is None:
            setattr(AsmTranslator, name, _wrap_asm_method(name, method))


def _install_tac_runs() -> None:
    """Puts `_wrap_tac_run`'s wrapper in place of the `run` of each TAC
    pass class."""
    for cls, name_of in _tac_pass_classes().items():
        if getattr(vars(cls).get("run"), "__wrapped_pass__", None) is None:
            cls.run = _wrap_tac_run(cls, name_of)


def _targets() -> dict[int, tuple[object, object]]:
    """The functions to wrap, by id, each with its wrapper."""
    import importlib
    out: dict[int, tuple[object, object]] = {}
    for key, (fn, passes) in _tac_functions().items():
        out[key] = (fn, _wrap_tac_function(fn, passes))
    for name, target in cp.TAC_STAGE_PASSES.items():
        module, attr = cp.resolve(target)
        fn = getattr(module, attr)
        out.setdefault(id(fn), (fn, _wrap_stage(name, fn)))
    for name, module in (("tac_to_ssa", "passes.optimization.ssa_construction"),
                         ("tac_from_ssa", "passes.optimization.ssa_destruction")):
        fn = getattr(importlib.import_module(module), name[len("tac_"):])
        out.setdefault(id(fn), (fn, _wrap_tac_ssa(name, fn)))
    # The preprocessor is left out: a test calls it on a source alone,
    # without the arguments and file name that its stage call takes,
    # and the parse record that follows holds its output.
    for name in cp.FRONT_PASSES:
        if name == "preprocess":
            continue
        module, attr = cp.resolve(cp.FRONT_PASSES[name])
        fn = getattr(module, attr)
        out.setdefault(id(fn), (fn, _wrap_front(name, fn)))
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
    from passes.optimization import row_address_tables
    _ORIGINAL_SAVING[:] = [row_address_tables._saving]
    _install_tac_runs()
    _install_c99_methods()
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
        _flush_sessions()
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
    if record["kind"] == "rules":
        return _record_parts({"kind": "capture", "text": record["text"]})[1:]
    if record["kind"] == "asm_session":
        return [record["symbols"] or "",
                *(x for st in record["steps"]
                  for x in (st["name"], *st["inputs"], *st["expected"]))]
    if record["kind"] == "c99_session":
        return [record["symbols"], record["symbols_after"],
                *(x for st in record["steps"]
                  for x in (st["method"], st["arg"] or "", st["result"] or "",
                            *st.get("before", []), *st.get("after", [])))]
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
    if record["kind"] == "asm_session":
        def items(xs, indent):
            if not xs:
                return "{}"
            return "{\n" + ",\n".join(
                indent + "  " + _text_expr(x, indent + "  ") for x in xs) + "}"

        steps = ",\n".join(
            "        {\n"
            f'          "{s["name"]}",\n'
            f"          {items(s['inputs'], '          ')},\n"
            f"          {items(s['expected'], '          ')},\n"
            "        }" for s in record["steps"])
        symbols = record["symbols"]
        title = f"asm_session: {module}::{cls_test} (call {n})"
        return (f'TEST_CASE("{title}") {{\n'
                f"  c6502::testing::check_translator_session(\n"
                f"      {_text_expr(symbols, '      ') if symbols else chr(34) * 2},\n"
                "      {\n" + steps + "\n      });\n}\n")
    if record["kind"] == "c99_session":
        def literal(text, indent):
            return _text_expr(text, indent) if text else '""'

        def strings(items, indent):
            if not items:
                return "{}"
            inner = ",\n".join(indent + "  " + literal(x, indent + "  ")
                               for x in items)
            return "{\n" + inner + "}"

        steps = []
        for s in record["steps"]:
            ind = "          "
            takes_list = "before" in s
            steps.append(
                "        {\n"
                f'          "{s["method"]}",\n'
                f"          {literal(s['arg'] or '', ind)},\n"
                f"          {'true' if takes_list else 'false'},\n"
                f"          {strings(s.get('before', []), ind)},\n"
                f"          {literal(s['result'] or '', ind)},\n"
                f"          {strings(s.get('after', []), ind)},\n"
                "        }")
        title = f"c99_session: {module}::{cls_test} (call {n})"
        return (f'TEST_CASE("{title}") {{\n'
                f"  c6502::testing::check_c99_session(\n"
                f"      {literal(record['symbols'], '      ')},\n"
                "      {\n" + ",\n".join(steps) + "\n      },\n"
                f"      {literal(record['symbols_after'], '      ')});\n}}\n")
    name = record["text"].split("\n")[1][len("pass "):]
    title = f"{name}: {module}::{cls_test} (call {n})"
    if record["kind"] == "rules":
        args, given, expected, *mutated = _record_parts(record)
        rules = ", ".join(f"c6502::passes::JumpFoldRule::{r}"
                          for r in record["rules"])

        def literal(text: str) -> str:
            return _text_expr(text, "      ") if text else '""'

        return (f'TEST_CASE("{title}") {{\n'
                f"  c6502::testing::check_rules_case(\n"
                f"      {{{rules}}},\n"
                f"      {literal(args)},\n"
                f"      {literal(given)},\n"
                f"      {literal(expected)},\n"
                f"      {literal(mutated[0] if mutated else '')});\n}}\n")
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
        f"// The calls that the tests of {module} made of the functions\n"
        "// that `check-captures` runs, each with what the Python function\n"
        "// returned, changed, or raised, which the C++ port must return,\n"
        "// change, or raise too. tools/record_test_calls.py recorded them\n"
        "// while the Python tests ran, before they were removed (step 8,\n"
        "// groups G6 and G7). A call that another C++ case already checks\n"
        "// isn't repeated here.\n"
        '#include "doctest.h"\n'
        '#include "record_case.h"\n'
    )
    if any("check_translator(" in c or "check_translator_session(" in c
           for c in cases):
        header += '#include "translator_case.h"\n'
    if any("check_rules_case(" in c for c in cases):
        header += '#include "tac_rules_case.h"\n'
    if any("check_c99_session(" in c for c in cases):
        header += '#include "c99_session_case.h"\n'
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
