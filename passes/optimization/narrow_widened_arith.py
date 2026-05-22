"""TAC peephole: fold `Truncate(Binary(safe_op, Extend(a), Extend(b)
| Constant), u)` → `Binary(safe_op, a, b | narrowed_Constant, u)`
at the narrow width.

# Motivating shape

C99 §6.3.1.1 integer-promotes 1-byte operands to `int` before any
arithmetic or bitwise op. For

    uint8_t a, b;
    uint8_t delta = (uint8_t)(a - b);

c99_to_tac emits:

    ZeroExtend(a   -> %ea:int)
    ZeroExtend(b   -> %eb:int)
    Binary(Subtract, %ea, %eb -> %r:int)    ; 16-bit subtract
    Truncate(%r -> %d:uchar)                ; cast back to 1 byte

`tac_to_asm` lowers the 16-bit subtract as a per-byte SBC chain.
The high byte's `LDA #$0; SBC #$0` is dead — its result feeds only
the dropped high byte of the Truncate — but the asm-level
`dead_a_arith` can't always see through the `LDA mem; BNE` shape
that follows because `all_flags_dead_at` bails at every Branch.
Killing the work at TAC level eliminates it at the source.

# Soundness

For an op whose result modulo 2^(8n) depends only on the operands
modulo 2^(8n) (the "wraparound-safe" ops), the low n bytes of
`op(extend_to_W(a), extend_to_W(b))` equal
`op(a_low_n, b_low_n) mod 2^(8n)`. So:

    Truncate(Binary(safe_op, Extend(a_n), Extend(b_n)), u_n)
        ≡ Binary(safe_op, a_n, b_n, u_n)

Wraparound-safe ops:
  - Add, Subtract — carry propagates strictly low→high.
  - Multiply — low n bytes of (a*b) depend only on low n bytes
    of a and b (two's-complement / unsigned modular arithmetic
    coincide for multiply).
  - BitwiseAnd, BitwiseOr, BitwiseXor — trivially per-byte.

NOT safe (high bytes contribute to low bytes of the result):
  - Divide, Modulo — long division pulls from high bytes.
  - RightShift — high bytes shift down into the low bytes.
  - LeftShift — narrowable in principle (low bytes of `x<<k`
    depend only on low bytes of `x` for any `k`), but excluded
    here because the asm shift helpers consume only one byte of
    the count anyway, so the high-byte work of an `int` LHS is
    already cheap and the win is marginal; revisit if a
    motivating case appears.

For a Constant operand, narrowing reduces the value modulo
2^(8n). Modular arithmetic preserves equivalence regardless of
the constant's magnitude or sign — e.g. `Binary(Subtract,
ZE(uc), Constant(0x1234:int))` truncated to uchar is equivalent
to `Binary(Subtract, uc, Constant(0x34:uchar))`.

# Eligibility (per Truncate)

  - Truncate.src is an SSA-renamed Var whose def is a
    wraparound-safe Binary.
  - Each Binary operand is either:
      (a) an SSA-renamed Var whose def is `SignExtend` or
          `ZeroExtend` of a Var whose declared width equals the
          Truncate's dst width, or
      (b) a Constant we can narrow to the dst width.
  - The Binary's dst width is strictly wider than the Truncate's
    dst width. (Otherwise `constant_fold` / `fold_truncate_extend`
    would already have caught the trivial case.)

# Output

The Truncate is rewritten to a narrow Binary that writes
directly to the Truncate's dst. The original Binary and Extends
become dead; single-use Extend dsts are dropped eagerly via
Rewrite(drop_defs=...) and SSA-DCE collects any remaining
orphaned instructions on the next iteration.

# Iteration

Runs inside the TAC fixed-point loop. Composes with
`fold_truncate_extend` (which handles the no-Binary case),
constant folding (which sometimes resolves one Extend to a
Constant), and SSA-DCE (which collects the orphaned chain).
"""
from __future__ import annotations

from collections import Counter

import c99_ast
import tac_ast
from passes.optimization.framework import (
    DefUsePass, DefUseEnv, PassContext, MatchResult, Rewrite,
    m_Cast, m_Var,
)
from passes.optimization.var_visit import uses_in


# Wraparound-safe ops: the low n bytes of op(W(a), W(b)) depend only
# on the low n bytes of W(a) and W(b). See module docstring.
_SAFE_OPS = (
    tac_ast.Add,
    tac_ast.Subtract,
    tac_ast.Multiply,
    tac_ast.BitwiseAnd,
    tac_ast.BitwiseOr,
    tac_ast.BitwiseXor,
)


