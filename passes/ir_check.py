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

`check_tac` runs after every TAC pass, about 15,000 times in 20 fuzzer
programs, so it finds each instruction's reads and writes once per
check and gives them to every test. Nothing found about an
instruction is kept for the next check, since a pass may change an
instruction in place (unreachable-code elimination rewrites a Phi's
arguments). The blocks and dominators of the SSA test are kept,
keyed by the function's control skeleton (`_control_skeleton`): the
number of instructions and the position, name, type, and target of
each label and block-ending instruction, which is everything
`build_cfg` reads. The skeleton is taken afresh at each check, so an
entry is used only for a function whose CFG is the one it was
computed from, and a pass that changes a jump in place gets a new
one.

Asm (`check_asm`, at the stage boundaries and after each peephole
sweep):

  * Labels are unique in each function, and every branch, and every
    jump to a local label (one starting with `.`), goes to one of
    them.
  * No `Phi` outside the asm optimizer's SSA form, and no `Pseudo`
    once pseudoregisters have been replaced.
  * Every `DecimalArith` (packed BCD) has lists of one length, at
    least one byte long, and operands of the forms its expansion
    takes (`passes/decimal_arith.form_problems`).
  * No `SetDecimal` or `ClearDecimal`, which only the runtime helpers
    write.
  * An `INC`, `DEC`, or shift of an array element (`IndexedData`) is
    indexed by X, since the 6502 has no abs,Y form of them.

Asm2 (`check_asm2`, after `asm_to_asm2`):

  * The decimal flag is set only across a packed BCD chain. Every
    `SED` is followed by `CLC` and ADCs or by `SEC` and SBCs, each
    byte as `LDA src; ADC src; STA dst` with no register operand
    but the A these name, and then a `CLD`. Nothing else is between
    them, no `CLD` is anywhere else, and no function ends with the
    flag set. A soft-stack operand's `LDY #off` is part of its
    access, so it's inside the chain too.
  * A runtime helper that sets the flag itself
    (`rtlib.helpers.DECIMAL_MODE_HELPERS`, which never runs through
    the asm passes) passes with a `CLD` right before each `RTS`
    instead, so that it returns with the flag clear. A compiled
    function of the same name passes by either rule.
