"""Records what the helpers the asm passes share return on the inputs
the passes see, for checking the C++ port of the helpers.

    uv run python tools/capture_helpers.py -j 16
    cpp/build/bin/c6502cpp check-helpers captures/helpers

The harness compiles the corpus of tools/capture_passes.py (by default
the examples, the games, and the fuzz seeds) and wraps every pass of
its `PASSES` table. Each time a pass is called, each function of the
program it is given is evaluated, unless the same function was already
evaluated in the same context. The helpers are evaluated at every
instruction index, on every operand the function contains, or on every
pair of those operands, as `function_values` lists, and the results
are written with the function and its context to
`<out>/fn/<sha1>.txt`. `unreachable_by_pointer`, which reads the whole
program, is evaluated on each program and written to
`<out>/prog/<sha1>.txt` with a reduced program that gives the same
answer (`reduced_program`). The call-graph edges of `call_edges.py`
are evaluated on each TAC function `select_abi` is given and written
to `<out>/tac/<sha1>.txt`.

The context of a function is the state of the compile that the
helpers read: the runtime's layout, the user zero-page globals, the
compiler's slot symbols (from the `zp_slot_symbols` argument of the
peephole fixed point under way, or of the last one), the zero-page
names and slot addresses that `compile.optimized_asm` gives
`shared_tails` and `outline_runs`, and the names `unreachable_by_pointer`
gives for the program, which the passes install as the pointer proof.

A function record is text with LF line endings:

    c6502-helpers 1
    source <the compile that first made it>
    context <JSON, one line>
    function
    <the function's dump>
    values
    <helper> <value>
    ...

and a program or TAC record is the same with `program` or `tac` in
place of `function` and no context line. Each value is one line, and
`function_values` describes the encoding of each.

The helpers that the code generation passes share (step 4 of the port,
docs/cpp_port.md) have two more records, which `--stage-only` makes
without the others. A `tables` record, made at each call of
`find_shares`, holds one line of tagged JSON (tools/capture_passes.py's
`encode`) with the TAC program, the symbol table, the type table, the
shares `find_shares` returned, and the runtime's layout, and
`stage_values` gives its values. A `pseudos` record, made at each call
of `replace_pseudoregisters` and `replace_program_bare_exit`, holds the
names of the pseudos of the program and of the colorings, and
`pseudo_values` gives `parse_pseudo_name` of each.

The helpers that the TAC optimizer's passes share (step 5) have a
record of their own, which `--tac-only` makes without the others. A
`tacopt` record, made at each call of a pass of the optimizer's driver
(`PhaseDriver`), holds one line of tagged JSON with the function the
pass is given, the symbols of its operands, and the SSA names, and
`tac_opt_values` gives its values.

The c99 type queries that the front end's passes share (step 6) have a
record of their own too, which `--front-only` makes without the others.
A `c99types` record, made at each call of the type checker, holds one
line of JSON with the dumps of the types of the type-checked program
and its symbol table, of its top-level declarations, and the names of
its symbol table, and `front_values` gives its values.
"""
from __future__ import annotations

import argparse
import contextlib
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
_TOOLS = str(Path(__file__).resolve().parent)
if _TOOLS not in sys.path:
    sys.path.insert(0, _TOOLS)

import capture_passes  # noqa: E402


def _bits(xs) -> str:
    return "".join("1" if x else "0" for x in xs) or "-"


def _ints(xs) -> str:
    return ",".join(str(x) for x in xs) or "-"


def _names(xs) -> str:
    return " ".join(xs) or "-"


def _dump1(v) -> str:
    import ir_dump
    return ir_dump.dump(v).rstrip("\n")


def _try(f):
    """`f()`, or "E" when it raises."""
    try:
        return f()
    except Exception:  # noqa: BLE001 - a raise is a value to compare
        return "E"


# --- The context ---

def make_context(prog, slots, zp_names, slot_addrs) -> dict:
    """The context of the functions of `prog` (the module docstring
    lists what it holds)."""
    from passes import asm_aliasing
    from passes.runtime_layout import current
    layout = current()
    return {
        "layout": [layout.ssp, layout.fp, layout.hargs, layout.hargs_size,
                   layout.dptr],
        "user_zp_globals": sorted(
            [k, v] for k, v in asm_aliasing._USER_ZP_GLOBALS.items()),
        "user_zp_bytes": sorted(asm_aliasing._USER_ZP_BYTES),
        "zp_slot_symbols": (
            None if slots is None else sorted([k, v] for k, v in slots.items())
        ),
        "zp_names": None if zp_names is None else sorted(zp_names),
        "slot_addrs": sorted(slot_addrs),
        "pointer_proof": sorted(asm_aliasing.unreachable_by_pointer(prog)),
    }


# --- The values ---

def function_operands(instrs) -> list:
    """The distinct operands of `instrs`, in the order they are first
    met: those of each instruction (`_instruction_operands`), then
    those each instruction reads (`asm_dead_store._read_operands`),
    then the pointer bytes of each of those (`_ptr_source_reads`)."""
    from passes.asm_aliasing import _instruction_operands
    from passes.asm_dead_store import _ptr_source_reads, _read_operands
    ops: list = []
    index: dict[str, int] = {}

    def add(op):
        d = _dump1(op)
        if d not in index:
            index[d] = len(ops)
            ops.append(op)

    for ins in instrs:
        for op in _instruction_operands(ins):
            add(op)
    for ins in instrs:
        for op in _read_operands(ins):
            add(op)
    for op in list(ops):
        for p in _ptr_source_reads(op):
            add(p)
    return ops


def _index_of(ops):
    index = {_dump1(op): i for i, op in enumerate(ops)}
    return lambda op: index[_dump1(op)]


