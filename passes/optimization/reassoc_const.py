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

import tac_ast
from passes.optimization.var_visit import count_uses
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, Rewrite, FixedpointPass, PassContext, MatchResult,
    m_Commutative, m_Constant, m_Var, m_Any,
)


def reassoc_constants(fn: tac_ast.Function) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each outer Add(Const, %inner)
    where the inner is a single-use Pseudo defined by another
    Add(Const, ...), combine the two Constants. Returns a new
    Function; doesn't mutate the input."""
    return _IMPL.run(fn, PassContext(ssa_dsts=_all_dsts(fn)))


def _all_dsts(fn: tac_ast.Function) -> set[str]:
    """Return the set of all Var dst names in `fn`. Used as a
    stand-in for ssa_dsts to satisfy the DefUsePass.run gate when
    calling from the free function (which may not be in SSA form).
    reassoc_constants is only called during the SSA fixed-point loop,
    so all named temps are SSA-renamed in practice."""
    out: set[str] = set()
    for instr in fn.instructions:
        if hasattr(instr, 'dst') and isinstance(instr.dst, tac_ast.Var):
            out.add(instr.dst.name)
    return out


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

# Inner-Binary pattern: Add with one Constant and one arbitrary operand.
_inner_pattern = m_Commutative(
    op=tac_ast.Add,
    lhs=m_Constant(capture='inner_const'),
    rhs=m_Any(capture='inner_other'),
)


class ReassocConstants(DefUsePass):
    """Fuses nested Add(Constant, Add(Constant, V)) chains into a single
    Add with the combined Constant, dropping the inner def atomically
    via Rewrite.drop_defs. The instructions_view trick in DefUsePass.run
    ensures chained fusions (A→B then B→C) see the rewritten form on the
    second match so they don't reference a stale already-dropped def."""
    name = "reassoc_constants"

    pattern = m_Commutative(
        op=tac_ast.Add,
        lhs=m_Constant(capture='outer_const'),
        rhs=m_Var(capture='outer_var'),
        dst=m_Var(capture='outer_dst'),
    )

    def prepare_extra(self, fn, ctx):
        return count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        outer_var: tac_ast.Var = m.bindings['outer_var']
        outer_const: tac_ast.Constant = m.bindings['outer_const']
        outer_dst: tac_ast.Var = m.bindings['outer_dst']

        # Gate on SSA-renamed inner var (single-def guarantee).
        if ctx.ssa_dsts is None or outer_var.name not in ctx.ssa_dsts:
            return None

        # Single-use gate: only fuse when %inner has exactly one reader.
        if env.extra.get(outer_var.name, 0) != 1:
            return None

        # Walk back to the inner Binary(Add) def.
        inner = env.def_of(outer_var)
        if inner is None:
            return None

        # Reject self-update shape (non-SSA `x = x + C` where
        # outer_var.name == outer_dst.name). The def_of would point at
        # the current instruction being matched, causing a fusion with
        # itself that would silently erase the increment.
        if inner is env.instructions[env.def_idx.get(outer_dst.name, -1)] \
                and outer_var.name == outer_dst.name:
            return None

        # Match the inner instruction.
        inner_m = MatchResult()
        if not _inner_pattern.match(inner, inner_m):
            return None

        inner_const: tac_ast.Constant = inner_m.bindings['inner_const']
        inner_other: tac_ast.Type_val = inner_m.bindings['inner_other']

        # Width gate: must share the same bit width (same-width signed vs
        # unsigned IS allowed — the bit pattern of Add is signedness-agnostic).
        outer_bits = _BITS_FOR_VARIANT.get(type(outer_const.const))
        inner_bits = _BITS_FOR_VARIANT.get(type(inner_const.const))
        if outer_bits is None or outer_bits != inner_bits:
            return None

        combined_value = _wrap(
            outer_const.const.value + inner_const.const.value,
            outer_const.const,
        )
        combined = tac_ast.Constant(
            const=type(outer_const.const)(value=combined_value),
        )
        replacement = tac_ast.Binary(
            op=tac_ast.Add(),
            src1=combined,
            src2=inner_other,
            dst=outer_dst,
        )
        return Rewrite(replacement=replacement, drop_defs=(outer_var,))


_IMPL = ReassocConstants()