"""
from __future__ import annotations

import os

import asm2_ast
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
    # The names each instruction reads and writes, found once and read
    # by every check below. They're found again on each call, since a
    # pass may change an instruction in place (UCE rewrites a Phi's
    # arguments), so nothing about a function is kept between two
    # checks of it.
    uses = [[v.name for v in uses_in(instr)] for instr in instrs]
    defs = [[v.name for v in defs_in(instr)] for instr in instrs]
    if symbols is not None:
        names: set[str] = set()
        for u in uses:
            names.update(u)
        for d in defs:
            names.update(d)
        names.difference_update(fn.params)
        # A SymbolTable's own dict, which `get` reads.
        table = getattr(symbols, "_table", symbols)
        for n in sorted(n for n in names if table.get(n) is None):
            problems.append(f"{n} isn't in the symbol table")
    if problems:
        return problems
    if ssa_dsts is not None:
        problems += _ssa_problems(fn, ssa_dsts, uses, defs)
    else:
        problems += _temp_problems(fn, uses, defs)
    return problems


def _control_skeleton(instrs) -> tuple:
    """Everything `build_cfg` reads of `instrs`: their count, and the
    position and name of each label and the position, type, and target
    of each instruction that ends a block. Two functions with equal
    skeletons have the same blocks and edges."""
    from passes.optimization.cfg import _TERMINATOR_TYPES
    out: list = [len(instrs)]
    for k, instr in enumerate(instrs):
        if isinstance(instr, tac_ast.Label):
            out.append((k, None, instr.name))
        elif isinstance(instr, _TERMINATOR_TYPES):
            out.append((k, type(instr), getattr(instr, "target", None)))
    return tuple(out)


# The block structure of each control skeleton checked recently
# (`_cfg_shape`). An entry depends on the skeleton alone, so it can't
# go stale. It's cleared when it reaches `_SHAPES_KEPT` entries.
_SHAPES: dict[tuple, tuple] = {}
_SHAPES_KEPT = 512


def _cfg_shape(fn):
    """`(spans, idom, label_block)` for `fn`: each block of
    `build_cfg(fn)` in `block_order` as `(id, start, end)`, the slice
    of `fn.instructions` it holds, `immediate_dominators` of that CFG,
    and the block of each label. Most passes leave the labels and
    jumps alone, so the CFG and its dominators are computed once for
    each skeleton (`_control_skeleton`) and reused by later checks.
    The skeleton is taken afresh on every check, so a pass that
    changes a label or a jump in place gets a new entry."""
    from passes.optimization.cfg import build_cfg, immediate_dominators
    key = _control_skeleton(fn.instructions)
    shape = _SHAPES.get(key)
    if shape is not None:
        return shape
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    # `build_cfg` puts every instruction in one block, in order, so the
    # blocks of `block_order` one after another are `fn.instructions`.
    spans = []
    label_block: dict[str, int] = {}
    at = 0
    for bid in cfg.block_order:
        block = cfg.blocks[bid].instructions
        spans.append((bid, at, at + len(block)))
        at += len(block)
        for instr in block:
            if isinstance(instr, tac_ast.Label):
                label_block[instr.name] = bid
    assert at == len(fn.instructions)
    if len(_SHAPES) >= _SHAPES_KEPT:
        _SHAPES.clear()
    shape = _SHAPES[key] = (tuple(spans), idom, label_block)
    return shape


def _ssa_problems(fn, ssa_dsts, uses, defs) -> list[str]:
    """The SSA problems of `fn`, whose instructions read the names
    `uses[i]` and write `defs[i]`."""
    from passes.optimization.cfg import ENTRY_ID
    problems: list[str] = []
    instrs = fn.instructions
    spans, idom, label_block = _cfg_shape(fn)
    block_len = {bid: end - start for bid, start, end in spans}
    # Where each SSA name is defined: (block, index in block). A
    # parameter's first SSA value is defined at entry. Unreachable
    # blocks are skipped: renaming never visits them, so they keep
    # the original names, and UCE drops them.
    def_at: dict[str, tuple[int, int]] = {
        p: (ENTRY_ID, -1) for p in fn.params if p in ssa_dsts
    }
    for bid, start, end in spans:
        if bid not in idom:
            continue
        for i in range(start, end):
            for d in defs[i]:
                if d not in ssa_dsts:
                    continue
                if d in def_at:
                    problems.append(f"SSA name {d} defined twice")
                def_at[d] = (bid, i - start)

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

    for bid, start, end in spans:
        if bid not in idom:
            continue  # unreachable; UCE drops it
        for i in range(start, end):
            instr = instrs[i]
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
                        check_read(src.name, pred, block_len[pred], "Phi")
                continue
            for u in uses[i]:
                if u in ssa_dsts:
                    check_read(u, bid, i - start, type(instr).__name__)
    return problems


def _temp_problems(fn, uses, defs) -> list[str]:
    """The undefined temporaries `fn` reads, whose instructions read
    the names `uses[i]` and write `defs[i]`."""
    defined: set[str] = set(fn.params)
    for d in defs:
        defined.update(d)
    in_memory: set[str] = set()
    for instr in fn.instructions:
        match instr:
            case tac_ast.GetAddress(operand=tac_ast.Var(name=n)):
                in_memory.add(n)
            case tac_ast.IndexedLoad(name=n) | tac_ast.IndexedSymbolStore(name=n):
                in_memory.add(n)
    problems: list[str] = []
    for instr, u in zip(fn.instructions, uses):
        for name in u:
            if (
                name.startswith("%") and name not in defined
                and name not in in_memory
            ):
                problems.append(
                    f"{type(instr).__name__} reads {name}, which nothing "
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
        if isinstance(instr, asm_ast.JumpTable):
            for t in instr.targets:
                if t.startswith(".") and t not in labels:
                    problems.append(f"jump table to missing label {t}")
        if not phis and isinstance(instr, asm_ast.Phi):
            problems.append("Phi outside SSA form")
        if not pseudos and any(
            isinstance(op, asm_ast.Pseudo) for op in _all_operands_in(instr)
        ):
            problems.append(f"Pseudo left in {type(instr).__name__}")
        if isinstance(instr, asm_ast.DecimalArith):
            from passes.decimal_arith import form_problems
            problems += form_problems(instr)
        if isinstance(instr, (asm_ast.SetDecimal, asm_ast.ClearDecimal)):
            problems.append(
                f"{type(instr).__name__} outside a runtime helper"
            )
        if (isinstance(instr, _READ_MODIFY_WRITE)
                and isinstance(instr.dst, asm_ast.IndexedData)
                and not isinstance(instr.dst.index, asm_ast.X)):
            problems.append(
                f"{type(instr).__name__} of {instr.dst!r}, which has no "
                "6502 addressing mode (only abs,X)"
            )
    return problems


# The read-modify-write instructions, whose one indexed form is abs,X.
_READ_MODIFY_WRITE = (
    asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
)


# ---------------------------------------------------------------------------
# Asm2.
# ---------------------------------------------------------------------------


def check_asm2(prog: asm2_ast.Program, *, after: str) -> None:
    """Raises `IRCheckError` if the decimal flag is set anywhere but
    across a packed BCD chain (see the module docstring)."""
    if not _enabled:
        return
    from rtlib.helpers import DECIMAL_MODE_HELPERS
    for tl in prog.top_level:
        if isinstance(tl, asm2_ast.Function):
            problems = decimal_region_problems(tl.instructions)
            if problems and tl.name in DECIMAL_MODE_HELPERS:
                problems = _helper_decimal_problems(tl.instructions)
            if problems:
                raise IRCheckError(
                    f"after {after}, function '{tl.name}': "
                    + "; ".join(problems[:5])
                )


# Operand forms an LDA / ADC / SBC / STA of a BCD chain may name:
# immediates, absolute and zero-page bytes, and soft-stack slots. No
# register, index, or pointer, whose address would have to be set up
# inside the chain.
_CHAIN_SOURCES = (
    asm2_ast.Imm, asm2_ast.ImmLabelLow, asm2_ast.ImmLabelHigh,
    asm2_ast.Data, asm2_ast.ZP, asm2_ast.Frame, asm2_ast.Stack,
)
_CHAIN_DESTS = (asm2_ast.Data, asm2_ast.ZP, asm2_ast.Frame, asm2_ast.Stack)


def _is_a2(op) -> bool:
    return isinstance(op, asm2_ast.Reg) and isinstance(op.reg, asm2_ast.A)


def _helper_decimal_problems(instrs) -> list[str]:
    """What's wrong with the decimal flag in a runtime helper that sets
    it itself: an `RTS` without a `CLD` right before it, or a last
    instruction that isn't an `RTS`."""
    problems = [
        f"RTS at {i} without a CLD before it"
        for i, ins in enumerate(instrs)
        if isinstance(ins, asm2_ast.Return)
        and not (i > 0 and isinstance(instrs[i - 1], asm2_ast.ClearDecimal))
    ]
    if not instrs or not isinstance(instrs[-1], asm2_ast.Return):
        problems.append("the helper doesn't end with an RTS")
    return problems


