"""Checks the IR between passes, so a pass that breaks it fails there.

An end-to-end test or a fuzzer finds a miscompile only when it changes
a program's result for the inputs it runs. A pass that leaves the IR
inconsistent, for example reading a temporary nothing defines, often
gives the right result anyway, and when it doesn't, the wrong value
shows up many passes later. These checks run after every pass instead
and name the pass that broke the IR.

They're off unless `C6502_CHECK_IR=1` is set in the environment or
`enable()` is called. The fuzzers turn them on, and
`C6502_CHECK_IR=1 uv run python -m unittest` runs the test suite with
them.

TAC (`check_tac`, after each TAC optimizer pass):

  * Labels are unique, and every jump goes to one of them.
  * Every variable is in the symbol table.
  * In SSA form, an SSA-renamed variable is defined once, and every
    read of it is dominated by its definition (for a Phi argument,
    the end of the predecessor it comes from). A parameter's first
    value is defined at entry. Unreachable blocks are ignored, since
    SSA renaming never visits them.
  * Outside SSA form, every compiler temporary (`%...`) that's read
    is defined somewhere in the function, unless it's an object
    accessed in memory (its address taken, or indexed by name).

Asm (`check_asm`, at the stage boundaries and after each peephole
sweep):

  * Labels are unique in each function, and every branch, and every
    jump to a local label (one starting with `.`), goes to one of
    them.
  * No `Phi` outside the asm optimizer's SSA form, and no `Pseudo`
    once pseudoregisters have been replaced.
"""
from __future__ import annotations

import os

import asm_ast
import tac_ast

_enabled = os.environ.get("C6502_CHECK_IR") == "1"


class IRCheckError(AssertionError):
    """The IR broke an invariant. The message names the pass after
    which it was found."""


def enable(on: bool = True) -> None:
    global _enabled
    _enabled = on


def enabled() -> bool:
    return _enabled


# ---------------------------------------------------------------------------
# TAC.
# ---------------------------------------------------------------------------

_JUMPS = (
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked,
)


def check_tac(
    fn: tac_ast.Function, *, after: str, symbols=None,
    ssa_dsts: set[str] | None = None,
) -> None:
    """Raises `IRCheckError` if `fn` breaks a TAC invariant (see the
    module docstring). `ssa_dsts` is the set of SSA-renamed names,
    given only while the function is in SSA form."""
    if not _enabled:
        return
    problems = _tac_problems(fn, symbols, ssa_dsts)
    if problems:
        raise IRCheckError(
            f"after {after}, function '{fn.name}': " + "; ".join(problems[:5])
        )


def _tac_problems(fn, symbols, ssa_dsts) -> list[str]:
    from passes.optimization.var_visit import uses_in
    from passes.recursion_saves import defs_in
    problems: list[str] = []
    instrs = fn.instructions
    labels: set[str] = set()
    for instr in instrs:
        if isinstance(instr, tac_ast.Label):
            if instr.name in labels:
                problems.append(f"label {instr.name} defined twice")
            labels.add(instr.name)
    for instr in instrs:
        if isinstance(instr, _JUMPS) and instr.target not in labels:
            problems.append(f"jump to missing label {instr.target}")
    if symbols is not None:
        names = {
            v.name for instr in instrs
            for v in (*uses_in(instr), *defs_in(instr))
        }
        for n in sorted(names - set(fn.params)):
            if symbols.get(n) is None:
                problems.append(f"{n} isn't in the symbol table")
    if problems:
        return problems
    if ssa_dsts is not None:
        problems += _ssa_problems(fn, ssa_dsts)
    else:
        problems += _temp_problems(fn)
    return problems


