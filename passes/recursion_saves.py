"""Saving the values live across a recursive call on the hardware stack.

A function on a cycle of the call graph normally can't keep its locals
in a private zero-page pool, as other functions do
(`passes/zp_local_allocation.py`): the call that recurses runs the same
code and would overwrite them. Such a function uses the shared
caller-saved / callee-saved split instead, and saves every callee-saved
byte it uses in its prologue and restores it in its epilogue, on every
call, recursive or not.

`#pragma c6502 call save(hardware_stack)` before a statement or a
declaration marks the calls in it (`tac_ast.FunctionCall.hw_stack_save`). It tells the
compiler that the caller may save what it needs across those calls on
the hardware stack. The compiler can't bound how deep the recursion
goes, so the user vouches for the stack's 256 bytes.

When every call that stays inside a cycle is marked
(`hw_stack_cycles`), and the cycle is otherwise eligible, the functions
on it get private pools like any other function. A function on the
cycle has its pool to itself except for the calls that recurse, so
`tac_to_asm` saves around each of those the values live across it
(`plan_saves`):

    LDA v      ; just after v's last definition in the call's block,
    PHA        ; or else just before the call
    JSR f
    PLA        ; in the reverse order, after the call's result is
    STA v      ; stored

With nothing live across the call in zero page, nothing needs saving at
entry or exit. A value copied from a global only to be saved becomes
`LDA g; PHA` once the copy is folded away.
"""
from __future__ import annotations

from dataclasses import dataclass, field

import tac_ast
from passes.optimization.cfg import build_cfg
from passes.optimization.var_visit import defs_in as _var_defs_in, uses_in
from passes.type_checking import StaticAttr, _is_volatile_qualified


class RecursionSaveError(Exception):
    """`#pragma c6502 call save(hardware_stack)` on a statement it
    can't apply to."""


_PRAGMA = "`#pragma c6502 call save(hardware_stack)`"


def call_graph(prog: tac_ast.Program) -> dict[str, set[str]]:
    """Direct call edges among the functions defined in `prog`."""
    names = {
        tl.name for tl in prog.top_level
        if isinstance(tl, tac_ast.Function)
    }
    graph: dict[str, set[str]] = {n: set() for n in names}
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if isinstance(instr, tac_ast.FunctionCall) and instr.name in names:
                graph[tl.name].add(instr.name)
    return graph


def cycles(graph: dict[str, set[str]]) -> dict[str, frozenset[str]]:
    """Maps each function on a cycle of `graph` to its cycle (its
    strongly connected component). A function that calls only itself
    is a cycle of one. Functions on no cycle aren't keys."""
    index: dict[str, int] = {}
    lowlink: dict[str, int] = {}
    stack: list[str] = []
    on_stack: set[str] = set()
    out: dict[str, frozenset[str]] = {}

    def visit(v: str) -> None:
        index[v] = lowlink[v] = len(index)
        stack.append(v)
        on_stack.add(v)
        for w in sorted(graph.get(v, ())):
            if w not in graph:
                continue
            if w not in index:
                visit(w)
                lowlink[v] = min(lowlink[v], lowlink[w])
            elif w in on_stack:
                lowlink[v] = min(lowlink[v], index[w])
        if lowlink[v] == index[v]:
            component: list[str] = []
            while True:
                w = stack.pop()
                on_stack.discard(w)
                component.append(w)
                if w == v:
                    break
            if len(component) > 1 or v in graph.get(v, ()):
                members = frozenset(component)
                for w in component:
                    out[w] = members

    for n in sorted(graph):
        if n not in index:
            visit(n)
    return out


def check_pragmas(prog: tac_ast.Program) -> None:
    """Raises `RecursionSaveError` for a marked call that can't
    recurse: one to a function that can't call the caller back."""
    cyc = cycles(call_graph(prog))
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function):
            continue
        for instr in tl.instructions:
            if (
                isinstance(instr, tac_ast.FunctionCall)
                and instr.hw_stack_save
                and instr.name not in cyc.get(tl.name, ())
            ):
                raise RecursionSaveError(
                    f"{_PRAGMA} is on a call to '{instr.name}' in "
                    f"'{tl.name}', but '{instr.name}' can't call "
                    f"'{tl.name}' again, so the call can't recurse"
                )


