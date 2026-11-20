"""Records the calls of asm passes that the Python compiler makes while
it compiles a corpus, for checking the C++ port of each pass.

    uv run python tools/capture_passes.py --passes split_mem_to_mem
    uv run python tools/capture_passes.py --passes all -j 12
    uv run python tools/capture_passes.py --passes redundant_load \\
        --corpus examples,games --modes O

For each call of a chosen pass, the harness writes the pass's name, the
call's arguments after the program, the program the pass was given,
and what it returned, to `<out>/<pass>/<sha1>.txt` (by default under
`captures/`, which git ignores). The name is the SHA-1 of the pass's
name, its arguments, and its input, so identical calls are stored
once. `cpp/build/bin/c6502cpp check-captures captures/<pass>` (or
tools/check_captures.py) runs the C++ port of the pass on each one.

The passes are the asm passes of `compile.optimized_asm` from
`apply_licm` on, which include every pass of `_peephole_fixedpoint`,
and the functions after them through emission (`place_zp_globals`,
`lower_to_asm2`, `emit_program`, and the rest), under the names in
`PASSES`. The unoptimized pipeline runs
`_peephole_fixedpoint` and `expand_long_branches` too. The harness
replaces each chosen pass's function in `compile`'s namespace with a
wrapper that records the call and returns the pass's result, so the
compile goes on as it would without it. A pass of the fixed point that
`_run_on_changed` runs on part of the program is recorded with that
part, which is the call the pass gets. The `memo` keyword argument of
the program-scope passes is left out of the record, since a pass's
result is the same with or without it.

The corpus (`--corpus`, comma-separated):

- `examples`: each `examples/*.c`.
- `games`: each unit of `examples/ali_baba`, `examples/lode_runner`,
  and `examples/zork`, with `-I` its directory.
- `chapters`: every C file under `tests/chapter_*`. The ones that
  don't compile still give the calls made before the error.
- `fuzz`: the programs of seeds `--fuzz-seeds` (0-99 by default) of
  `fuzz.stmts`, `fuzz.calls`, and `fuzz.exprs`.

Each is compiled with `-S -O` and with `-S` (`--modes O,unopt`).

A capture file is text with LF line endings:

    c6502-capture 2
    pass <name>
    source <the compile that first made the call>
    args <tagged JSON, one line>
    context <JSON, one line>
    input
    <the input program's dump (ir_dump.py)>
    output
    <the result's dump>

When the result isn't an IR value (`apply_slot_coalescing` returns a
tuple), the `output` line is `output-json` and the result's tagged
JSON follows on one line. When the first argument isn't one
(`format_metadata` takes a `LinkMetadata`), the `input` line is
`input-json` and its tagged JSON follows on one line. Two kinds of call
are recorded with other arguments than the function is given:
`section_items` with what it reads of the symbol table
(`sections.section_info`) in place of the table, and `hargs_need` and
`place_zp_globals` with the helpers' HARGS footprints, which they read
inside the call, as the keyword argument `footprints`. The tagged JSON of the arguments is an object
`{"args": [...], "kwargs": {...}}`, and `encode` gives the encoding of
each Python value: JSON's own types for None, bools, ints, strings,
and lists, and an object with one tag for the rest, `{"tuple": [...]}`,
`{"set": [...]}` (sorted by encoding, for a set or a frozenset),
`{"dict": [[key, value], ...]}` (in insertion order),
`{"range": [start, stop, step]}`, `{"asdl": module, "dump": text}` for
an IR value, `{"enum": "module.Class", "name": name}`, and
`{"object": "module.Class", "fields": {...}}` for any other object,
from its dataclass fields or its `__dict__`.

The context is the state of the compile that the asm passes read from
module globals and the C++ passes receive as an `AsmContext`
(cpp/README.md): `{"layout": [ssp, fp, hargs, hargs_size, dptr],
"user_zp_globals": [[name, address], ...], "user_zp_bytes": [...]}`,
from `runtime_layout.current()` and `asm_aliasing.set_user_zp_globals`,
sorted. A pass that installs a pointer proof computes it from its
program, so the proof isn't part of the context.
"""
from __future__ import annotations

import argparse
import contextlib
import dataclasses
import enum
import hashlib
import importlib
import io
import json
import os
import sys
import tempfile
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

