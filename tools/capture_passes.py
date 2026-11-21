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
`{"range": [start, stop, step]}`, `{"float": bits}` with the bits of
the IEEE 754 double, `{"asdl": module, "dump": text}` for an IR value,
`{"enum": "module.Class", "name": name}`, and
`{"object": "module.Class", "fields": {...}}` for any other object,
from its dataclass fields or its `__dict__`. Three classes of register
allocation are recorded with other fields (`_CAPTURE_FORMS`): a
`Liveness` with the shape of its CFG in place of the blocks'
instructions and without its cache, and an `InterferenceGraph` and a
`CoalesceResult` with their dicts sorted, since the order of their
entries follows Python's set iteration.

The calls of `STAGE_PASSES`, the stages of code generation before
`apply_licm` (`--passes stage` records them all), are recorded the same
way, with `input-json` for a first argument that isn't an IR value
(`build_local_slot_symbols` takes the pools first), and with one more
line. A call that changes an argument in place (`choose_carry_returns`
sets attributes in the symbol table, and `tac_to_asm` adds locals to
it) has a line

    mutated {"args": {"<index>": ...}, "kwargs": {...}}

after the context line, with the tagged JSON of each changed argument
after the call, by its position in `args` or its keyword. The context is
taken before the call, since `plan_zero_page` runs before the zero-page
plan is in effect.

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
import copy
import dataclasses
import enum
import hashlib
import importlib
import io
import json
import os
import struct
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

# The calls of step 4 of the port (docs/cpp_port.md), the stages of
# code generation before `apply_licm`, by the name the harness records
# each under, with the function to wrap: a name in `compile`'s
# namespace, or `module:name` for one that compile.py reaches through
# its module (`asm_opt.optimize_program`) or imports inside a function.
# Their inputs are TAC or asm programs with the compile's tables, and
# not every one's first argument is a program (the module docstring
# describes the records). tests/test_capture_passes.py checks that each
# one resolves.
STAGE_PASSES = {
    "plan_zero_page": "plan_zero_page",
    "select_abi": "select_abi",
    "choose_global_carry_returns": "choose_global_carry_returns",
    "choose_carry_returns": "choose_carry_returns",
    "recursive_pool_functions": "recursive_pool_functions",
    "pool_eligible": "pool_eligible",
    # Group A's calls inside its own modules. `cycles` and
    # `hw_stack_cycles` are recorded where the pool eligibility calls
    # them, `_live_out` where `find_shares` calls it, and `build_cfg`
    # where `_live_out` calls it, from `find_shares` and `plan_saves`.
    "pinned_slot_symbols": "pinned_slot_symbols",
    "cycles": "passes.zp_local_allocation:cycles",
    "hw_stack_cycles": "passes.zp_local_allocation:hw_stack_cycles",
    "check_marked_calls": "passes.recursion_saves:check_marked_calls",
    "plan_saves": "passes.recursion_saves:plan_saves",
    "live_out": "passes.slot_sharing:_live_out",
    "build_cfg": "passes.recursion_saves:build_cfg",
    "find_shares": "find_shares",
    "allocate_zp_slots": "allocate_zp_slots",
    "pinned_param_copies": "pinned_param_copies",
    "slot_aliases": "slot_aliases",
    "tac_to_asm": "translate_to_asm",
    "asm_optimizer": "passes.optimization_asm.optimizer:optimize_program",
    "compute_local_bytes": "compute_local_bytes",
    "compute_address_taken_bytes": "compute_address_taken_bytes",
    "allocate_function_locals": "allocate_function_locals",
    "address_taken_assignments": "compute_address_taken_assignments",
    "address_taken_slot_symbols": "_addr_taken_slot_symbols",
    "replace_pseudoregisters_bare_exit": "replace_pseudoregs_bare_exit",
    "local_slot_names": "passes.zp_slot_naming:compute_local_slot_names",
    "local_slot_symbols": "build_local_slot_symbols",
    "lower_data_load_address":
        "passes.lower_data_load_address:lower_data_load_address",
    "synthesize_prologue": "synthesize_prologue",
    "fixed_address_layouts": "fixed_address_layouts",
    "replace_pseudoregisters": "replace_pseudoregs",
    # The asm SSA round trip of `_optimize_function`, under the names
    # optimizer.py imports, and the ordering of a parallel copy, which
    # `from_ssa` and `tac_to_asm` both call through its module.
    "to_ssa": "passes.optimization_asm.optimizer:to_ssa",
    "copy_propagate": "passes.optimization_asm.optimizer:copy_propagate",
    "backward_copy_propagate":
        "passes.optimization_asm.optimizer:backward_copy_propagate",
    "byte_dce": "passes.optimization_asm.optimizer:byte_dce",
    "absorb_zero_load": "passes.optimization_asm.optimizer:absorb_zero_load",
    "from_ssa": "passes.optimization_asm.optimizer:from_ssa",
    "order_parallel_copies":
        "passes.optimization_asm.ssa_destruction:_order_parallel_copies",
    # The register allocation of `_optimize_function`, each call made on
    # one function in asm SSA form.
    "compute_liveness": "passes.optimization_asm.optimizer:compute_liveness",
    "build_interference":
        "passes.optimization_asm.optimizer:build_interference",
    "coalesce_moves": "passes.optimization_asm.optimizer:coalesce_moves",
    "hwreg_eligibility":
        "passes.optimization_asm.optimizer:scan_hwreg_eligibility",
    "color_graph": "passes.optimization_asm.optimizer:color_graph",
}