def _count_uses(instrs) -> Counter[str]:
    counts: Counter[str] = Counter()
    for instr in instrs:
        for v in uses_in(instr):
            counts[v.name] += 1
    return counts


class NarrowWidenedArith(DefUsePass):
    """DefUsePass: matches Truncate(src=Var, dst=Var). For each match,
    walks back via env.def_of to find a wraparound-safe Binary on
    widened operands; if found, rewrites the Truncate to a narrow
    Binary that produces the Truncate's dst directly.

    Drops the Binary's def eagerly via Rewrite(drop_defs=...) and
    also drops any single-use Extend defs that fed the Binary,
    so SSA-DCE doesn't need a separate sweep to clean them up."""
    name = "narrow_widened_arith"
    pattern = m_Cast(
        kind=tac_ast.Truncate,
        src=m_Var(capture='trunc_src'),
        dst=m_Var(capture='dst'),
        capture='trunc',
    )

    def prepare_extra(self, fn, ctx):
        return _count_uses(fn.instructions)

    def rewrite(self, m: MatchResult, env: DefUseEnv, ctx: PassContext) -> object | None:
        if ctx.ssa_dsts is None or ctx.symbols is None:
            return None
        trunc_src = m.bindings['trunc_src']
        dst_var = m.bindings['dst']

        # trunc_src must be SSA-renamed (single-def guarantee).
        if trunc_src.name not in ctx.ssa_dsts:
            return None

        # Walk back: trunc_src must be defined by a wraparound-safe Binary.
        bin_instr = env.def_of(trunc_src)
        if not isinstance(bin_instr, tac_ast.Binary):
            return None
        if not isinstance(bin_instr.op, _SAFE_OPS):
            return None
        if not isinstance(bin_instr.dst, tac_ast.Var):
            return None

        dst_width = _byte_width(dst_var, ctx.symbols)
        bin_width = _byte_width(bin_instr.dst, ctx.symbols)
        if dst_width is None or bin_width is None:
            return None
        if dst_width >= bin_width:
            # No widening to undo. fold_truncate_extend or constant_fold
            # handles the trivial cases.
            return None

        dst_unsigned = _is_unsigned(dst_var, ctx.symbols)
        if dst_unsigned is None:
            return None

        narrow_s1 = _narrow_operand(
            bin_instr.src1, dst_width, dst_unsigned,
            env.instructions, env.def_idx, ctx.symbols, ctx.ssa_dsts,
        )
        if narrow_s1 is None:
            return None
        narrow_s2 = _narrow_operand(
            bin_instr.src2, dst_width, dst_unsigned,
            env.instructions, env.def_idx, ctx.symbols, ctx.ssa_dsts,
        )
        if narrow_s2 is None:
            return None

        narrow_binary = tac_ast.Binary(
            op=bin_instr.op, src1=narrow_s1, src2=narrow_s2,
            dst=dst_var,
        )

        # Eagerly drop the Binary's dst. Also drop each single-use Extend
        # dst that fed the Binary: use_counts[operand.name] == 1 means
        # only the Binary reads it, so after the Binary is gone it's dead.
        # (The Truncate is the current USE site; it's replaced, not dropped.)
        use_counts = env.extra
        drop_vars: list[tac_ast.Var] = [bin_instr.dst]
        for operand in (bin_instr.src1, bin_instr.src2):
            if not isinstance(operand, tac_ast.Var):
                continue
            if operand.name not in ctx.ssa_dsts:
                continue
            ext = env.def_of(operand)
            if not isinstance(ext, (tac_ast.SignExtend, tac_ast.ZeroExtend)):
                continue
            # Only drop the Extend if the Binary was its sole consumer.
            if use_counts.get(operand.name, 0) == 1:
                drop_vars.append(operand)

        return Rewrite(replacement=narrow_binary, drop_defs=tuple(drop_vars))


def narrow_widened_arith(
    fn: tac_ast.Function, *,
    symbols=None, ssa_dsts: set[str] | None = None,
) -> tac_ast.Function:
    """Walk `fn`'s instructions; for each Truncate(t, u) whose src is
    defined by a wraparound-safe Binary on widened operands, rewrite
    the Truncate to a narrow Binary that produces u directly.

    `ssa_dsts` is the set of names introduced by `to_ssa`. The
    rewrite requires both the Binary's dst and each Extend's dst to
    be SSA-renamed so the def chain is unambiguous. Without it (or
    without `symbols`), the pass is a no-op."""
    ctx = PassContext(symbols=symbols, ssa_dsts=ssa_dsts)
    return NarrowWidenedArith().run(fn, ctx)