_ROOT = Path(__file__).resolve().parent.parent
if str(_ROOT) not in sys.path:
    sys.path.insert(0, str(_ROOT))


# Each pass the harness can record, by the name it records it under,
# with the name of its function in `compile`'s namespace. The first
# group is `_peephole_fixedpoint`'s sweep, under the sweep's names, and
# the second the other passes of `optimized_asm` from `apply_licm` on.
# tests/test_capture_passes.py checks the table against compile.py.
SWEEP_PASSES = {
    "split_mem_to_mem": "apply_split_mem_to_mem",
    "carry_halve": "apply_carry_halve",
    "carry_in_add": "apply_carry_in_add",
    "inc_peephole": "apply_inc_peephole",
    "inc_low_byte_test": "apply_inc_low_byte_test",
    "carry_inc": "apply_carry_inc_peephole",
    "dec_peephole": "apply_dec_peephole",
    "sub1_test_zero": "apply_sub1_test_zero_peephole",
    "element_in_place": "apply_element_in_place",
    "direct_index_load": "apply_direct_index_load",
    "dead_pha_pla": "apply_dead_pha_pla",
    "cpx_cpy": "apply_cpx_cpy_peephole",
    "memory_value_propagation": "apply_memory_value_propagation",
    "shift_in_a": "apply_shift_in_a",
    "redundant_load_after_rmw": "apply_redundant_load_after_rmw",
    "redundant_load": "apply_redundant_load_elimination",
    "redundant_store": "apply_redundant_store_elimination",
    "constant_store_order": "apply_constant_store_order",
    "join_store": "apply_join_store",
    "remat": "apply_remat",
    "asm_dead_store": "apply_asm_dead_store",
    "loop_constant_a": "apply_loop_constant_a",
    "dead_a_arith": "apply_dead_a_arith_elimination",
    "dead_label_drop": "apply_dead_label_drop",
    "unreachable_drop": "apply_unreachable_drop",
    "branch_invert": "apply_branch_invert",
    "branch_to_next_drop": "apply_branch_to_next_drop",
    "branch_through_jump": "apply_branch_through_jump",
    "branch_thread": "apply_branch_thread",
    "branch_value_thread": "apply_branch_value_thread",
    "dec_register_peephole": "apply_dec_register_peephole",
    "transfer_past_branch": "apply_transfer_past_branch",
    "index_inc_cmp_y": "apply_index_inc_cmp_y",
    "volatile_void_read_cmp": "apply_volatile_void_read_cmp",
    "dec_branch_indirect_y_fold": "apply_dec_branch_indirect_y_fold",
    "mem_const_prop": "apply_mem_const_prop",
    "const_arith_fold": "apply_const_arith_fold",
    "label_offset_fold": "apply_label_offset_fold",
    "carry_recompute_drop": "apply_carry_recompute_drop",
    "carry_return_tails": "apply_carry_return_tails",
    "carry_const_fold": "apply_carry_const_fold",
    "carry_branch_fold": "apply_carry_branch_fold",
    "round_trip_load_drop": "apply_round_trip_load_drop",
    "and_sign_bit_branch": "apply_and_sign_bit_branch",
    "self_store_drop": "apply_self_store_drop",
    "pointer_self_store_drop": "apply_pointer_self_store_drop",
    "redundant_compare": "apply_redundant_compare",
    "cmp_through_tmp_fold": "apply_cmp_through_tmp_fold",
    "adc_commute": "apply_adc_commute",
    "cmp_sbc_fusion": "apply_cmp_sbc_fusion",
    "dec_inc_branch_fold": "apply_dec_inc_branch_fold",
    "tail_call": "apply_tail_call",
    "tail_merge": "apply_tail_merge",
    "cross_jump": "apply_cross_jump",
    "via_a_store_fold": "apply_via_a_store_fold",
    "transfer_pm1_store": "apply_transfer_pm1_store",
}

