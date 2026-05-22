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
become dead and SSA-DCE collects them on the next iteration. (If
they have other consumers they survive — the rewrite is purely
local to the Truncate.)

# Iteration

Runs inside the TAC fixed-point loop. Composes with
`fold_truncate_extend` (which handles the no-Binary case),
constant folding (which sometimes resolves one Extend to a
Constant), and SSA-DCE (which collects the orphaned chain).
"""
from __future__ import annotations

import c99_ast
import tac_ast


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
    if ssa_dsts is None or symbols is None:
        return fn
    def_idx: dict[str, int] = {}
    for i, instr in enumerate(fn.instructions):
        if isinstance(instr, (
            tac_ast.SignExtend, tac_ast.ZeroExtend, tac_ast.Binary,
        )):
            if (isinstance(instr.dst, tac_ast.Var)
                    and instr.dst.name in ssa_dsts):
                def_idx[instr.dst.name] = i
    if not def_idx:
        return fn
    out: list[tac_ast.Type_instruction] = []
    for instr in fn.instructions:
        out.append(_maybe_rewrite(
            instr, fn.instructions, def_idx, symbols, ssa_dsts,
        ))
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _maybe_rewrite(
    instr: tac_ast.Type_instruction,
    all_instrs: list[tac_ast.Type_instruction],
    def_idx: dict[str, int],
    symbols,
    ssa_dsts: set[str],
) -> tac_ast.Type_instruction:
    if not isinstance(instr, tac_ast.Truncate):
        return instr
    if not isinstance(instr.src, tac_ast.Var):
        return instr
    if instr.src.name not in ssa_dsts:
        return instr
    bin_idx = def_idx.get(instr.src.name)
    if bin_idx is None:
        return instr
    bin_instr = all_instrs[bin_idx]
    if not isinstance(bin_instr, tac_ast.Binary):
        return instr
    if not isinstance(bin_instr.op, _SAFE_OPS):
        return instr
    if not isinstance(bin_instr.dst, tac_ast.Var):
        return instr
    dst_width = _byte_width(instr.dst, symbols)
    bin_width = _byte_width(bin_instr.dst, symbols)
    if dst_width is None or bin_width is None:
        return instr
    if dst_width >= bin_width:
        # No widening to undo. fold_truncate_extend or constant_fold
        # handles the trivial cases.
        return instr
    dst_unsigned = _is_unsigned(instr.dst, symbols)
    if dst_unsigned is None:
        return instr
    narrow_s1 = _narrow_operand(
        bin_instr.src1, dst_width, dst_unsigned,
        all_instrs, def_idx, symbols, ssa_dsts,
    )
    if narrow_s1 is None:
        return instr
    narrow_s2 = _narrow_operand(
        bin_instr.src2, dst_width, dst_unsigned,
        all_instrs, def_idx, symbols, ssa_dsts,
    )
    if narrow_s2 is None:
        return instr
    return tac_ast.Binary(
        op=bin_instr.op, src1=narrow_s1, src2=narrow_s2,
        dst=instr.dst,
    )


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