def _cfg_text(build, fn) -> str:
    """The blocks, edges, and dominators of `build(fn)`, or "E"."""
    from passes.optimization_asm import cfg as cfg_mod
    try:
        cfg = build(fn)
    except KeyError:
        return "E"
    begin = {cfg_mod.ENTRY_ID: 0, cfg_mod.EXIT_ID: 0}
    pos = 0
    for bid in cfg.block_order:
        begin[bid] = pos
        pos += len(cfg.blocks[bid].instructions)
    blocks = ";".join(
        f"{bid}:{begin[bid]}:{begin[bid] + len(b.instructions)}:"
        f"{_ints(b.predecessors)}:{_ints(b.successors)}"
        for bid, b in sorted(cfg.blocks.items())
    )
    rpo = cfg_mod.reverse_postorder(cfg)
    idom = cfg_mod.immediate_dominators(cfg)
    children = cfg_mod.dominator_tree_children(idom)
    df = cfg_mod.dominance_frontiers(cfg)
    return (
        f"{_ints(cfg.block_order)}|{blocks}|{_ints(rpo)}|"
        + ",".join(f"{b}>{p}" for b, p in idom.items()) + "|"
        + ";".join(f"{p}:{_ints(c)}" for p, c in children.items()) + "|"
        + ";".join(f"{b}:{_ints(sorted(s))}" for b, s in df.items())
    )