PIPELINE_PASSES = {
    "licm": "apply_licm",
    "promote_loop_carried_to_a": "apply_promote_loop_carried_to_a",
    "loop_counter_to_x": "apply_loop_counter_to_x",
    "counter_to_index_reg": "apply_counter_to_index_reg",
    "x_save_slot_load": "apply_x_save_slot_load",
    "dual_index_promotion": "apply_dual_index_promotion",
    "carry_known": "apply_carry_known",
    "slot_coalescing": "apply_slot_coalescing",
    "ret_slot_coalescing": "apply_ret_slot_coalescing",
    "loop_invariant_a": "apply_loop_invariant_a",
    "rotate_loop_test": "apply_rotate_loop_test",
    "jump_tables": "apply_jump_tables",
    "copy_through_index": "apply_copy_through_index",
    "long_branches": "expand_long_branches",
    "short_jumps": "apply_short_jumps",
    "shared_tails": "apply_shared_tails",
    "outline_runs": "apply_outline_runs",
    # The work between the region and emission that reads the asm
    # program, and emission, which `_codegen` and the end of
    # `optimized_asm` run.
    "link_records": "link_records",
    "place_zp_globals": "place_zp_globals",
    "check_shares": "check_shares",
    "prune_unused_locals": "prune_unused_locals",
    "lower_to_asm2": "lower_to_asm2",
    "hargs_need": "hargs_need",
    "section_items": "section_items",
    "format_metadata": "format_metadata",
    "emit_program": "emit_program",
}

PASSES = {**SWEEP_PASSES, **PIPELINE_PASSES}

_GAMES = ("ali_baba", "lode_runner", "zork")
_FUZZERS = ("stmts", "calls", "exprs")


# --- Encoding the arguments ---

_IR_MODULES = frozenset({"asm_ast", "asm2_ast", "tac_ast", "c99_ast"})


def encode(v):
    """Returns the tagged JSON form of a Python value (the module
    docstring lists the forms)."""
    import ir_dump
    if v is None or type(v) in (bool, int, str):
        return v
    if type(v) is list:
        return [encode(e) for e in v]
    if type(v) is tuple:
        return {"tuple": [encode(e) for e in v]}
    if isinstance(v, (set, frozenset)):
        items = [encode(e) for e in v]
        items.sort(key=lambda e: json.dumps(e, sort_keys=True))
        return {"set": items}
    if isinstance(v, dict):
        return {"dict": [[encode(k), encode(x)] for k, x in v.items()]}
    if isinstance(v, range):
        return {"range": [v.start, v.stop, v.step]}
    if type(v).__module__ in _IR_MODULES:
        return {"asdl": type(v).__module__, "dump": ir_dump.dump(v)}
    if isinstance(v, enum.Enum):
        return {"enum": _qualname(type(v)), "name": v.name}
    if dataclasses.is_dataclass(v) and not isinstance(v, type):
        return {"object": _qualname(type(v)), "fields": {
            f.name: encode(getattr(v, f.name)) for f in dataclasses.fields(v)
        }}
    if hasattr(v, "__dict__"):
        return {"object": _qualname(type(v)), "fields": {
            k: encode(x) for k, x in vars(v).items()
        }}
    if hasattr(v, "__slots__"):
        return {"object": _qualname(type(v)), "fields": {
            k: encode(getattr(v, k)) for k in v.__slots__ if hasattr(v, k)
        }}
    raise TypeError(f"can't encode a {type(v).__name__} for a capture")


def _qualname(cls: type) -> str:
    return f"{cls.__module__}.{cls.__qualname__}"


# --- Recording ---

# The state of a worker process: where captures go, the label of the
# compile under way, and the number of calls of each pass so far.
_OUT: Path | None = None
_SOURCE = ""
_COUNTS: dict[str, int] = {}
_FAILURES: dict[str, str] = {}


def context_json() -> str:
    """The context line's JSON for the compile under way (the module
    docstring describes it)."""
    from passes import asm_aliasing
    from passes.runtime_layout import current
    layout = current()
    return json.dumps({
        "layout": [layout.ssp, layout.fp, layout.hargs, layout.hargs_size,
                   layout.dptr],
        "user_zp_globals": sorted(
            [k, v] for k, v in asm_aliasing._USER_ZP_GLOBALS.items()),
        "user_zp_bytes": sorted(asm_aliasing._USER_ZP_BYTES),
    }, separators=(",", ":"))


