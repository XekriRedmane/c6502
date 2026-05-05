"""Constant reassociation across nested Add chains.

Recognize the pattern

    Binary(Add, C2, V, %inner)
    Binary(Add, C1, %inner, %outer)

(or any commutative variant — Add doesn't care which side the
Constant is on) and rewrite to

    Binary(Add, (C1 + C2), V, %outer)

dropping the now-dead inner def. The two Constants combine at
compile time; the runtime work is one Add instead of two.

The motivating case: pointer arithmetic on a static-storage array
with a constant base offset. After the const-static fold and
const-array-subscript fold,

    hires_page1[interlace_p1_offsets[2] + col] = value;

lowers to

    Binary(Add, Constant(0x01D0), col_extended, %sum)
    Binary(Add, Constant(0x2000), %sum, %addr)
    Store(value, %addr)

The two Constants fold across the Add chain into a single 0x21D0,
turning the address computation into a single 16-bit Add of
`Constant(0x21D0) + col` — half the runtime work, and the
shape an IndexedStore lowering can match for absolute,X stores.

# Eligibility

The fusion fires when:

  * The outer instruction is `Binary(Add, ..., dst=%outer)` with
    one Constant operand and one Var operand.
  * That Var (`%inner`) names a Pseudo whose def in the function
    is `Binary(Add, ..., dst=%inner)` with one Constant operand
    and one (any) operand.
  * `%inner` has exactly one use across the function (the outer
    Binary). Without this, rewriting the outer would replicate
    work the inner used to share with other consumers.
  * The two Constants have the same const variant (matching width
    and signedness) — the combined Constant takes that variant,
    and Add wraps modulo the variant's bit width.

# Width / wraparound

The two Constants combine modulo 2^bits where `bits` is the
variant's bit width. This matches what the runtime would compute
for two-byte (Pointer / UInt) or four-byte (ULong) arithmetic on
the 6502 (per-byte ADC chain wraps naturally). For signed
overflow this also matches — c6502 doesn't model signed-overflow
UB; the lowering is just two's-complement arithmetic.

# Sub variants

Subtract is also handled, with the obvious sign rules:

    Binary(Sub, C, V, %inner); Binary(Sub, K, %inner, %outer)
        →  Binary(Sub, (K - C), V, %outer) on `(K - C) - V`?
    No — `(K) - (C - V)` = `K - C + V` ≠ `(K - C) - V`. Skip.

So Sub at the outer position requires careful handling. For now
this pass only fuses `Add ... Add` chains. Mixed Add/Sub is
deferred (it'd require tracking signs through the chain — the
canonical algorithm is value-numbering or polynomial
representation).

# Scope

Single-pair, single-iteration. Nested chains of three or more
Adds get reduced one pair per fixed-point iteration of the TAC
optimizer, which is enough for the common `hires_page1 +
arr[K] + col` shape.
"""

from __future__ import annotations

from collections import Counter

import tac_ast
from passes.optimization.var_visit import uses_in