def function_values(fn, ctx: dict) -> list[tuple[str, str]]:
    """Each helper's values on `fn` in context `ctx`, as (name, text).
    A value over the instruction indices 0 to n (n the number of
    instructions, so the index past the end is included for the walks)
    or over the instructions is a string of 0s and 1s. A value over the
    function's operands (`function_operands`) is the same, by operand,
    and one over pairs of operands is n_ops * n_ops bits, row by row.
    Lists are written with commas, and lists of lists with spaces, with
    "-" for an empty one."""
    import asm_ast
    from passes import asm_aliasing, asm_liveness, decimal_arith
    from passes import asm_dead_store as dse
    from passes import dead_a_arith, inc_peephole
    from passes.asm_aliasing import _instruction_operands, pointer_proof
    from passes.optimization_asm import cfg as cfg_mod
    from passes.redundant_load import _build_cfg_tolerant
    from sim.assembler import instruction_size

    instrs = fn.instructions
    n = len(instrs)
    out: list[tuple[str, str]] = []
    slots = (None if ctx["zp_slot_symbols"] is None
             else {k: v for k, v in ctx["zp_slot_symbols"]})
    zp_names = None if ctx["zp_names"] is None else frozenset(ctx["zp_names"])
    slot_addrs = frozenset(ctx["slot_addrs"])
    proof = frozenset(ctx["pointer_proof"])

    # asm_liveness: the walks at every index, and the predicates of
    # each instruction.
    for name in ("a_dead_at", "x_dead_at", "y_dead_at", "flags_dead_at",
                 "all_flags_dead_at", "c_dead_at", "v_dead_at"):
        f = getattr(asm_liveness, name)
        out.append((name, _bits(f(instrs, i) for i in range(n + 1))))
    for name in ("reads_a", "kills_a", "reads_x", "kills_x", "reads_y",
                 "kills_y", "sets_up_y", "sets_flags", "returns_carry"):
        f = getattr(asm_liveness, name)
        out.append((name, _bits(f(ins) for ins in instrs)))
    for reg in ("X", "Y"):
        out.append((f"call_preserves_{reg.lower()}", _bits(
            asm_liveness.call_preserves(ins, reg) for ins in instrs)))
    out.append(("label_map", _names(
        f"{k}={v}" for k, v in sorted(asm_liveness.label_map(instrs).items())
    )))

    # The operands.
    ops = function_operands(instrs)
    idx = _index_of(ops)
    out.append(("operands", str(len(ops))))
    out.append(("instruction_operands", " ".join(
        _ints(idx(op) for op in _instruction_operands(ins)) for ins in instrs
    ) or "-"))
    out.append(("read_operands", " ".join(
        _ints(idx(op) for op in dse._read_operands(ins)) for ins in instrs
    ) or "-"))
    out.append(("write_operand", " ".join(
        "-" if (w := dse._write_operand(ins)) is None else str(idx(w))
        for ins in instrs
    ) or "-"))
    out.append(("ptr_source_reads", " ".join(
        _ints(idx(p) for p in dse._ptr_source_reads(op)) for op in ops
    ) or "-"))

    # asm_aliasing.
    def name_of(op):
        return getattr(op, "name", None)
    out.append(("is_zp_symbol", "".join(
        "-" if name_of(op) is None
        else "1" if asm_aliasing._is_zp_symbol(op.name) else "0"
        for op in ops) or "-"))
    out.append(("is_user_zp_global", "".join(
        "-" if name_of(op) is None
        else "1" if asm_aliasing.is_user_zp_global(op.name) else "0"
        for op in ops) or "-"))
    out.append(("is_user_zp_byte", "".join(
        ("1" if asm_aliasing.is_user_zp_byte(op.address + op.offset) else "0")
        if isinstance(op, asm_ast.ZP) else "-"
        for op in ops) or "-"))
    out.append(("may_alias", _bits(
        asm_aliasing.may_alias(a, b) for a in ops for b in ops)))
    out.append(("may_alias_slots", _bits(
        asm_aliasing.may_alias(a, b, zp_slot_symbols=slots)
        for a in ops for b in ops)))
    with pointer_proof(proof):
        out.append(("may_alias_proof", _bits(
            asm_aliasing.may_alias(a, b, zp_slot_symbols=slots)
            for a in ops for b in ops)))
    pba = asm_aliasing.pointer_byte_addresses(instrs)
    out.append(("pointer_byte_addresses", _ints(sorted(pba))))
    out.append(("is_pointer_byte", _bits(
        asm_aliasing.is_pointer_byte(op, pba, slots) for op in ops)))
    out.append(("through_slot_pointer", _bits(
        asm_aliasing.through_slot_pointer(ins, slot_addrs) for ins in instrs)))
    called, taken, seen = asm_aliasing._function_names(instrs)
    out.append(("function_names_called", _names(sorted(called))))
    out.append(("function_names_taken", _names(sorted(taken))))
    out.append(("function_names_seen", _names(sorted(seen))))
    out.append(("runtime_zp_addrs", _names(
        f"{k}={v}" for k, v in sorted(asm_aliasing.runtime_zp_addrs().items()))))

    # decimal_arith, on each DecimalArith.
    out.append(("decimal_arith", " ".join(
        f"{i}:{_ints(idx(o) for o in decimal_arith.reads(ins))}:"
        f"{_ints(idx(o) for o in decimal_arith.writes(ins))}:"
        f"{_ints(idx(o) for o in decimal_arith.operands(ins))}:"
        f"{int(decimal_arith.writes_y(ins))}"
        for i, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.DecimalArith)
    ) or "-"))

    # asm_dead_store.
    labels = asm_liveness.label_map(instrs)
    taken_names = dse._collect_address_taken_names(instrs)
    prefix = f"__local_{fn.name}__"
    out.append(("collect_address_taken_names", _names(sorted(taken_names))))
    out.append(("successors", " ".join(
        _ints(dse._successors(instrs, i, labels)) for i in range(n)) or "-"))
    out.append(("is_opaque", _bits(
        isinstance(ins, dse._OPAQUE_TYPES) for ins in instrs)))
    targets = [op for op in ops if isinstance(op, (asm_ast.Data, asm_ast.ZP))]
    out.append(("reads", _bits(
        dse._reads(ins, t, slots) for ins in instrs for t in targets)))
    out.append(("reads_private", _bits(
        dse._reads(ins, t, slots, private=True)
        for ins in instrs for t in targets)))
    with pointer_proof(proof):
        out.append(("reads_proof", _bits(
            dse._reads(ins, t, slots) for ins in instrs for t in targets)))
    out.append(("writes_same", _bits(
        dse._writes_same(ins, t) for ins in instrs for t in targets)))
    out.append(("call_cannot_touch", _bits(
        dse._call_cannot_touch(t, prefix, taken_names) for t in targets)))
    out.append(("is_dead_at_exit", _bits(
        dse._is_dead_at_exit(t, slots) for t in targets)))
    out.append(("is_dead_at_tail_call", _bits(
        dse._is_dead_at_tail_call(t, taken_names) for t in targets)))
    rows = []
    for i in range(n):
        near = mem_dead_targets(instrs, i)
        rows.append(_bits(
            dse.mem_dead_after(instrs, i, t, fn_name=fn.name,
                               zp_slot_symbols=slots)
            for t in near))
    out.append(("mem_dead_after", " ".join(rows) or "-"))
    with pointer_proof(proof):
        rows = []
        for i in range(n):
            rows.append(_bits(
                dse.mem_dead_after(instrs, i, t, fn_name=fn.name,
                                   zp_slot_symbols=slots)
                for t in mem_dead_targets(instrs, i)))
    out.append(("mem_dead_after_proof", " ".join(rows) or "-"))

    # inc_peephole and dead_a_arith.
    out.append(("is_x_indexed", _bits(
        inc_peephole.is_x_indexed(op) for op in ops)))
    out.append(("is_inc_eligible_operand", _bits(
        inc_peephole._is_inc_eligible_operand(op) for op in ops)))
    out.append(("operands_equal", _bits(
        inc_peephole._operands_equal(a, b) for a in ops for b in ops)))
    out.append(("flags_reflect_src_reg", _bits(
        dead_a_arith._flags_reflect_src_reg(instrs, i) for i in range(n))))
    for reg in (asm_ast.A, asm_ast.X, asm_ast.Y):
        out.append((f"writes_reg_setting_n_z_{reg.__name__.lower()}", _bits(
            dead_a_arith._writes_reg_setting_n_z(ins, reg) for ins in instrs)))

    # cfg and redundant_load's tolerant builder.
    out.append(("build_cfg", _cfg_text(cfg_mod.build_cfg, fn)))
    out.append(("build_cfg_tolerant", _cfg_text(_build_cfg_tolerant, fn)))

    # sim.assembler.
    out.append(("instruction_size", " ".join(
        str(_try(lambda ins=ins: instruction_size(ins))) for ins in instrs
    ) or "-"))
    if zp_names is not None:
        out.append(("instruction_size_zp", " ".join(
            str(_try(lambda ins=ins: instruction_size(ins, zp_names=zp_names)))
            for ins in instrs
        ) or "-"))
    return out


def mem_dead_targets(instrs, i) -> list:
    """The bytes `mem_dead_after` is asked about at index `i`: the
    `Data` and `ZP` operands of `instrs[i]` and `instrs[i + 1]`, each
    once, in order."""
    import asm_ast
    from passes.asm_aliasing import _instruction_operands
    out: list = []
    seen: set[str] = set()
    for ins in instrs[i:i + 2]:
        for op in _instruction_operands(ins):
            if isinstance(op, (asm_ast.Data, asm_ast.ZP)):
                d = _dump1(op)
                if d not in seen:
                    seen.add(d)
                    out.append(op)
    return out