def hw_stack_cycles(prog: tac_ast.Program) -> dict[str, frozenset[str]]:
    """The cycles whose every call from one member to another is
    marked, as `cycles` maps them. Only these can have private pools:
    a call that recurses without the mark would need its caller's
    bytes saved somewhere other than the hardware stack."""
    cyc = cycles(call_graph(prog))
    unmarked: set[str] = set()
    for tl in prog.top_level:
        if not isinstance(tl, tac_ast.Function) or tl.name not in cyc:
            continue
        for instr in tl.instructions:
            if (
                isinstance(instr, tac_ast.FunctionCall)
                and instr.name in cyc[tl.name]
                and not instr.hw_stack_save
            ):
                unmarked.add(tl.name)
    bad: set[str] = set()
    for n in unmarked:
        bad |= cyc[n]
    return {n: members for n, members in cyc.items() if n not in bad}


@dataclass
class SavePlan:
    """Where `tac_to_asm` saves and restores in one function, keyed by
    TAC instruction index. Each entry is a saved value's name and the
    TAC val its bytes come from (a push) or go to (a pull).
    `push_before[i]` are pushed just before marked call `i`,
    `push_after[i]` just after instruction `i` (their last definition
    before the call), and `pop_after[i]` are pulled after marked call
    `i`, in that order. The instructions in `skip` aren't translated:
    a pull already stored their value."""
    push_before: dict[int, list[tuple[str, tac_ast.Type_val]]] = field(
        default_factory=dict,
    )
    push_after: dict[int, list[tuple[str, tac_ast.Type_val]]] = field(
        default_factory=dict,
    )
    pop_after: dict[int, list[tuple[str, tac_ast.Type_val]]] = field(
        default_factory=dict,
    )
    skip: set[int] = field(default_factory=set)


# Instructions that touch no memory but the variables they name, so a
# global they don't name can be stored before them.
_REGISTER_ONLY = (
    tac_ast.Copy, tac_ast.Binary, tac_ast.Unary, tac_ast.SignExtend,
    tac_ast.ZeroExtend, tac_ast.Truncate,
)


def plan_saves(
    fn: tac_ast.Function, members: frozenset[str], symbols,
) -> SavePlan:
    """The saves around `fn`'s calls to the functions in `members`
    (its cycle). A value is saved when it's live across the call and
    could be in `fn`'s private pool: not a static, not a parameter
    (those are in the frame), and not an object whose address is
    taken or that's indexed by name (also in the frame).

    A value its last definition copies from somewhere is pushed from
    there, so `v = g` then `LDA v; PHA` becomes `LDA g; PHA` once the
    copy is found dead. A value whose first use after the call copies
    it to a global is pulled straight into the global, when nothing
    between the call and the copy can see the global."""
    statics = {
        n for n, sym in symbols.items() if isinstance(sym.attrs, StaticAttr)
    }
    in_memory = statics | set(fn.params)
    for instr in fn.instructions:
        match instr:
            case tac_ast.GetAddress(operand=tac_ast.Var(name=n)):
                in_memory.add(n)
            case (
                tac_ast.IndexedLoad(name=n)
                | tac_ast.IndexedSymbolStore(name=n)
            ):
                in_memory.add(n)

    def plain(val: tac_ast.Type_val) -> bool:
        """A val that can be read twice, or written early."""
        if isinstance(val, tac_ast.Constant):
            return True
        sym = symbols.get(val.name)
        return sym is not None and not _is_volatile_qualified(sym.type)

    live_out = _live_out(fn)
    plan = SavePlan()
    block_start = 0
    for i, instr in enumerate(fn.instructions):
        if isinstance(instr, tac_ast.Label):
            block_start = i
        if not (
            isinstance(instr, tac_ast.FunctionCall) and instr.name in members
        ):
            if _ends_block(instr):
                block_start = i + 1
            continue
        defined = {d.name for d in defs_in(instr)}
        saved = sorted(live_out[i] - defined - in_memory)
        # Each value's last definition between the block's start (or
        # the previous saved call) and this call.
        last_def: dict[str, int] = {}
        for k in range(block_start, i):
            for d in defs_in(fn.instructions[k]):
                if d.name in saved:
                    last_def[d.name] = k
        order = sorted(
            saved, key=lambda n: (last_def.get(n, i), n),
        )
        for n in order:
            src: tac_ast.Type_val = tac_ast.Var(name=n)
            if n not in last_def:
                plan.push_before.setdefault(i, []).append((n, src))
                continue
            d = fn.instructions[last_def[n]]
            if isinstance(d, tac_ast.Copy) and plain(d.src):
                src = d.src
            plan.push_after.setdefault(last_def[n], []).append((n, src))
        pops = []
        for n in reversed(order):
            j = _store_to_global(fn, i, n, statics, live_out, plain)
            if j is None or j in plan.skip:
                pops.append((n, tac_ast.Var(name=n)))
            else:
                plan.skip.add(j)
                pops.append((n, fn.instructions[j].dst))
        plan.pop_after[i] = pops
        block_start = i + 1
    return plan


