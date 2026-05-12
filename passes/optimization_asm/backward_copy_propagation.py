"""Asm-level backward copy propagation.

Forward copy propagation (`copy_propagation.py`) substitutes uses of
a copy's `dst` with the copy's `src` — propagating along the
def → use direction. This pass does the dual: when a Pseudo `P`'s
single use is to feed its value into a final memory destination
`D`, rewrite `P`'s def to write `D` directly and drop the round-
trip.

The asm-level twist. `tac_to_asm` routes every Pseudo↔Memory
transfer through `Reg(A)`, so a "copy from `P` to `D`" actually
shows up as the consecutive pair

    Mov(Pseudo P, Reg(A))      # load P into A
    Mov(Reg(A), D)             # store A into D

This pass treats that pair as a logical `Copy(P, D)`. When `P` has
exactly one such use and its def is `Mov(Reg(A), P)`, the pair can
be eliminated and the def's destination redirected to `D`.

Concretely, rewrites the pattern

    Mov(Reg(A), Pseudo P)        # def of P     (instr `def_idx`)
    ... region R ...
    Mov(Pseudo P, Reg(A))        # last use of P  (instr `i`)
    Mov(Reg(A), D)               # immediately following  (instr `i+1`)

into

    Mov(Reg(A), D)               # relocated def
    ... region R ...
    [pair deleted]

Preconditions (all checked):

  * `P` has exactly one USE in the function (the pair's first
    instr).
  * `P` is not in `excluded` (statics, address-taken, RMW
    targets) — same set the SSA construction excludes.
  * `def_idx` is the unique `Mov(Reg(A), P)` upstream of `i`.
  * `def_idx`, `i`, `i+1` lie in the same straight-line basic
    block — no `Label` / `Jump` / `Branch` / `Ret` / `Return`
    between `def_idx + 1` and `i`.
  * `D`'s storage isn't read or written in region R — every
    operand of every intermediate instruction is checked for
    aliasing against `D`.
  * No `Call` in region R — Calls clobber HARGS (the runtime
    helpers' argument-exchange block) and we can't statically
    bound their effects.
  * `Reg(A)`'s value is dead immediately after the pair is
    deleted: walking forward from the (former) `i+2` position,
    the next instruction that touches `A` writes it without
    reading first (or we hit a `Ret(save_a=False)` /
    `Return(save_a=False)` / fall off the function).
  * The N/Z flags' values immediately after the pair are dead.
    The deleted `Mov(P, Reg(A))` is an `LDA` that sets N/Z;
    deletion would silently change a downstream `Branch` if its
    flags remained live without an intervening flag-setter.

`D` is restricted to non-`Pseudo` memory operands (`Data` /
`Stack`). Merging two Pseudos is the regalloc / coalescing
problem, not this pass's job.

The pass iterates to a fixed point: a successful rewrite can
expose another. Statics are excluded both ways: a static-named
`P` would be externally observable so its single-use invariant
doesn't hold; a static-named `D` is fine as a relocation target —
the no-Call rule covers external visibility.
"""
from __future__ import annotations

from typing import Iterable

import asm_ast
from passes.asm_liveness import (
    a_dead_at as _a_dead_at,
    flags_dead_at as _flags_dead_at,
    is_reg_a as _is_reg_a,
    kills_a as _kills_a,
    reads_a as _reads_a,
    sets_flags as _sets_flags,
)


ByteVar = tuple[str, int]


def backward_copy_propagate(
    fn: asm_ast.Function, *,
    statics: frozenset[str] = frozenset(),
) -> asm_ast.Function:
    """Run backward copy propagation to a fixed point. Returns the
    rewritten function. See module docstring for the rewrite rule
    and preconditions."""
    while True:
        prev = fn
        fn = _one_pass(fn, statics)
        if fn.instructions == prev.instructions:
            return fn


# ---------------------------------------------------------------------------
# Driver.
# ---------------------------------------------------------------------------