def reduced_program(prog):
    """A program that gives the same `unreachable_by_pointer` as `prog`,
    with only what the function reads: each static's name, linkage, and
    initializers that take an address, and for each function a `Call`
    of each name it calls, a load of `#<name` for each name whose
    address it takes, and a load of each `Data` name it reads."""
    import asm_ast
    from passes import asm_aliasing
    a = asm_ast.Reg(reg=asm_ast.A())
    top = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.StaticVariable):
            top.append(asm_ast.StaticVariable(
                name=tl.name, is_global=tl.is_global,
                init=[i for i in tl.init if isinstance(
                    i, (asm_ast.AddressInit, asm_ast.AddressByteInit))],
            ))
        elif isinstance(tl, asm_ast.Function):
            called, taken, seen = asm_aliasing._function_names(tl.instructions)
            instrs = [asm_ast.Call(name=c, reg_args=[], preserves=[])
                      for c in sorted(called)]
            instrs += [asm_ast.Mov(src=asm_ast.ImmLabelLow(name=t, offset=0),
                                   dst=a, is_volatile=False)
                       for t in sorted(taken)]
            instrs += [asm_ast.Mov(src=asm_ast.Data(name=s, offset=0),
                                   dst=a, is_volatile=False)
                       for s in sorted(seen)]
            top.append(asm_ast.Function(
                name=tl.name, is_global=tl.is_global, params=[],
                instructions=instrs,
            ))
    return asm_ast.Program(top_level=top)


def tac_values(fn) -> list[tuple[str, str]]:
    """`call_edges` on a TAC function: `abandons_caller` of each
    instruction, and the indices of the calls `graph_calls` gives."""
    from passes.call_edges import abandons_caller, graph_calls
    calls = graph_calls(fn)
    ids = {id(c) for c in calls}
    return [
        ("abandons_caller", _bits(abandons_caller(i) for i in fn.instructions)),
        ("graph_calls", _ints(
            k for k, i in enumerate(fn.instructions) if id(i) in ids)),
    ]


def _val_text(v) -> str:
    """A TAC value in the stage records: a `Var`'s name, or `#` and a
    constant's dump with `_` for each space."""
    import tac_ast
    if isinstance(v, tac_ast.Var):
        return v.name
    return "#" + _dump1(v).replace(" ", "_")


def _fields(xs) -> str:
    """A list of fields in the stage records, with "-" for an empty
    field."""
    return " ".join(x or "-" for x in xs) or "-"


def stage_values(tac, symbols, types, shares, layout) -> list[tuple[str, str]]:
    """The values of the helpers that the code generation passes share
    (step 4 of the port), on the TAC program, the tables, and the shares
    `find_shares` is given and returns. Each is a list of fields, one per
    symbol, member, instruction, or function, separated by spaces, with
    the items of a field separated by commas and "-" for an empty one."""
    import tac_ast
    from passes.optimization.var_visit import (
        count_uses, defs_in, uses_in, vals_in,
    )
    from passes.replace_pseudoregisters import size_of_name, sizeof
    from passes.runtime_layout import RuntimeLayout
    from passes.slot_sharing import coexist, reach_sets, share_conflicts
    from passes.type_checking import FunAttr
    from passes.zp_slot_naming import param_slot_symbols, source_spelling
    names = [n for n, _ in symbols.items()]
    fns = [tl for tl in tac.top_level if isinstance(tl, tac_ast.Function)]
    instrs = [i for fn in fns for i in fn.instructions]

    def slots_of(fn):
        sym = symbols.get(fn.name)
        if sym is None or not isinstance(sym.attrs, FunAttr):
            return "-"
        return _try(lambda: ",".join(param_slot_symbols(
            fn.name, list(fn.params),
            [sizeof(p, types) for p in sym.type.params],
        )))

    reach = reach_sets(tac)
    keys = list(reach)
    return _cfg_values(fns, instrs) + [
        ("size_of_name", _fields(
            str(size_of_name(n, symbols, types)) for n in names)),
        ("size_of_name_untyped", _fields(
            str(size_of_name(n, symbols, None)) for n in names)),
        ("sizeof_members", _fields(
            ",".join(str(sizeof(m.type, types)) for m in layout_.members)
            for _, layout_ in types.items())),
        ("source_spelling", _fields(source_spelling(n) for n in names)),
        ("param_slot_symbols", _fields(slots_of(fn) for fn in fns)),
        ("vals_in", _fields(
            ",".join(_val_text(v) for v in vals_in(i)) for i in instrs)),
        ("uses_in", _fields(",".join(v.name for v in uses_in(i)) for i in instrs)),
        ("defs_in", _fields(",".join(v.name for v in defs_in(i)) for i in instrs)),
        ("count_uses", _fields(
            ",".join(f"{n}:{c}" for n, c in count_uses(fn.instructions).items())
            for fn in fns)),
        ("reach_sets", _fields(
            f"{f}:" + ",".join(sorted(reach[f])) for f in keys)),
        ("coexist", _bits(coexist(a, b, reach) for a in keys for b in keys)),
        ("share_conflicts", _fields(
            ",".join(str(_index_by_identity(shares, s))
                     for s in share_conflicts(f, shares, reach))
            for f in keys)),
        ("slot_share_sets", _fields(
            f"{s.width}:" + ",".join(sorted(s.functions())) + ":"
            + ",".join(sorted(s.owners())) + ":" + ",".join(sorted(s.slots()))
            for s in shares)),
        ("runtime_bytes", _ints(sorted(RuntimeLayout(*layout).bytes()))),
    ]