def reassoc_constants(fn: tac_ast.Function) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each outer Add(Const, %inner)
    where the inner is a single-use Pseudo defined by another
    Add(Const, ...), combine the two Constants. Returns a new
    Function; doesn't mutate the input.

    Two passes: pass 1 scans for rewrites and records which inner-
    Binary indices to drop and which outer-Binary indices to
    replace; pass 2 rebuilds the instruction list. The split is
    needed because the inner def precedes the outer in source
    order — a single-pass loop would emit the inner before
    realizing the outer subsumes it."""
    use_counts = _count_uses(fn.instructions)
    inner_def = _build_inner_def_index(fn.instructions)
    # Pass 1: record rewrites.
    rewrites: dict[int, tac_ast.Type_instruction] = {}
    dropped_def_indices: set[int] = set()
    for i, instr in enumerate(fn.instructions):
        rewritten = _try_reassoc(
            instr, fn.instructions, inner_def, use_counts,
            dropped_def_indices,
        )
        if rewritten is not instr:
            rewrites[i] = rewritten
    # Pass 2: rebuild.
    new_instrs: list[tac_ast.Type_instruction] = []
    for i, instr in enumerate(fn.instructions):
        if i in dropped_def_indices:
            continue
        new_instrs.append(rewrites.get(i, instr))
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )


def _count_uses(
    instrs: list[tac_ast.Type_instruction],
) -> Counter[str]:
    counts: Counter[str] = Counter()
    for instr in instrs:
        for v in uses_in(instr):
            counts[v.name] += 1
    return counts


def _build_inner_def_index(
    instrs: list[tac_ast.Type_instruction],
) -> dict[str, int]:
    """Map each Var name to the index of its (single) defining
    instruction in `instrs`. Only Binary(Add)-shape defs are
    recorded — those are the only candidates for reassoc fusion."""
    out: dict[str, int] = {}
    for i, instr in enumerate(instrs):
        if (
            isinstance(instr, tac_ast.Binary)
            and isinstance(instr.op, tac_ast.Add)
            and isinstance(instr.dst, tac_ast.Var)
        ):
            out[instr.dst.name] = i
    return out


def _try_reassoc(
    instr: tac_ast.Type_instruction,
    all_instrs: list[tac_ast.Type_instruction],
    inner_def: dict[str, int],
    use_counts: Counter[str],
    dropped_def_indices: set[int],
) -> tac_ast.Type_instruction:
    """If `instr` is an outer Add(Const, %inner) (or Add(%inner,
    Const)) where `%inner`'s single-use def is itself an Add with
    a Constant operand of the same width, return the combined
    Add. The inner def's index is added to `dropped_def_indices`
    so the caller skips emitting it. Otherwise return `instr`
    unchanged."""
    if not (
        isinstance(instr, tac_ast.Binary)
        and isinstance(instr.op, tac_ast.Add)
    ):
        return instr
    outer_const, outer_var = _split_const_var(instr.src1, instr.src2)
    if outer_const is None or outer_var is None:
        return instr
    inner_idx = inner_def.get(outer_var.name)
    if inner_idx is None:
        return instr
    if use_counts.get(outer_var.name, 0) != 1:
        return instr
    if inner_idx in dropped_def_indices:
        return instr
    inner = all_instrs[inner_idx]
    if not (
        isinstance(inner, tac_ast.Binary)
        and isinstance(inner.op, tac_ast.Add)
    ):
        return instr
    inner_const, inner_other = _split_const_var_or_const(
        inner.src1, inner.src2,
    )
    if inner_const is None:
        return instr
    outer_bits = _BITS_FOR_VARIANT.get(type(outer_const.const))
    inner_bits = _BITS_FOR_VARIANT.get(type(inner_const.const))
    if outer_bits is None or outer_bits != inner_bits:
        # Different bit widths → can't combine directly. Same-width
        # signed vs unsigned IS allowed: the bit pattern of an Add
        # is signedness-agnostic, and the result wraps modulo 2^N
        # the same way either way. We pick the outer's variant for
        # the combined Constant — downstream consumers see the
        # expected result type.
        return instr
    combined_value = _wrap(
        outer_const.const.value + inner_const.const.value,
        outer_const.const,
    )
    combined = tac_ast.Constant(
        const=type(outer_const.const)(value=combined_value),
    )
    dropped_def_indices.add(inner_idx)
    return tac_ast.Binary(
        op=tac_ast.Add(),
        src1=combined,
        src2=inner_other,
        dst=instr.dst,
    )


def _split_const_var(
    a: tac_ast.Type_val, b: tac_ast.Type_val,
) -> tuple[tac_ast.Constant | None, tac_ast.Var | None]:
    """If exactly one of (a, b) is Constant and the other is Var,
    return (constant, var). Otherwise return (None, None)."""
    if isinstance(a, tac_ast.Constant) and isinstance(b, tac_ast.Var):
        return (a, b)
    if isinstance(b, tac_ast.Constant) and isinstance(a, tac_ast.Var):
        return (b, a)
    return (None, None)


def _split_const_var_or_const(
    a: tac_ast.Type_val, b: tac_ast.Type_val,
) -> tuple[tac_ast.Constant | None, tac_ast.Type_val | None]:
    """If exactly one of (a, b) is Constant, return (constant,
    other_operand). The other operand can be Var OR Constant.
    Otherwise (None, None)."""
    if isinstance(a, tac_ast.Constant) and not isinstance(
        b, tac_ast.Constant,
    ):
        return (a, b)
    if isinstance(b, tac_ast.Constant) and not isinstance(
        a, tac_ast.Constant,
    ):
        return (b, a)
    return (None, None)


def _wrap(value: int, c: tac_ast.Type_const) -> int:
    """Wrap `value` to the bit width of `c`'s variant. Mirrors what
    the 6502 lowering computes for the corresponding integer
    width — modulo 2^N for an N-bit variant."""
    bits = _BITS_FOR_VARIANT.get(type(c))
    if bits is None:
        return value
    mask = (1 << bits) - 1
    raw = value & mask
    # For signed variants, canonicalize to two's-complement signed
    # representation so the resulting Constant's `.value` field is
    # in the variant's natural range.
    if isinstance(c, _SIGNED_VARIANTS):
        if raw & (1 << (bits - 1)):
            raw -= 1 << bits
    return raw


_BITS_FOR_VARIANT: dict[type, int] = {
    tac_ast.ConstChar: 8,
    tac_ast.ConstUChar: 8,
    tac_ast.ConstInt: 16,
    tac_ast.ConstUInt: 16,
    tac_ast.ConstLong: 32,
    tac_ast.ConstULong: 32,
    tac_ast.ConstLongLong: 64,
    tac_ast.ConstULongLong: 64,
}


_SIGNED_VARIANTS: tuple[type, ...] = (
    tac_ast.ConstChar, tac_ast.ConstInt,
    tac_ast.ConstLong, tac_ast.ConstLongLong,
)