def _record(name: str, input_section: str, args_json: str, result) -> None:
    import ir_dump
    context = context_json()
    key = hashlib.sha1(
        f"{name}\n{args_json}\n{context}\n{input_section}".encode("utf-8")
    ).hexdigest()
    _COUNTS[name] = _COUNTS.get(name, 0) + 1
    path = _OUT / name / f"{key}.txt"
    if path.exists():
        return
    if type(result).__module__ in _IR_MODULES:
        output = "output\n" + ir_dump.dump(result)
    else:
        output = "output-json\n" + json.dumps(encode(result)) + "\n"
    text = (
        f"c6502-capture 2\npass {name}\nsource {_SOURCE}\n"
        f"args {args_json}\ncontext {context}\n{input_section}{output}"
    )
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(f"{key}.{os.getpid()}.tmp")
    with open(tmp, "wb") as f:
        f.write(text.encode("utf-8"))
    os.replace(tmp, path)


def _section_items_args(args, kwargs):
    """`section_items(prog, symbols)` reads only a little of each
    top-level name's symbol, and a symbol table can hold floats, which
    the C++ JSON reader takes no fractions of, so the harness records
    what it reads (`sections.section_info`) in place of the table."""
    from passes.sections import section_info
    return (args[0], section_info(args[0], args[1])), kwargs


def _with_footprints(args, kwargs):
    """`hargs_need` and `place_zp_globals` read the helpers' HARGS
    footprints from rtlib/helpers.py inside the call, so the harness
    records them beside the arguments, as the keyword `footprints`."""
    from passes.runtime_layout import helper_footprints
    return args, {**kwargs, "footprints": helper_footprints()}


# The calls whose record differs from the arguments the function is
# given, each with the function that gives what is recorded.
_RECORDED_ARGS = {
    "section_items": _section_items_args,
    "hargs_need": _with_footprints,
    "place_zp_globals": _with_footprints,
}


def _wrap(name: str, fn):
    def wrapper(*args, **kwargs):
        import ir_dump
        # The input and the arguments are encoded before the call, in
        # case the pass changes them in place. A first argument that
        # isn't an IR value (`format_metadata`'s `LinkMetadata`) is
        # recorded as its tagged JSON under `input-json`.
        try:
            rec_args, rec_kwargs = _RECORDED_ARGS.get(
                name, lambda a, k: (a, k),
            )(args, kwargs)
            if type(rec_args[0]).__module__ in _IR_MODULES:
                input_section = "input\n" + ir_dump.dump(rec_args[0])
            else:
                input_section = (
                    "input-json\n" + json.dumps(encode(rec_args[0])) + "\n"
                )
            args_json = json.dumps({
                "args": [encode(a) for a in rec_args[1:]],
                "kwargs": {
                    k: encode(v) for k, v in rec_kwargs.items() if k != "memo"
                },
            })
        except (ir_dump.DumpError, TypeError) as e:
            _FAILURES.setdefault(name, f"{_SOURCE}: {e}")
            return fn(*args, **kwargs)
        result = fn(*args, **kwargs)
        try:
            _record(name, input_section, args_json, result)
        except (ir_dump.DumpError, TypeError) as e:
            _FAILURES.setdefault(name, f"{_SOURCE}: {e}")
        return result
    return wrapper


def install(passes: list[str], out: Path) -> None:
    """Replaces each pass of `passes` in `compile`'s namespace with a
    wrapper that records its calls under `out`."""
    global _OUT
    import compile as compile_module
    _OUT = out
    for name in passes:
        attr = PASSES[name]
        setattr(compile_module, attr, _wrap(name, getattr(compile_module, attr)))


# --- The corpus ---

def corpus(kinds: list[str], seeds: range) -> list[tuple[str, list[str]]]:
    """Returns each program of the corpus as (label, compile.py
    arguments other than the stage and -O). A fuzz program's argument is
    `fuzz:<tool>:<seed>`, which the worker turns into a file."""
    jobs: list[tuple[str, list[str]]] = []
    if "examples" in kinds:
        for p in sorted((_ROOT / "examples").glob("*.c")):
            rel = p.relative_to(_ROOT).as_posix()
            jobs.append((rel, [str(p)]))
    if "games" in kinds:
        for game in _GAMES:
            d = _ROOT / "examples" / game
            for p in sorted(d.glob("*.c")):
                rel = p.relative_to(_ROOT).as_posix()
                jobs.append((rel, [str(p), "-I", str(d)]))
    if "chapters" in kinds:
        for p in sorted((_ROOT / "tests").glob("chapter_*/**/*.c")):
            rel = p.relative_to(_ROOT).as_posix()
            jobs.append((rel, [str(p)]))
    if "fuzz" in kinds:
        for tool in _FUZZERS:
            for seed in seeds:
                jobs.append((f"fuzz.{tool} seed {seed}", [f"fuzz:{tool}:{seed}"]))
    return jobs