def _cfg_values(fns, instrs) -> list[tuple[str, str]]:
    """The TAC CFG's values (passes/optimization/cfg.py) on each
    function, and `recursion_saves.defs_in` on each instruction. A
    dominance frontier and a loop body are sets, and they are written in
    the order Python iterates them, since the passes that read them walk
    them in that order. A block is `id/instructions/predecessors/
    successors`, with the lists' items separated by `.`."""
    from passes.optimization import cfg as tac_cfg
    from passes.recursion_saves import defs_in as saves_defs_in

    def dots(xs):
        return ".".join(str(x) for x in xs)

    def graph(fn):
        c = tac_cfg.build_cfg(fn)
        blocks = ";".join(
            f"{b}/{len(blk.instructions)}/{dots(blk.predecessors)}/"
            f"{dots(blk.successors)}" for b, blk in c.blocks.items())
        return f"{blocks}|{dots(c.block_order)}"

    def per_fn(f):
        return _fields(_try(lambda fn=fn: f(tac_cfg.build_cfg(fn))) for fn in fns)

    return [
        ("tac_cfg", _fields(_try(lambda fn=fn: graph(fn)) for fn in fns)),
        ("reverse_postorder", per_fn(
            lambda c: dots(tac_cfg.reverse_postorder(c)))),
        ("immediate_dominators", per_fn(lambda c: ";".join(
            f"{b}:{p}" for b, p in tac_cfg.immediate_dominators(c).items()))),
        ("dominator_tree_children", per_fn(lambda c: ";".join(
            f"{b}:{dots(ch)}" for b, ch in tac_cfg.dominator_tree_children(
                tac_cfg.immediate_dominators(c)).items()))),
        ("dominance_frontiers", per_fn(lambda c: ";".join(
            f"{b}:{dots(df)}"
            for b, df in tac_cfg.dominance_frontiers(c).items()))),
        ("natural_loops", per_fn(lambda c: ";".join(
            f"{h}:{dots(body)}" for h, body in tac_cfg.natural_loops(c)))),
        ("recursion_saves_defs_in", _fields(
            ",".join(v.name for v in saves_defs_in(i)) for i in instrs)),
    ]


def _index_by_identity(xs, x) -> int:
    return next(k for k, y in enumerate(xs) if y is x)


def pseudo_values(names) -> list[tuple[str, str]]:
    """`parse_pseudo_name` of each name, as `source|byte`, with "-" for
    a name with no source."""
    from passes.zp_slot_naming import parse_pseudo_name

    def text(n):
        src, byte = parse_pseudo_name(n)
        return f"{'-' if src is None else src}|{byte}"

    return [("parse_pseudo_name", _fields(text(n) for n in names))]


# --- Recording ---

_OUT: Path | None = None
_SOURCE = ""
# The slot symbols of the fixed point under way, or of the last one in
# the compile.
_SLOTS: dict | None = None
# The functions evaluated in this process: (ids of the instructions,
# the context's text) for the ones seen as objects, kept with the
# instructions so the ids stay theirs.
_DONE: dict = {}
_COUNTS = {"fn": 0, "prog": 0, "tac": 0, "tables": 0, "pseudos": 0, "tacopt": 0,
           "c99types": 0}


def tac_opt_values(fn, symbols, ssa_dsts) -> list[tuple[str, str]]:
    """The values of the helpers that the TAC optimizer's passes share
    (step 5 of the port), on a function one of them is given, with the
    symbols of its operands and the SSA names. Each is a list of fields,
    one per instruction or symbol, separated by spaces, with "-" for an
    empty field.

    - `max_value`: `value_range.max_value` of each operand `vals_in`
      gives, in order, separated by commas, with "." for None. A Var's
      definition is the one `DefUsePass` indexes.
    - `type_queries`: `int_width`, `type_max`, and `is_unsigned_type` of
      each symbol's type, separated by commas, with "." for None.
    - `rewrite_vars` and `rewrite_consts`: `var_visit.rewrite_uses_in`
      of each instruction, with a mapper that renames every Var `x` to
      `x'`, and one that makes every constant the `int` 7. A field is
      "=" when the same instruction comes back, and otherwise the dump
      of the new one with "_" for each space."""
    import tac_ast
    from passes.optimization.framework.defuse import _build_def_idx
    from passes.optimization.value_range import (
        int_width, is_unsigned_type, max_value, type_max,
    )
    from passes.optimization.var_visit import rewrite_uses_in, vals_in
    instrs = fn.instructions
    def_idx = _build_def_idx(instrs)

    def def_of(v):
        i = def_idx.get(v.name)
        return None if i is None else instrs[i]

    def opt(x):
        return "." if x is None else str(x)

    bounds = [
        ",".join(opt(max_value(v, def_of, symbols, ssa_dsts))
                 for v in vals_in(ins))
        for ins in instrs
    ]
    queries = [
        ",".join((opt(int_width(sym.type)), opt(type_max(sym.type)),
                  "1" if is_unsigned_type(sym.type) else "0"))
        for _, sym in symbols.items()
    ]

    def rename(v):
        if isinstance(v, tac_ast.Var):
            return tac_ast.Var(name=v.name + "'")
        return v

    seven = tac_ast.Constant(const=tac_ast.ConstInt(value=7))

    def constants(v):
        return seven if isinstance(v, tac_ast.Constant) else v

    def rewritten(mapper):
        out = []
        for ins in instrs:
            new = rewrite_uses_in(ins, mapper)
            out.append("=" if new is ins else _dump1(new).replace(" ", "_"))
        return out

    return [
        ("max_value", _fields(bounds)),
        ("type_queries", _fields(queries)),
        ("rewrite_vars", _fields(rewritten(rename))),
        ("rewrite_consts", _fields(rewritten(constants))),
    ]


# The values `c99_coerce` reduces to each integer type: the edges of each
# width's range and values past them, negative ones among them.
_COERCE_VALUES = (
    0, 1, -1, 2, 127, 128, -128, -129, 255, 256, 32767, 32768, -32768,
    65535, 65536, -65536, 2**31 - 1, 2**31, -(2**31), 2**32 - 1, 2**32,
    2**63 - 1, 2**63, -(2**63), 2**64 - 1, 2**64, -(2**64), 12345678901,
)


