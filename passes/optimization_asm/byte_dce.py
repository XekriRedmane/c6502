"""Byte-granular dead-store elimination, asm-level SSA edition.

Drops `Mov` and `Phi` instructions whose Pseudo dst doesn't appear
as a use anywhere in the function. Runs to a fixed point inside
the asm-SSA round-trip — dropping one Mov can make its src's def
unused next round.

Why asm-level rather than TAC-level. TAC's DSE is already pretty
good for whole-value dead writes (a 4-byte Long that's defined
but never read). What it CAN'T see: byte-level dead writes. After
asm-SSA versioning, byte 0 of a value and byte 3 of the same
value are independent variables; if a function uses only the low
byte, byte 3's def is independently dead and droppable. Common
example: a `(long)y` cast emits 4 byte-stores, but if `y` is
later used only as `(int)y` at the same width, the high two bytes
are dead.

Which instructions are eligible. For step 6 the pass is
conservative: only `Mov(src, Pseudo)` and `Phi(Pseudo, ...)` are
ever dropped. Other definers stay:

  * `LoadAddress(src, Pseudo)` — unused dst is droppable in
    principle, but `src` is address-taken, and dropping the
    LoadAddress can leave `src` without any operand-discovery
    site, which `replace_pseudoregisters` uses to allocate its
    frame slot. Defer to a later pass if needed.
  * `Pop(Pseudo)` — has a stack side effect. Dropping needs a
    matching `AllocateStack` adjustment. Defer.
  * `Add(src, Reg(A))` / `Sub(...)` / `And/Or/Xor` — their dst is
    a Reg, not a Pseudo, so they're not in scope for this pass
    even if A's flow is dead. (Reg-level DCE would need flag and
    register liveness, which today's IR doesn't track.)
  * `Inc/Dec/ASL/LSR/ROL/ROR(Pseudo)` — read-modify-write target,
    excluded from SSA promotion upstream so its name has no SSA
    versioning. Conservatively skip.

A Mov with Reg(A) dst (the common "load into A" pattern) is also
skipped — the Reg(A) is never in `use_set`, but its flow value is
implicitly used by the next instruction. This pass treats Reg
operands as opaque (always live).
"""
from __future__ import annotations

from typing import Iterable

import asm_ast


def byte_dce(
    fn: asm_ast.Function, *,
    statics: frozenset[str] = frozenset(),
) -> asm_ast.Function:
    """Run byte-granular DCE to a fixed point. The function must be
    in asm-SSA form (post-`to_ssa`, pre-`from_ssa`).

    `statics` is the set of static-storage Pseudo names (file-scope
    globals, block-scope statics, externs). Writes to these names
    are externally observable — other functions in the program can
    read them — so they're never considered dead even if the local
    function never reads them. Same set the SSA construction pass
    uses to exclude statics from versioning."""
    excluded = _excluded_names(fn)
    while True:
        prev = fn
        fn = _one_pass(fn, statics, excluded)
        if fn.instructions == prev.instructions:
            return fn


def _one_pass(
    fn: asm_ast.Function, statics: frozenset[str], excluded: frozenset[str],
) -> asm_ast.Function:
    use_set = _collect_pseudo_uses(fn)
    new_instrs: list[asm_ast.Type_instruction] = []
    for instr in fn.instructions:
        if _is_dead(instr, use_set, statics, excluded):
            continue
        new_instrs.append(instr)
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _is_dead(
    instr: asm_ast.Type_instruction,
    use_set: set[tuple[str, int]],
    statics: frozenset[str],
    excluded: frozenset[str],
) -> bool:
    """True iff dropping `instr` is safe (its dst Pseudo is unused
    AND not a static-storage name AND not on an address-taken
    name). Only Mov / Phi are considered today — see the module
    docstring for the rationale.

    The `excluded` gate matters for address-taken Pseudos: a
    `LoadAddress(P, _)` makes EVERY byte of P observable via
    indirect-Y reads through the produced pointer. `_use_operands`
    only counts the addressed byte (offset 0) as a use, so without
    this gate the high bytes of `P` look unused and a multi-byte
    init like `int i = -100;` would have its high-byte store
    silently dropped."""
    if isinstance(instr, asm_ast.Mov):
        if not isinstance(instr.dst, asm_ast.Pseudo):
            return False
        if instr.dst.name in statics:
            return False
        if instr.dst.name in excluded:
            return False
        return (instr.dst.name, instr.dst.offset) not in use_set
    if isinstance(instr, asm_ast.Phi):
        if not isinstance(instr.dst, asm_ast.Pseudo):
            return False
        if instr.dst.name in statics:
            return False
        if instr.dst.name in excluded:
            return False
        return (instr.dst.name, instr.dst.offset) not in use_set
    return False


def _excluded_names(fn: asm_ast.Function) -> frozenset[str]:
    """Pseudo names that asm-level SSA construction excludes from
    byte-granular versioning — address-taken (via LoadAddress) and
    read-modify-write targets. Mirror of
    `passes.optimization_asm.ssa_construction._excluded_names`.
    Kept in sync defensively: a name not promoted to SSA can't be
    safely DCE'd at the byte level either."""
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
    return frozenset(excluded)


def _collect_pseudo_uses(
    fn: asm_ast.Function,
) -> set[tuple[str, int]]:
    """Set of (name, offset) pairs that appear as a Pseudo USE
    somewhere in the function. Defs DON'T count — that's the whole
    point of DCE."""
    out: set[tuple[str, int]] = set()
    for instr in fn.instructions:
        for op in _use_operands(instr):
            if isinstance(op, asm_ast.Pseudo):
                out.add((op.name, op.offset))
    return out


def _use_operands(
    instr: asm_ast.Type_instruction,
) -> Iterable[asm_ast.Type_operand]:
    """Yield every operand-typed USE of `instr`. Defs are
    excluded. Mirrors `passes.optimization_asm.ssa_construction.
    _uses_in` — same shape, kept here so the DCE pass is self-
    contained."""
    match instr:
        case asm_ast.Mov(src=src):
            yield src
        case asm_ast.Add(src=src, dst=dst):
            yield src
            # `dst` is also a use (read-modify-write semantics for
            # ADC's accumulator). Pseudo dsts here are
            # theoretically possible but tac_to_asm always emits
            # Reg(A); yielded for completeness.
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
            # `src` is the addressed Pseudo; its bytes still need
            # to live somewhere in the frame, so count it as a use
            # at every byte to keep `replace_pseudoregisters` from
            # losing track. (We don't know the byte width here;
            # yield offset 0 — the discover step handles the rest
            # via the operand-walk in `_operands_in`.)
            yield src
        case asm_ast.Phi(args=args):
            for a in args:
                yield a.source