def _init_worker(passes: list[str], out: str) -> None:
    # The calls recorded are the Python passes', which don't run when
    # C6502_CPP_BACKEND=1 hands the back end to the C++ port.
    os.environ.pop("C6502_CPP_BACKEND", None)
    install(passes, Path(out))


def _run_job(job) -> tuple[str, bool, dict[str, int], dict[str, str]]:
    """Compiles one program of the corpus in one mode, recording the
    chosen passes' calls, and returns the label, whether the compile
    succeeded, and the calls and failures to record counted since."""
    global _SOURCE
    import compile as compile_module
    label, args, mode = job
    _SOURCE = f"{label} -S{' -O' if mode == 'O' else ''}"
    _COUNTS.clear()
    _FAILURES.clear()
    with tempfile.TemporaryDirectory() as tmp:
        if args[0].startswith("fuzz:"):
            _, tool, seed = args[0].split(":")
            src = importlib.import_module(f"fuzz.{tool}").make_program(int(seed))[0]
            path = os.path.join(tmp, f"{tool}{seed}.c")
            with open(path, "w", encoding="utf-8", newline="\n") as f:
                f.write(src)
            args = [path] + args[1:]
        argv = ["compile.py", *args, "-S", "-o", os.path.join(tmp, "out.s")]
        if mode == "O":
            argv.append("-O")
        sink = io.StringIO()
        try:
            with contextlib.redirect_stderr(sink), contextlib.redirect_stdout(sink):
                ok = compile_module.main(argv) == 0
        except SystemExit:
            ok = False
        except Exception:  # noqa: BLE001 - a crash is a failed compile here
            ok = False
    return _SOURCE, ok, dict(_COUNTS), dict(_FAILURES)


def _parse_seeds(text: str) -> range:
    lo, _, hi = text.partition("-")
    return range(int(lo), int(hi or lo) + 1)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(
        description="Record the asm passes' calls over the corpus.",
    )
    ap.add_argument("--passes", required=True,
                    help="comma-separated pass names, or 'all'")
    ap.add_argument("--out", default=str(_ROOT / "captures"),
                    help="where to write the captures (default captures/)")
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--modes", default="O,unopt",
                    help="O for -S -O, unopt for -S")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    passes = list(PASSES) if ns.passes == "all" else ns.passes.split(",")
    unknown = [p for p in passes if p not in PASSES]
    if unknown:
        ap.error(f"unknown passes: {', '.join(unknown)}")
    modes = ns.modes.split(",")
    jobs = [
        (label, args, mode)
        for label, args in corpus(ns.corpus.split(","), _parse_seeds(ns.fuzz_seeds))
        for mode in modes
    ]
    out = Path(ns.out)
    out.mkdir(parents=True, exist_ok=True)
    start = time.time()
    totals: dict[str, int] = {}
    failures: dict[str, str] = {}
    failed_compiles = 0
    with ProcessPoolExecutor(
        max_workers=ns.j, initializer=_init_worker,
        initargs=(passes, str(out)),
    ) as pool:
        for label, ok, counts, fails in pool.map(_run_job, jobs, chunksize=4):
            failed_compiles += not ok
            # The chapters hold programs that must be rejected, and a
            # few the backend can't take, so only the others' failures
            # are worth a line.
            if not ok and not label.startswith("tests/chapter_"):
                print(f"failed to compile: {label}")
            for name, n in counts.items():
                totals[name] = totals.get(name, 0) + n
            for name, why in fails.items():
                failures.setdefault(name, why)
    print(f"{len(jobs)} compiles ({failed_compiles} failed) in "
          f"{time.time() - start:.0f} s")
    for name in passes:
        stored = len(list((out / name).glob("*.txt"))) if (out / name).exists() else 0
        print(f"{name}: {totals.get(name, 0)} calls, {stored} distinct stored")
    for name, why in failures.items():
        print(f"{name}: a call couldn't be recorded: {why}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