def _c99_types(prog, symbols) -> list:
    """Every c99 type the type-checked `prog` and its symbol table hold,
    the types inside other types among them, each once (by its repr),
    in the order of a walk over the program's fields and then the
    table, with the types that have no fields first so that every one
    is there."""
    import dataclasses
    import c99_ast
    from passes.dataclass_fields import fields_of
    seen: dict[str, object] = {}

    def add(t) -> None:
        seen.setdefault(repr(t), t)

    for cls in (c99_ast.Int, c99_ast.Long, c99_ast.LongLong, c99_ast.UInt,
                c99_ast.ULong, c99_ast.ULongLong, c99_ast.Float,
                c99_ast.Double, c99_ast.Char, c99_ast.SChar, c99_ast.UChar,
                c99_ast.Short, c99_ast.UShort, c99_ast.Bool,
                c99_ast.NullptrT, c99_ast.Void):
        add(cls())
    # The corpus has few bit-precise types and qualified pointers, so a
    # set of them is in every record too.
    for width in (1, 2, 3, 7, 8, 9, 15, 16, 17, 24, 31, 32, 33, 48, 63, 64):
        add(c99_ast.BitInt(width=width, is_signed=False))
        if width >= 2:
            add(c99_ast.BitInt(width=width, is_signed=True))
    add(c99_ast.Const(referenced_type=c99_ast.Volatile(
        referenced_type=c99_ast.Int())))
    add(c99_ast.Pointer(referenced_type=c99_ast.Const(
        referenced_type=c99_ast.Void())))
    add(c99_ast.Volatile(referenced_type=c99_ast.Pointer(
        referenced_type=c99_ast.Void())))

    def walk(v) -> None:
        if isinstance(v, list):
            for x in v:
                walk(x)
            return
        if not dataclasses.is_dataclass(v) or isinstance(v, type):
            return
        if isinstance(v, c99_ast.Type_data_type):
            add(v)
        for f in fields_of(v):
            walk(getattr(v, f.name))

    walk(prog)
    for _, sym in symbols.items():
        walk(sym.type)
    return list(seen.values())


def front_values(types, decls, names) -> list[tuple[str, str]]:
    """The values of the c99 type queries that the front end's passes
    share (step 6 of the port), on the types of a type-checked program,
    its top-level declarations, and the names of its symbol table. Each
    value is the JSON list of one string per type (or per pair of
    types, declaration, or name), with "." where a query doesn't apply.

    - `c99_repr_type`, `c99_repr_decl`: Python's repr of each type and
      declaration.
    - `c99_strip_quals`, `c99_promote`: the repr of `_strip_quals` and
      `_promote_integer` of each type.
    - `c99_predicates`: a 0 or 1 for each of `_is_integer_type`,
      `_is_floating_type`, `_is_pointer_type`, `_is_struct_or_union`,
      `_is_array_type`, `_is_nullptr_type`, `_is_void`, and
      `_is_void_pointer`.
    - `c99_c_type`: `_c_type` of each type.
    - `c99_int_queries`: `_int_width`, `_int_bits`, and `_is_signed` of
      each integer type.
    - `c99_rank`: `_rank` of each integer type the promotions leave.
    - `c99_coerce`: `_coerce_int_to_type` of each of `_COERCE_VALUES`
      for each integer type.
    - `c99_common_type`: the repr of `_common_type` of each ordered pair
      of the promoted arithmetic types, which are each arithmetic type's
      `_promote_integer`, each once, in the order of the types.
    - `c99_display_name`: `_display_name` of each name."""
    from passes import type_checking as tc

    def js(xs) -> str:
        return json.dumps(list(xs))

    def bit(b) -> str:
        return "1" if b else "0"

    ints = [tc._is_integer_type(t) for t in types]
    promoted: dict[str, object] = {}
    for t in types:
        if tc._is_integer_type(t) or tc._is_floating_type(t):
            p = tc._promote_integer(t)
            promoted.setdefault(repr(p), p)
    arith = list(promoted.values())
    ranked = (tc.Int, tc.UInt, tc.Long, tc.ULong, tc.LongLong, tc.ULongLong,
              tc.BitInt)
    return [
        ("c99_repr_type", js(repr(t) for t in types)),
        ("c99_repr_decl", js(repr(d) for d in decls)),
        ("c99_strip_quals", js(repr(tc._strip_quals(t)) for t in types)),
        ("c99_promote", js(repr(tc._promote_integer(t)) for t in types)),
        ("c99_predicates", js(
            "".join(bit(f(t)) for f in (
                tc._is_integer_type, tc._is_floating_type,
                tc._is_pointer_type, tc._is_struct_or_union,
                tc._is_array_type, tc._is_nullptr_type, tc._is_void,
                tc._is_void_pointer,
            ))
            for t in types)),
        ("c99_c_type", js(tc._c_type(t) for t in types)),
        ("c99_int_queries", js(
            f"{tc._int_width(t)},{tc._int_bits(t)},{bit(tc._is_signed(t))}"
            if is_int else "."
            for t, is_int in zip(types, ints))),
        ("c99_rank", js(
            ",".join(map(str, tc._rank(t))) if isinstance(t, ranked) else "."
            for t in types)),
        ("c99_coerce", js(
            ",".join(str(tc._coerce_int_to_type(v, t))
                     for v in _COERCE_VALUES) if is_int else "."
            for t, is_int in zip(types, ints))),
        ("c99_common_type", js(
            repr(tc._common_type(a, b)) for a in arith for b in arith)),
        ("c99_display_name", js(tc._display_name(n) for n in names)),
    ]