def _ssa_problems(fn, ssa_dsts) -> list[str]:
    from passes.optimization.cfg import (
        ENTRY_ID, build_cfg, immediate_dominators,
    )
    from passes.optimization.var_visit import uses_in
    from passes.recursion_saves import defs_in
    problems: list[str] = []
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    # Where each SSA name is defined: (block, index in block). A
    # parameter's first SSA value is defined at entry. Unreachable
    # blocks are skipped: renaming never visits them, so they keep
    # the original names, and UCE drops them.
    def_at: dict[str, tuple[int, int]] = {
        p: (ENTRY_ID, -1) for p in fn.params if p in ssa_dsts
    }
    label_block: dict[str, int] = {}
    for bid in cfg.block_order:
        for k, instr in enumerate(cfg.blocks[bid].instructions):
            if isinstance(instr, tac_ast.Label):
                label_block[instr.name] = bid
            if bid not in idom:
                continue
            for d in defs_in(instr):
                if d.name not in ssa_dsts:
                    continue
                if d.name in def_at:
                    problems.append(f"SSA name {d.name} defined twice")
                def_at[d.name] = (bid, k)

    def dominates(a: int, b: int) -> bool:
        cur = b
        while True:
            if cur == a:
                return True
            nxt = idom.get(cur)
            if nxt is None or nxt == cur:
                return False
            cur = nxt

    def check_read(name: str, bid: int, k: int, what: str) -> None:
        where = def_at.get(name)
        if where is None:
            problems.append(f"{what} reads {name}, which nothing defines")
            return
        dbid, dk = where
        if dbid == bid:
            if dk >= k:
                problems.append(f"{what} reads {name} before its definition")
        elif not dominates(dbid, bid):
            problems.append(
                f"{what} reads {name}, whose definition doesn't dominate it"
            )

    for bid in cfg.block_order:
        if bid not in idom:
            continue  # unreachable; UCE drops it
        for k, instr in enumerate(cfg.blocks[bid].instructions):
            what = type(instr).__name__
            if isinstance(instr, tac_ast.Phi):
                for arg in instr.args:
                    src = arg.source
                    if not (isinstance(src, tac_ast.Var) and src.name in ssa_dsts):
                        continue
                    pred = label_block.get(arg.pred_label)
                    if pred is None:
                        problems.append(
                            f"Phi argument from missing label {arg.pred_label}"
                        )
                        continue
                    if pred in idom:
                        # Read at the end of the predecessor.
                        check_read(
                            src.name, pred,
                            len(cfg.blocks[pred].instructions), what,
                        )
                continue
            for u in uses_in(instr):
                if u.name in ssa_dsts:
                    check_read(u.name, bid, k, what)
    return problems


def _temp_problems(fn) -> list[str]:
    from passes.optimization.var_visit import uses_in
    from passes.recursion_saves import defs_in
    defined: set[str] = set(fn.params)
    in_memory: set[str] = set()
    for instr in fn.instructions:
        defined |= {d.name for d in defs_in(instr)}
        match instr:
            case tac_ast.GetAddress(operand=tac_ast.Var(name=n)):
                in_memory.add(n)
            case tac_ast.IndexedLoad(name=n) | tac_ast.IndexedSymbolStore(name=n):
                in_memory.add(n)
    problems: list[str] = []
    for instr in fn.instructions:
        for u in uses_in(instr):
            if (
                u.name.startswith("%") and u.name not in defined
                and u.name not in in_memory
            ):
                problems.append(
                    f"{type(instr).__name__} reads {u.name}, which nothing "
                    f"defines"
                )
    return problems


# ---------------------------------------------------------------------------
# Asm.
# ---------------------------------------------------------------------------


def check_asm(
    prog: asm_ast.Program, *, after: str, pseudos: bool = False,
    phis: bool = False,
) -> None:
    """Raises `IRCheckError` if `prog` breaks an asm invariant (see the
    module docstring). `pseudos` / `phis` say whether `Pseudo`
    operands / `Phi` nodes may still appear."""
    if not _enabled:
        return
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            problems = _asm_problems(tl, pseudos, phis)
            if problems:
                raise IRCheckError(
                    f"after {after}, function '{tl.name}': "
                    + "; ".join(problems[:5])
                )


def _asm_problems(fn, pseudos, phis) -> list[str]:
    from passes.optimization_asm.optimizer import _all_operands_in
    problems: list[str] = []
    labels: set[str] = set()
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.Label):
            if instr.name in labels:
                problems.append(f"label {instr.name} defined twice")
            labels.add(instr.name)
    for instr in fn.instructions:
        if isinstance(instr, asm_ast.Branch) and instr.target not in labels:
            problems.append(f"branch to missing label {instr.target}")
        if (
            isinstance(instr, asm_ast.Jump) and instr.target.startswith(".")
            and instr.target not in labels
        ):
            problems.append(f"jump to missing label {instr.target}")
        if not phis and isinstance(instr, asm_ast.Phi):
            problems.append("Phi outside SSA form")
        if not pseudos and any(
            isinstance(op, asm_ast.Pseudo) for op in _all_operands_in(instr)
        ):
            problems.append(f"Pseudo left in {type(instr).__name__}")
    return problems