def _one_pass(
    fn: asm_ast.Function, statics: frozenset[str],
) -> asm_ast.Function:
    excluded = _excluded_names(fn) | statics
    use_counts = _compute_use_counts(fn)
    instrs = fn.instructions

    rewrites: list[tuple[int, int, asm_ast.Type_operand]] = []
    claimed: set[int] = set()
    for i in range(len(instrs) - 1):
        if i in claimed or (i + 1) in claimed:
            continue
        match = _try_match_at(instrs, i, excluded, use_counts)
        if match is None:
            continue
        def_idx, new_dst = match
        if def_idx in claimed:
            continue
        rewrites.append((def_idx, i, new_dst))
        claimed.update({def_idx, i, i + 1})

    if not rewrites:
        return fn

    delete: set[int] = set()
    rewrite_def: dict[int, asm_ast.Type_operand] = {}
    for def_idx, pair_idx, new_dst in rewrites:
        rewrite_def[def_idx] = new_dst
        delete.update({pair_idx, pair_idx + 1})

    new_instrs: list[asm_ast.Type_instruction] = []
    for idx, instr in enumerate(instrs):
        if idx in delete:
            continue
        if idx in rewrite_def:
            new_instrs.append(_with_mov_dst(instr, rewrite_def[idx]))
        else:
            new_instrs.append(instr)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _try_match_at(
    instrs: list[asm_ast.Type_instruction], i: int,
    excluded: set[str],
    use_counts: dict[ByteVar, int],
) -> tuple[int, asm_ast.Type_operand] | None:
    """Test whether `instrs[i:i+2]` is a virtual-copy pair `P → D`
    that can be safely collapsed. Returns `(def_idx, D)` on
    success."""
    first = instrs[i]
    if not isinstance(first, asm_ast.Mov):
        return None
    if not isinstance(first.src, asm_ast.Pseudo):
        return None
    if not _is_reg_a(first.dst):
        return None
    second = instrs[i + 1]
    if not isinstance(second, asm_ast.Mov):
        return None
    if not _is_reg_a(second.src):
        return None
    if not _is_memory_dst(second.dst):
        return None

    P = first.src
    D = second.dst
    if P.name in excluded:
        return None
    if use_counts.get((P.name, P.offset), 0) != 1:
        return None

    def_idx = _find_canonical_def(instrs, P, upper_bound=i)
    if def_idx is None:
        return None

    if not _safe_relocation_range(instrs, def_idx + 1, i, D):
        return None
    if not _a_dead_at(instrs, i + 2):
        return None
    if not _flags_dead_at(instrs, i + 2):
        return None

    return def_idx, D


# ---------------------------------------------------------------------------
# Def discovery + safety analyses.
# ---------------------------------------------------------------------------


def _find_canonical_def(
    instrs: list[asm_ast.Type_instruction],
    P: asm_ast.Pseudo, *, upper_bound: int,
) -> int | None:
    """Return the unique index of `Mov(Reg(A), P)` in
    `instrs[:upper_bound]`, or None. Multiple matches → None
    (defensive — SSA shouldn't admit multiple defs)."""
    found: int | None = None
    for idx in range(upper_bound):
        instr = instrs[idx]
        if not isinstance(instr, asm_ast.Mov):
            continue
        if not isinstance(instr.dst, asm_ast.Pseudo):
            continue
        if instr.dst.name != P.name or instr.dst.offset != P.offset:
            continue
        if not _is_reg_a(instr.src):
            return None
        if found is not None:
            return None
        found = idx
    return found


def _safe_relocation_range(
    instrs: list[asm_ast.Type_instruction],
    lo: int, hi: int,
    D: asm_ast.Type_operand,
) -> bool:
    """True iff every instruction in `instrs[lo:hi]` is safe given
    that `D`'s storage write will be relocated to position
    `lo - 1`. Bails on:

      * Any control-flow boundary (`Label` / `Jump` / `Branch` /
        `Ret` / `Return`) — the def and use must lie in one
        straight-line basic block for the linear walk to faithfully
        cover what executes between them.
      * Any `Call` — Calls may clobber `D` (HARGS, statics) and
        their full memory-write set is unknown.
      * Any operand of any intermediate instruction that aliases
        `D`'s storage cell."""
    for idx in range(lo, hi):
        instr = instrs[idx]
        if isinstance(instr, (
            asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
            asm_ast.Ret, asm_ast.Return, asm_ast.Call,
        )):
            return False
        for op in _all_operands(instr):
            if _operands_alias(op, D):
                return False
    return True


def _operands_alias(
    a: asm_ast.Type_operand, b: asm_ast.Type_operand,
) -> bool:
    """True iff `a` and `b` name the same memory cell."""
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.Stack) and isinstance(b, asm_ast.Stack):
        return a.offset == b.offset
    return False