def _record_front(result) -> None:
    """Writes a `c99types` record of what the type checker returned:
    one line of JSON with the dumps of its types (`_c99_types`) and of
    its top-level declarations, and the names of its symbol table."""
    import c99_ast
    prog, symbols, _ = result
    types = _c99_types(prog, symbols)
    decls = prog.declaration
    names = [name for name, _ in symbols.items()]
    body = json.dumps({
        "types": [capture_passes.dump(t) for t in types],
        "decls": [
            capture_passes.dump(c99_ast.Program(declaration=[d]))
            for d in decls
        ],
        "names": names,
    }) + "\n"
    k = hashlib.sha1(body.encode("utf-8")).hexdigest()
    if (_OUT / "c99types" / f"{k}.txt").exists():
        return
    _write_keyed(
        "c99types", k,
        f"c6502-helpers 1\nsource {_SOURCE}\nc99types\n" + body,
        front_values(types, decls, names),
    )


def _wrap_type_check(fn):
    def wrapper(*args, **kwargs):
        result = fn(*args, **kwargs)
        _record_front(result)
        return result
    return wrapper


def _record_tac_opt(fn, ctx) -> None:
    """Writes a `tacopt` record of the function a pass of the TAC
    optimizer's driver is given, with the symbols of its operands, in
    the table's order, and the SSA names."""
    import ir_dump
    from passes.optimization.var_visit import vals_in
    from passes.type_checking import SymbolTable
    import tac_ast
    if ctx.symbols is None:
        return
    names = {
        v.name for ins in fn.instructions for v in vals_in(ins)
        if isinstance(v, tac_ast.Var)
    }
    table = SymbolTable()
    for name, sym in ctx.symbols.items():
        if name in names:
            table[name] = sym
    body = json.dumps({
        "function": {"asdl": "tac_ast", "dump": ir_dump.dump(fn)},
        "symbols": capture_passes.encode(table),
        "ssa_dsts": capture_passes.encode(ctx.ssa_dsts),
    }) + "\n"
    k = hashlib.sha1(body.encode("utf-8")).hexdigest()
    if (_OUT / "tacopt" / f"{k}.txt").exists():
        return
    _write_keyed(
        "tacopt", k, f"c6502-helpers 1\nsource {_SOURCE}\ntacopt\n" + body,
        tac_opt_values(fn, table, ctx.ssa_dsts),
    )


def _wrap_tac_pass(run):
    def wrapper(fn, ctx):
        _record_tac_opt(fn, ctx)
        return run(fn, ctx)
    return wrapper


def _record_program(prog, kwargs) -> None:
    import asm_ast
    import ir_dump
    from passes import asm_aliasing
    zp_names = kwargs.get("zp_names")
    slot_addrs = kwargs.get("slot_addrs")
    if slot_addrs is None:
        slot_addrs = frozenset(
            a for a in (_SLOTS or {}).values() if a <= 0xFF)
    ctx = make_context(prog, _SLOTS, zp_names, slot_addrs)
    ctx_text = json.dumps(ctx, separators=(",", ":"))
    for tl in prog.top_level:
        if not isinstance(tl, asm_ast.Function):
            continue
        key = (tuple(map(id, tl.instructions)), tl.name, ctx_text)
        if key in _DONE:
            continue
        _DONE[key] = tuple(tl.instructions)
        header = (f"c6502-helpers 1\nsource {_SOURCE}\ncontext {ctx_text}\n"
                  "function\n")
        body = ir_dump.dump(tl)
        k = hashlib.sha1(f"{ctx_text}\n{body}".encode("utf-8")).hexdigest()
        if (_OUT / "fn" / f"{k}.txt").exists():
            continue
        values = function_values(tl, ctx)
        _write_keyed("fn", k, header + body, values)
    reduced = reduced_program(prog)
    body = ir_dump.dump(reduced)
    k = hashlib.sha1(body.encode("utf-8")).hexdigest()
    if not (_OUT / "prog" / f"{k}.txt").exists():
        names = sorted(asm_aliasing.unreachable_by_pointer(prog))
        _write_keyed(
            "prog", k, f"c6502-helpers 1\nsource {_SOURCE}\nprogram\n" + body,
            [("unreachable_by_pointer", _names(names))],
        )


def _write_keyed(kind: str, key: str, head: str, values) -> None:
    path = _OUT / kind / f"{key}.txt"
    if path.exists():
        return
    text = head + "values\n" + "".join(f"{k} {v}\n" for k, v in values)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(f"{key}.{os.getpid()}.tmp")
    with open(tmp, "wb") as f:
        f.write(text.encode("utf-8"))
    try:
        os.replace(tmp, path)
    except PermissionError:
        # On Windows the rename fails while another worker renames its
        # copy of the same record into place, which has the same key and
        # so the same text (capture_passes._record does the same).
        if not path.exists():
            raise
        tmp.unlink()
        return
    _COUNTS[kind] += 1


def _record_tac(prog) -> None:
    import ir_dump
    import tac_ast
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        body = ir_dump.dump(tl)
        k = hashlib.sha1(body.encode("utf-8")).hexdigest()
        _write_keyed("tac", k, f"c6502-helpers 1\nsource {_SOURCE}\ntac\n" + body,
                     tac_values(tl))


def _wrap_pass(fn):
    def wrapper(*args, **kwargs):
        import asm_ast
        # The table also holds emission's functions, of which
        # `emit_program` takes an asm2 program and `format_metadata` a
        # `LinkMetadata`. Neither reads the shared helpers.
        if isinstance(args[0], asm_ast.Program):
            _record_program(args[0], kwargs)
        return fn(*args, **kwargs)
    return wrapper


def _wrap_fixedpoint(fn):
    def wrapper(*args, **kwargs):
        global _SLOTS
        _SLOTS = kwargs.get("zp_slot_symbols")
        return fn(*args, **kwargs)
    return wrapper


