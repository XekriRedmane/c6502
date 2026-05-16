"""Asm-level SSA-aware copy propagation.

Mirrors `passes.optimization.copy_propagation` (TAC), but operates
on the asm IR with byte-granular `(name, offset)` keys and the
asm-level instruction shapes.

A copy candidate is `Mov(src, Pseudo dst)` where:

  * `dst` is a Pseudo whose name has been versioned by `to_ssa`
    (or is a parameter byte-var that's still in the SSA-promotable
    set). Equivalently: `dst.name` is not in `statics` and not in
    the function's address-taken / RMW-target name set. Those
    Pseudos have a single SSA def, so `dst ≡ src` holds at every
    use of `dst`.
  * `src` is one of:
      - `Imm` / `ImmLabelLow` / `ImmLabelHigh` — immutable (literal
        values / link-time addresses).
      - `Pseudo` whose name is also SSA-safe (same statics + excluded
        check). SSA guarantees the source's value is stable between
        the def of `dst` and any use.
    Every other source kind is rejected: `Reg`, `Stack`, `Frame`,
    `Data`, `ZP`, `Indirect` all alias mutable cells that other
    instructions can write between the def and the use, so
    propagating their value through to later uses would observe
    stale values.

Renamed asm-SSA Pseudos always carry `offset=0` (the byte position
is baked into the name as `<orig>.b<k>.vN`), but the unrenamed
parameter / static / address-taken Pseudos still carry their
original byte offsets. Both cases are keyed by `(name, offset)`
uniformly — for renamed names that's `(name, 0)`, for unrenamed
ones it's `(name, k)`.

The pass is deliberately conservative — it only handles direct
`Mov(src, dst)` copies. It does NOT recognize the two-step pattern
`Mov src→A; Mov A→Pseudo` as a copy, because tracking `Reg(A)` flow
across intervening clobbers is outside this pass's scope. (Today's
`tac_to_asm` always routes Pseudo writes through `Reg(A)`, so direct
`Mov(Pseudo, Pseudo)` Movs are rare on input — but they appear after
SSA destruction lowers Phis to per-edge Movs, and the pass remains
sound infrastructure for future passes that emit them earlier.)

Phis are NOT treated as copies (a Phi merges multiple values, not
one); their dsts don't enter the propagation map.

Algorithm — same shape as the TAC pass:
  1. Walk every `Mov`. For each copy candidate, record
     `copy_src[(dst.name, dst.offset)] = src`.
  2. Resolve chains: follow each entry whose `src` is itself a
     Pseudo that's another copy's dst, until we reach a base value
     (`Imm` / `ImmLabelLow` / `ImmLabelHigh` / a Pseudo not in the
     map). SSA guarantees no cycles.
  3. Walk every instruction again. For each Pseudo USE whose
     `(name, offset)` is in the map, substitute the resolved value.
     `Phi.args[k].source` is rewritten the same way. DEFs are left
     alone (they ARE the SSA identity we're propagating from).

After propagation, the original `Mov(src, dst)` becomes a dead
store (its dst no longer has any reads); `byte_dce` picks it up on
the next fixed-point round.

Requires asm-SSA form to be sound: in non-SSA asm, a Pseudo dst
can be re-defined by a later `Mov`, breaking the `dst ≡ src`
invariant. The optimizer driver sequences `to_ssa` → copy_prop →
byte_dce → ... → from_ssa, so the input is guaranteed SSA.
"""
from __future__ import annotations

from typing import Iterable

import asm_ast


# A byte-level Pseudo identity is `(name, offset)` — same key as
# the rest of the asm-SSA layer.
ByteVar = tuple[str, int]


def copy_propagate(
    fn: asm_ast.Function, *,
    statics: frozenset[str] = frozenset(),
) -> asm_ast.Function:
    """Substitute every use of a copy's dst with its (chain-resolved)
    src. Returns the rewritten function.

    `statics` is the set of static-storage Pseudo names visible at
    the program top level — same set `byte_dce` and `to_ssa` use.
    Writes to a static are externally observable (other functions
    in the program can read them) and reads can be invalidated by
    intervening function calls, so static-named Pseudos are
    excluded from both copy dsts and copy srcs."""
    excluded = _excluded_names(fn) | statics
    copy_src = _collect_copy_sources(fn, excluded)
    if not copy_src:
        return fn
    resolved = _resolve_chains(copy_src)
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=[_rewrite(i, resolved) for i in fn.instructions],
    )


def _collect_copy_sources(
    fn: asm_ast.Function, excluded: set[str],
) -> dict[ByteVar, asm_ast.Type_operand]:
    """Build the `copy_src[(dst_name, dst_offset)] = src_val` map.
    A `Mov(src, dst)` contributes only when:

      * `dst` is a Pseudo whose name is NOT in `excluded` (statics,
        address-taken, RMW-target).
      * `src` is an `Imm` / `ImmLabelLow` / `ImmLabelHigh`, OR a
        Pseudo whose name is also not in `excluded`.

    Other src kinds (`Reg`, `Stack`, `Frame`, `Data`, `ZP`,
    `Indirect`) alias mutable cells that can be rewritten between
    the def of `dst` and any later use; propagating their
    instantaneous value would observe stale data."""
    out: dict[ByteVar, asm_ast.Type_operand] = {}
    for instr in fn.instructions:
        if not isinstance(instr, asm_ast.Mov):
            continue
        if not isinstance(instr.dst, asm_ast.Pseudo):
            continue
        if instr.dst.name in excluded:
            continue
        src = instr.src
        if isinstance(src, (asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)):
            out[(instr.dst.name, instr.dst.offset)] = src
            continue
        if isinstance(src, asm_ast.Pseudo) and src.name not in excluded:
            out[(instr.dst.name, instr.dst.offset)] = src
            continue
    return out