def _is_memory_dst(op: asm_ast.Type_operand) -> bool:
    """Allowed final-destination shapes — non-Pseudo memory cells.
    `Data` and `Stack` are what `tac_to_asm` emits at this point in
    the pipeline; `Frame` / `ZP` are introduced by later passes
    (`replace_pseudoregisters`) and shouldn't appear here."""
    return isinstance(op, (asm_ast.Data, asm_ast.Stack))




def _with_mov_dst(
    instr: asm_ast.Type_instruction,
    new_dst: asm_ast.Type_operand,
) -> asm_ast.Type_instruction:
    """Return a `Mov` with the dst replaced. Only `Mov` is
    supported (the canonical def we relocate is always
    `Mov(Reg(A), P)`)."""
    if isinstance(instr, asm_ast.Mov):
        return asm_ast.Mov(src=instr.src, dst=new_dst)
    raise AssertionError(
        f"_with_mov_dst: unsupported {type(instr).__name__}",
    )


# ---------------------------------------------------------------------------
# Use counting + operand walks.
# ---------------------------------------------------------------------------


def _compute_use_counts(
    fn: asm_ast.Function,
) -> dict[ByteVar, int]:
    """Count Pseudo USES per `(name, offset)`. Defs don't count —
    the goal is to find single-use Pseudos."""
    out: dict[ByteVar, int] = {}
    for instr in fn.instructions:
        for op in _use_operands(instr):
            if isinstance(op, asm_ast.Pseudo):
                key = (op.name, op.offset)
                out[key] = out.get(key, 0) + 1
    return out


def _use_operands(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Type_operand]:
    """Yield every USE operand. RMW targets count. Mirrors
    `byte_dce._use_operands`."""
    match instr:
        case asm_ast.Mov(src=src):
            yield src
        case asm_ast.Add(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Sub(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.And(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Or(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Xor(src1=s1, src2=s2):
            yield s1
            yield s2
        case asm_ast.Inc(dst=dst):
            yield dst
        case asm_ast.Dec(dst=dst):
            yield dst
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            yield dst
        case asm_ast.LogicalShiftRight(dst=dst):
            yield dst
        case asm_ast.RotateLeft(dst=dst):
            yield dst
        case asm_ast.RotateRight(dst=dst):
            yield dst
        case asm_ast.Push(src=src):
            yield src
        case asm_ast.Compare(left=left, right=right):
            yield left
            yield right
        case asm_ast.LoadAddress(src=src):
            yield src
        case asm_ast.Phi(args=args):
            for a in args:
                yield a.source


def _all_operands(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Type_operand]:
    """Every operand-typed field of `instr` — uses AND defs.
    Mirrors `ssa_construction._operand_fields`."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Add(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Sub(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.And(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Or(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            yield s1
            yield s2
            yield dst
        case asm_ast.Inc(dst=dst):
            yield dst
        case asm_ast.Dec(dst=dst):
            yield dst
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            yield dst
        case asm_ast.LogicalShiftRight(dst=dst):
            yield dst
        case asm_ast.RotateLeft(dst=dst):
            yield dst
        case asm_ast.RotateRight(dst=dst):
            yield dst
        case asm_ast.Push(src=src):
            yield src
        case asm_ast.Pop(dst=dst):
            yield dst
        case asm_ast.Compare(left=left, right=right):
            yield left
            yield right
        case asm_ast.LoadAddress(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Phi(dst=dst, args=args):
            yield dst
            for a in args:
                yield a.source


def _excluded_names(fn: asm_ast.Function) -> set[str]:
    """Pseudo names that aren't in SSA form: address-taken
    (`LoadAddress.src`), 2-byte address holders (`LoadAddress.dst`),
    and read-modify-write targets (`Inc / Dec / ASL / LSR / ROL /
    ROR.dst`). Same set used by `ssa_construction` and
    `copy_propagation`."""
    excluded: set[str] = set()
    for instr in fn.instructions:
        match instr:
            case asm_ast.LoadAddress(src=src, dst=dst):
                if isinstance(src, asm_ast.Pseudo):
                    excluded.add(src.name)
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
            case (
                asm_ast.Inc(dst=dst)
                | asm_ast.Dec(dst=dst)
                | asm_ast.ArithmeticShiftLeft(dst=dst)
                | asm_ast.LogicalShiftRight(dst=dst)
                | asm_ast.RotateLeft(dst=dst)
                | asm_ast.RotateRight(dst=dst)
            ):
                if isinstance(dst, asm_ast.Pseudo):
                    excluded.add(dst.name)
    return excluded