def _record_stage_tables(tac, symbols, types, shares) -> None:
    import json as json_module
    from passes.runtime_layout import current
    layout = current()
    layout_list = [layout.ssp, layout.fp, layout.hargs, layout.hargs_size,
                   layout.dptr]
    body = json_module.dumps({
        "tac": capture_passes.encode(tac),
        "symbols": capture_passes.encode(symbols),
        "types": capture_passes.encode(types),
        "shares": capture_passes.encode(shares),
        "layout": layout_list,
    }) + "\n"
    k = hashlib.sha1(body.encode("utf-8")).hexdigest()
    _write_keyed(
        "tables", k, f"c6502-helpers 1\nsource {_SOURCE}\ntables\n" + body,
        stage_values(tac, symbols, types, shares, layout_list),
    )


def _wrap_find_shares(fn):
    def wrapper(*args, **kwargs):
        result = fn(*args, **kwargs)
        tac, _abi, symbols, types = args[:4]
        _record_stage_tables(tac, symbols, types, result[0])
        return result
    return wrapper


def _record_pseudos(prog, colorings) -> None:
    import json as json_module
    import re
    import ir_dump
    names: dict[str, None] = {}
    for name in re.findall(r'Pseudo\("([^"]*)"', ir_dump.dump(prog)):
        names.setdefault(name, None)
    for coloring in (colorings or {}).values():
        for name in coloring.assignments:
            names.setdefault(name, None)
    body = json_module.dumps({"names": list(names)}) + "\n"
    k = hashlib.sha1(body.encode("utf-8")).hexdigest()
    _write_keyed(
        "pseudos", k, f"c6502-helpers 1\nsource {_SOURCE}\npseudos\n" + body,
        pseudo_values(list(names)),
    )


def _wrap_replace(fn):
    def wrapper(*args, **kwargs):
        _record_pseudos(args[0], kwargs.get("colorings"))
        return fn(*args, **kwargs)
    return wrapper


def _wrap_select_abi(fn):
    def wrapper(*args, **kwargs):
        _record_tac(args[0])
        return fn(*args, **kwargs)
    return wrapper


def install(out: Path, stage_only: bool = False, tac_only: bool = False,
            front_only: bool = False) -> None:
    """Wraps the passes in `compile`'s namespace to record the helpers'
    values under `out`. With `stage_only`, only the records of the code
    generation helpers (`tables` and `pseudos`) are made, with
    `tac_only` only those of the TAC optimizer's helpers (`tacopt`),
    made at each call of a pass of its driver, and with `front_only`
    only those of the c99 type queries (`c99types`), made at each call
    of the type checker."""
    global _OUT
    import compile as compile_module
    _OUT = out
    if front_only:
        compile_module.type_check_program = _wrap_type_check(
            compile_module.type_check_program)
        return
    if tac_only:
        from passes.optimization.optimizer import _DRIVER
        for slot in capture_passes._DRIVER_SLOTS:
            for p in getattr(_DRIVER, slot):
                p.run = _wrap_tac_pass(p.run)
        return
    if not stage_only:
        for attr in capture_passes.PASSES.values():
            setattr(compile_module, attr,
                    _wrap_pass(getattr(compile_module, attr)))
        compile_module._peephole_fixedpoint = _wrap_fixedpoint(
            compile_module._peephole_fixedpoint)
        compile_module.select_abi = _wrap_select_abi(compile_module.select_abi)
    compile_module.find_shares = _wrap_find_shares(compile_module.find_shares)
    for attr in ("replace_pseudoregs", "replace_pseudoregs_bare_exit"):
        setattr(compile_module, attr,
                _wrap_replace(getattr(compile_module, attr)))


def _init_worker(out: str, stage_only: bool, tac_only: bool = False,
                 front_only: bool = False) -> None:
    # The values recorded are those the Python passes see, and they
    # don't run when C6502_CPP_BACKEND=1 hands the back end to the C++
    # port.
    os.environ.pop("C6502_CPP_BACKEND", None)
    install(Path(out), stage_only, tac_only, front_only)


def _run_job(job) -> tuple[str, bool, dict[str, int]]:
    global _SOURCE, _SLOTS
    import compile as compile_module
    label, args, mode = job
    _SOURCE = f"{label} -S{' -O' if mode == 'O' else ''}"
    _SLOTS = None
    _DONE.clear()
    for k in _COUNTS:
        _COUNTS[k] = 0
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
    return _SOURCE, ok, dict(_COUNTS)


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(
        description="Record the shared asm helpers' values over the corpus.",
    )
    ap.add_argument("--out", default=str(_ROOT / "captures" / "helpers"))
    ap.add_argument("--corpus", default="examples,games,fuzz")
    ap.add_argument("--fuzz-seeds", default="0-99")
    ap.add_argument("--modes", default="O,unopt")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1)
    ap.add_argument("--stage-only", action="store_true",
                    help="record only the code generation helpers")
    ap.add_argument("--tac-only", action="store_true",
                    help="record only the TAC optimizer's helpers")
    ap.add_argument("--front-only", action="store_true",
                    help="record only the c99 type queries of the front end")
    ns = ap.parse_args(argv)
    jobs = [
        (label, args, mode)
        for label, args in capture_passes.corpus(
            ns.corpus.split(","), capture_passes._parse_seeds(ns.fuzz_seeds))
        for mode in ns.modes.split(",")
    ]
    out = Path(ns.out)
    out.mkdir(parents=True, exist_ok=True)
    start = time.time()
    totals = dict.fromkeys(_COUNTS, 0)
    failed = 0
    with ProcessPoolExecutor(
        max_workers=ns.j, initializer=_init_worker,
        initargs=(str(out), ns.stage_only, ns.tac_only, ns.front_only),
    ) as pool:
        for label, ok, counts in pool.map(_run_job, jobs, chunksize=1):
            if not ok:
                failed += 1
                print(f"failed to compile: {label}")
            for k, v in counts.items():
                totals[k] += v
    print(f"{len(jobs)} compiles ({failed} failed) in {time.time() - start:.0f} s")
    for k, v in totals.items():
        print(f"{k}: {v} records written")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