def _narrow_operand(
    operand: tac_ast.Type_val,
    target_width: int,
    target_unsigned: bool,
    all_instrs: list[tac_ast.Type_instruction],
    def_idx: dict[str, int],
    symbols,
    ssa_dsts: set[str],
) -> tac_ast.Type_val | None:
    """Return a `Type_val` narrowed to `target_width` bytes (signed
    iff target_unsigned is False), or None if narrowing isn't safe.

    For a Var: requires the Var to be SSA-renamed AND defined by a
    SignExtend / ZeroExtend of a Var whose declared width equals
    `target_width`. Returns the cast's source Var.

    For a Constant: returns a fresh Constant of the matching narrow
    variant, value masked to `target_width` bytes."""
    if isinstance(operand, tac_ast.Constant):
        return _narrow_constant(operand, target_width, target_unsigned)
    if not isinstance(operand, tac_ast.Var):
        return None
    if operand.name not in ssa_dsts:
        return None
    cast_idx = def_idx.get(operand.name)
    if cast_idx is None:
        return None
    cast = all_instrs[cast_idx]
    if not isinstance(cast, (tac_ast.SignExtend, tac_ast.ZeroExtend)):
        return None
    if not isinstance(cast.src, tac_ast.Var):
        return None
    src_width = _byte_width(cast.src, symbols)
    if src_width != target_width:
        return None
    return cast.src


def _narrow_constant(
    c: tac_ast.Constant, width: int, unsigned: bool,
) -> tac_ast.Constant | None:
    """Return a Constant of the matching narrow width / signedness,
    or None if the source is FP (FP narrowing isn't a modular
    reduction, and the safe-op list excludes FP-relevant cases
    anyway)."""
    inner = c.const
    if isinstance(inner, (tac_ast.ConstFloat, tac_ast.ConstDouble)):
        return None
    raw = inner.value
    mask = (1 << (8 * width)) - 1
    masked = raw & mask
    if width == 1:
        if unsigned:
            return tac_ast.Constant(const=tac_ast.ConstUChar(value=masked))
        signed = masked if masked < 0x80 else masked - 0x100
        return tac_ast.Constant(const=tac_ast.ConstChar(value=signed))
    if width == 2:
        if unsigned:
            return tac_ast.Constant(const=tac_ast.ConstUInt(value=masked))
        signed = masked if masked < 0x8000 else masked - 0x10000
        return tac_ast.Constant(const=tac_ast.ConstInt(value=signed))
    if width == 4:
        if unsigned:
            return tac_ast.Constant(const=tac_ast.ConstULong(value=masked))
        signed = masked if masked < 0x80000000 else masked - 0x100000000
        return tac_ast.Constant(const=tac_ast.ConstLong(value=signed))
    if width == 8:
        if unsigned:
            return tac_ast.Constant(
                const=tac_ast.ConstULongLong(value=masked),
            )
        signed = (
            masked if masked < 0x8000000000000000
            else masked - 0x10000000000000000
        )
        return tac_ast.Constant(
            const=tac_ast.ConstLongLong(value=signed),
        )
    return None


def _byte_width(v: tac_ast.Var, symbols) -> int | None:
    """Byte width of `v`'s declared type, or None if symbols missing
    or type is opaque (Array / Structure / Union)."""
    sym = symbols.get(v.name)
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt, c99_ast.Pointer)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong, c99_ast.Float)):
        return 4
    if isinstance(t, (
        c99_ast.LongLong, c99_ast.ULongLong, c99_ast.Double,
    )):
        return 8
    return None


def _is_unsigned(v: tac_ast.Var, symbols) -> bool | None:
    """True iff `v`'s declared type is an unsigned integer type
    (UChar / Char (c6502 plain char is unsigned) / UInt / ULong /
    ULongLong / Pointer). False for the signed integer types.
    None for FP / Array / Structure / Union (the narrowing
    rewrite isn't applicable)."""
    sym = symbols.get(v.name)
    if sym is None:
        return None
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, (c99_ast.Char, c99_ast.UChar)):
        return True
    if isinstance(t, c99_ast.SChar):
        return False
    if isinstance(t, c99_ast.Int):
        return False
    if isinstance(t, (c99_ast.UInt, c99_ast.Pointer)):
        return True
    if isinstance(t, c99_ast.Long):
        return False
    if isinstance(t, c99_ast.ULong):
        return True
    if isinstance(t, c99_ast.LongLong):
        return False
    if isinstance(t, c99_ast.ULongLong):
        return True
    return None