def _store_to_global(
    fn: tac_ast.Function, call: int, name: str, statics: set[str],
    live_out: list[set[str]], plain,
) -> int | None:
    """The index of a `Copy(name, g)` to a global `g` that a pull
    after `call` can do instead: the first instruction after the call
    to name `name`, in the same block, after which `name` is dead,
    with only `_REGISTER_ONLY` instructions that don't name `g` before
    it."""
    between: list[tac_ast.Type_instruction] = []
    for j in range(call + 1, len(fn.instructions)):
        instr = fn.instructions[j]
        names = {v.name for v in uses_in(instr)} | {
            v.name for v in defs_in(instr)
        }
        if name in names:
            if not (
                isinstance(instr, tac_ast.Copy)
                and instr.src == tac_ast.Var(name=name)
                and isinstance(instr.dst, tac_ast.Var)
                and instr.dst.name in statics
                and plain(instr.dst)
                and name not in live_out[j]
            ):
                return None
            g = instr.dst.name
            for b in between:
                if g in {v.name for v in uses_in(b)} | {
                    v.name for v in defs_in(b)
                }:
                    return None
            return j
        if not isinstance(instr, _REGISTER_ONLY):
            return None
        between.append(instr)
    return None


def defs_in(instr: tac_ast.Type_instruction) -> list[tac_ast.Var]:
    """`var_visit.defs_in`, which leaves out `IndirectIndexedLoad`'s
    destination. Without it that value would look live everywhere."""
    if isinstance(instr, tac_ast.IndirectIndexedLoad):
        d = instr.dst
        return [d] if isinstance(d, tac_ast.Var) else []
    return _var_defs_in(instr)


def _ends_block(instr: tac_ast.Type_instruction) -> bool:
    return isinstance(instr, (
        tac_ast.Ret, tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
        tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
    ))


def _live_out(fn: tac_ast.Function) -> list[set[str]]:
    """The names live just after each instruction of `fn`."""
    cfg = build_cfg(fn)
    starts: dict[int, int] = {}
    pos = 0
    for bid in cfg.block_order:
        starts[bid] = pos
        pos += len(cfg.blocks[bid].instructions)
    assert pos == len(fn.instructions)
    use: dict[int, set[str]] = {}
    kill: dict[int, set[str]] = {}
    for bid in cfg.block_order:
        u: set[str] = set()
        k: set[str] = set()
        for instr in cfg.blocks[bid].instructions:
            u |= {v.name for v in uses_in(instr)} - k
            k |= {v.name for v in defs_in(instr)}
        use[bid], kill[bid] = u, k
    live_in: dict[int, set[str]] = {b: set() for b in cfg.blocks}
    changed = True
    while changed:
        changed = False
        for bid in reversed(cfg.block_order):
            out: set[str] = set()
            for s in cfg.blocks[bid].successors:
                out |= live_in[s]
            new_in = use[bid] | (out - kill[bid])
            if new_in != live_in[bid]:
                live_in[bid] = new_in
                changed = True
    result: list[set[str]] = [set() for _ in fn.instructions]
    for bid in cfg.block_order:
        live: set[str] = set()
        for s in cfg.blocks[bid].successors:
            live |= live_in[s]
        instrs = cfg.blocks[bid].instructions
        for j in range(len(instrs) - 1, -1, -1):
            result[starts[bid] + j] = set(live)
            live -= {v.name for v in defs_in(instrs[j])}
            live |= {v.name for v in uses_in(instrs[j])}
    return result