def _resolve_chains(
    copy_src: dict[ByteVar, asm_ast.Type_operand],
) -> dict[ByteVar, asm_ast.Type_operand]:
    """Follow each entry whose `src` is a Pseudo that's another
    copy's dst, until we reach a base value (Imm / ImmLabelLow /
    ImmLabelHigh / a Pseudo not in the map). SSA guarantees no
    cycles."""
    resolved: dict[ByteVar, asm_ast.Type_operand] = {}
    for key in copy_src:
        seen: set[ByteVar] = set()
        cur: asm_ast.Type_operand = copy_src[key]
        while isinstance(cur, asm_ast.Pseudo):
            cur_key = (cur.name, cur.offset)
            if cur_key not in copy_src:
                break
            if cur_key in seen:
                # Defensive — SSA shouldn't admit cycles; bail at
                # the cycle rather than spin if a malformed input
                # slips through.
                break
            seen.add(cur_key)
            cur = copy_src[cur_key]
        resolved[key] = cur
    return resolved


def _rewrite(
    instr: asm_ast.Type_instruction,
    resolved: dict[ByteVar, asm_ast.Type_operand],
) -> asm_ast.Type_instruction:
    """Return `instr` with every Pseudo USE rewritten to its
    resolved base value. DEFs stay as-is — they ARE the SSA
    identity we're propagating from. `LoadAddress.src` is treated
    as a non-value operand (the Pseudo names a storage cell, not a
    value) and is never substituted."""

    def sub_use(op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        if isinstance(op, asm_ast.Pseudo):
            key = (op.name, op.offset)
            if key in resolved:
                return resolved[key]
        return op

    match instr:
        case asm_ast.Mov(src=src, dst=dst, is_volatile=v):
            return asm_ast.Mov(src=sub_use(src), dst=dst, is_volatile=v)
        case asm_ast.Add(src=src, dst=dst):
            # `dst` is read-modify-write (it's the accumulator A in
            # current emissions, and A isn't a Pseudo, so sub_use
            # here is a no-op for Reg dsts; defensive for the
            # unrestricted IR shape).
            return asm_ast.Add(src=sub_use(src), dst=sub_use(dst))
        case asm_ast.Sub(src=src, dst=dst):
            return asm_ast.Sub(src=sub_use(src), dst=sub_use(dst))
        case asm_ast.And(src=src, dst=dst):
            return asm_ast.And(src=sub_use(src), dst=sub_use(dst))
        case asm_ast.Or(src=src, dst=dst):
            return asm_ast.Or(src=sub_use(src), dst=sub_use(dst))
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            return asm_ast.Xor(src1=sub_use(s1), src2=sub_use(s2), dst=dst)
        case asm_ast.Push(src=src):
            return asm_ast.Push(src=sub_use(src))
        case asm_ast.Compare(left=left, right=right):
            return asm_ast.Compare(
                left=sub_use(left), right=sub_use(right),
            )
        case asm_ast.Inc(dst=dst):
            return asm_ast.Inc(dst=sub_use(dst))
        case asm_ast.Dec(dst=dst):
            return asm_ast.Dec(dst=sub_use(dst))
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            return asm_ast.ArithmeticShiftLeft(dst=sub_use(dst))
        case asm_ast.LogicalShiftRight(dst=dst):
            return asm_ast.LogicalShiftRight(dst=sub_use(dst))
        case asm_ast.RotateLeft(dst=dst):
            return asm_ast.RotateLeft(dst=sub_use(dst))
        case asm_ast.RotateRight(dst=dst):
            return asm_ast.RotateRight(dst=sub_use(dst))
        case asm_ast.LoadAddress(src=src, dst=dst):
            # `src` names a storage cell, not a value — propagating
            # it to a literal would lose the address. Leave it.
            return instr
        case asm_ast.Phi(dst=dst, args=args):
            return asm_ast.Phi(
                dst=dst,
                args=[
                    asm_ast.AsmPhiArg(
                        pred_label=a.pred_label,
                        source=sub_use(a.source),
                    )
                    for a in args
                ],
            )
        # Pop / Call / Jump / Branch / Label / Return / Ret /
        # ClearCarry / SetCarry / AllocateStack / FunctionPrologue
        # — no Pseudo USES (only DEFs or no operands at all).
    return instr


# ---------------------------------------------------------------------------
# Excluded names. Mirrors `ssa_construction._excluded_names` — kept here so
# the pass is self-contained, same way `byte_dce` keeps its own use-walker.
# ---------------------------------------------------------------------------


def _excluded_names(fn: asm_ast.Function) -> set[str]:
    """Pseudo names that asm-SSA construction skipped: address-taken
    (`LoadAddress.src`), 2-byte address holders (`LoadAddress.dst`),
    and read-modify-write targets (`Inc / Dec / ASL / LSR / ROL /
    ROR.dst`). Their values can change without an SSA-versioned def,
    so they're unsafe both as copy dsts AND as copy srcs."""
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