# The stage calls that change their first argument in place where the
# caller never reads it again, so the harness gives each a copy and
# records the argument as the caller gave it. `from_ssa` rewrites the
# predecessor labels of its input's `Phi`s when it splits an edge.
_COPIES_INPUT = frozenset({"from_ssa"})


def all_passes() -> dict[str, str]:
    """Every call the harness can record, `PASSES` and `STAGE_PASSES`
    together."""
    return {**PASSES, **STAGE_PASSES}


def resolve(target: str):
    """Returns the module object and the attribute name that a table
    entry names: `compile` and the name, or the module before the colon
    and the name after it."""
    import compile as compile_module
    if ":" in target:
        module, _, attr = target.partition(":")
        return importlib.import_module(module), attr
    return compile_module, target

_GAMES = ("ali_baba", "lode_runner", "zork")
_FUZZERS = ("stmts", "calls", "exprs")


# --- Encoding the arguments ---

_IR_MODULES = frozenset({"asm_ast", "asm2_ast", "tac_ast", "c99_ast"})


def _liveness_fields(v) -> dict:
    """The fields of a `Liveness` as the harness records them. The CFG
    is `build_cfg` of the function the call is given, so each block is
    recorded as its id, its number of instructions, and its edges, and
    the per-instruction sets, a cache filled as they are asked for, are
    left out."""
    return {
        "blocks": [
            (b.id, len(b.instructions), b.predecessors, b.successors)
            for b in v.cfg.blocks.values()
        ],
        "block_order": v.cfg.block_order,
        "live_in": v.live_in,
        "live_out": v.live_out,
    }


def _graph_fields(v) -> dict:
    """The fields of an `InterferenceGraph`, with its nodes and its
    adjacency sorted by name. The order the builder adds them in follows
    the iteration of Python sets, which changes from one process to the
    next, and nothing reads it."""
    return {
        "nodes": dict(sorted(v.nodes.items())),
        "adj": dict(sorted(v.adj.items())),
    }


def _coalesce_fields(v) -> dict:
    """The fields of a `CoalesceResult`, with `byte_offset` sorted by
    name, since the order of its entries follows the order of the
    graph's nodes and nothing reads it."""
    return {
        "representative": v.representative,
        "byte_offset": dict(sorted(v.byte_offset.items())),
    }


# The classes whose recorded fields differ from their dataclass fields,
# each with the function that gives them.
_CAPTURE_FORMS = {
    "passes.optimization_asm.liveness.Liveness": _liveness_fields,
    "passes.optimization.interference.InterferenceGraph": _graph_fields,
    "passes.optimization_asm.coalescing.CoalesceResult": _coalesce_fields,
}


def encode(v):
    """Returns the tagged JSON form of a Python value (the module
    docstring lists the forms)."""
    import ir_dump
    if v is None or type(v) in (bool, int, str):
        return v
    if type(v) is float:
        return {"float": struct.unpack("<Q", struct.pack("<d", v))[0]}
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
    form = _CAPTURE_FORMS.get(_qualname(type(v)))
    if form is not None:
        return {"object": _qualname(type(v)), "fields": {
            k: encode(x) for k, x in form(v).items()
        }}
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