def decimal_region_problems(instrs) -> list[str]:
    """What's wrong with the decimal-mode regions of a function's asm2
    instructions. Public so that a test can feed it a broken region."""
    problems: list[str] = []
    i, n = 0, len(instrs)
    while i < n:
        ins = instrs[i]
        if isinstance(ins, asm2_ast.ClearDecimal):
            problems.append(f"CLD at {i} outside a BCD chain")
        if not isinstance(ins, asm2_ast.SetDecimal):
            i += 1
            continue
        start = i
        i += 1
        if i < n and isinstance(instrs[i], asm2_ast.ClearCarry):
            arith = asm2_ast.Add
        elif i < n and isinstance(instrs[i], asm2_ast.SetCarry):
            arith = asm2_ast.Sub
        else:
            problems.append(f"SED at {start} isn't followed by CLC or SEC")
            continue
        i += 1
        count = 0
        while i + 2 < n:
            load, op, store = instrs[i], instrs[i + 1], instrs[i + 2]
            if not (isinstance(load, asm2_ast.Mov) and _is_a2(load.dst)
                    and isinstance(load.src, _CHAIN_SOURCES)):
                break
            if not (type(op) is arith and _is_a2(op.dst)
                    and isinstance(op.src, _CHAIN_SOURCES)):
                break
            if not (isinstance(store, asm2_ast.Mov) and _is_a2(store.src)
                    and isinstance(store.dst, _CHAIN_DESTS)):
                break
            count += 1
            i += 3
        if count == 0:
            problems.append(f"the BCD chain at {start} has no byte")
        if i >= n or not isinstance(instrs[i], asm2_ast.ClearDecimal):
            what = type(instrs[i]).__name__ if i < n else "the end"
            problems.append(
                f"{what} at {i} runs with the decimal flag set (SED at "
                f"{start})"
            )
            continue
        i += 1
    return problems