def _record(name: str, input_section: str, args_json: str, result,
            mutated: str | None = None, context: str | None = None) -> None:
    import ir_dump
    if context is None:
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
    extra = "" if mutated is None else f"mutated {mutated}\n"
    text = (
        f"c6502-capture 2\npass {name}\nsource {_SOURCE}\n"
        f"args {args_json}\ncontext {context}\n{extra}{input_section}{output}"
    )
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(f"{key}.{os.getpid()}.tmp")
    with open(tmp, "wb") as f:
        f.write(text.encode("utf-8"))
    try:
        os.replace(tmp, path)
    except PermissionError:
        # On Windows the rename fails while another worker renames its
        # copy of the same record into place. That record has the same
        # key, so it holds the same text.
        if not path.exists():
            raise
        tmp.unlink()


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


def _encode_call(args, kwargs) -> tuple[list[str], dict[str, str]]:
    """The JSON text of each argument's tagged encoding."""
    return (
        [json.dumps(encode(a)) for a in args],
        {k: json.dumps(encode(v)) for k, v in kwargs.items() if k != "memo"},
    )


def _call_json(args: list[str], kwargs: dict[str, str]) -> str:
    """The `args` line of a record from the texts `_encode_call` gives,
    which is `json.dumps` of the call's tagged arguments."""
    return (
        '{"args": [' + ", ".join(args) + '], "kwargs": {'
        + ", ".join(f"{json.dumps(k)}: {v}" for k, v in kwargs.items())
        + "}}"
    )


def _wrap_stage(name: str, fn):
    """The wrapper of a call of `STAGE_PASSES`. The first argument is the
    record's input, an IR value's dump or the `input-json` line of any
    other value, as for the other passes. Each argument is encoded again
    after the call, and those the call changed in place
    (`choose_carry_returns` sets attributes in `symbols`, and
    `tac_to_asm` adds the `.reg` locals to it) are recorded on the
    `mutated` line with their new encoding, by position in `args` or by
    keyword."""
    def wrapper(*args, **kwargs):
        import ir_dump

        def input_section():
            if type(args[0]).__module__ in _IR_MODULES:
                return "input\n" + ir_dump.dump(args[0])
            return "input-json\n" + json.dumps(encode(args[0])) + "\n"

        rest = args[1:]
        context = context_json()
        try:
            input_text = input_section()
            before = _encode_call(rest, kwargs)
        except (ir_dump.DumpError, TypeError) as e:
            _FAILURES.setdefault(name, f"{_SOURCE}: {e}")
            return fn(*args, **kwargs)
        if name in _COPIES_INPUT:
            result = fn(copy.deepcopy(args[0]), *rest, **kwargs)
        else:
            result = fn(*args, **kwargs)
        try:
            after = _encode_call(rest, kwargs)
            changed_args = {
                str(i): a for i, (a, b) in enumerate(zip(after[0], before[0]))
                if a != b
            }
            changed_kwargs = {
                k: v for k, v in after[1].items() if v != before[1][k]
            }
            mutated = None
            if changed_args or changed_kwargs:
                mutated = (
                    '{"args": {' + ", ".join(
                        f'"{i}": {v}' for i, v in changed_args.items())
                    + '}, "kwargs": {' + ", ".join(
                        f"{json.dumps(k)}: {v}"
                        for k, v in changed_kwargs.items())
                    + "}}"
                )
            if input_section() != input_text:
                raise TypeError("the call changed its first argument in place")
            _record(name, input_text, _call_json(*before), result,
                    mutated=mutated, context=context)
        except (ir_dump.DumpError, TypeError) as e:
            _FAILURES.setdefault(name, f"{_SOURCE}: {e}")
        return result
    return wrapper


def install(passes: list[str], out: Path) -> None:
    """Replaces each pass of `passes` in `compile`'s namespace (or, for
    an entry of `STAGE_PASSES` that names a module, in that module)
    with a wrapper that records its calls under `out`."""
    global _OUT
    import compile as compile_module
    _OUT = out
    for name in passes:
        if name in STAGE_PASSES:
            module, attr = resolve(STAGE_PASSES[name])
            setattr(module, attr, _wrap_stage(name, getattr(module, attr)))
            continue
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
                    help="comma-separated pass names, 'all' for those of "
                         "PASSES, or 'stage' for those of STAGE_PASSES")
    ap.add_argument("--out", default=str(_ROOT / "captures"),
                    help="where to write the captures (default captures/)")
    ap.add_argument("--corpus", default="examples,games,chapters,fuzz")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--modes", default="O,unopt",
                    help="O for -S -O, unopt for -S")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ns = ap.parse_args(argv)
    if ns.passes == "all":
        passes = list(PASSES)
    elif ns.passes == "stage":
        passes = list(STAGE_PASSES)
    else:
        passes = ns.passes.split(",")
    unknown = [p for p in passes if p not in all_passes()]
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
