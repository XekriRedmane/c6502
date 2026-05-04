"""TAC strength-reduction pass.

Rewrites multiplicative ops by power-of-2 constants into shifts
(or simpler ops):

  * `Multiply(x, 2^k)` / `Multiply(2^k, x)` → `LeftShift(x, k)`
    (any signedness — left shift by k is equivalent to multiply
    by 2^k for both signed and unsigned, with the same wrap-around
    behavior).
  * `Multiply(x, 1)` / `Multiply(1, x)` → `Copy(x, dst)`.
  * `Multiply(x, 0)` / `Multiply(0, x)` → not handled here —
    `constant_folding` catches the all-constant case; the mixed
    `var * 0` case is left alone (constant folding via copy
    propagation usually unblocks it later).
  * `Divide(x, 2^k)` UNSIGNED → `RightShift(x, k)`. Signed
    `Divide(x, 2^k)` is NOT rewritten — C99 §6.5.5.6 truncates
    toward zero, while arithmetic right shift rounds toward
    negative infinity. They disagree on negative dividends:
    `-3 / 2 == -1` but `-3 >> 1 == -2`.
  * `Divide(x, 1)` → `Copy(x, dst)` (any signedness).
  * `Modulo(x, 2^k)` UNSIGNED → `BitwiseAnd(x, 2^k - 1)`. Signed
    modulo by 2^k is similarly NOT rewritten (sign of the result
    follows the dividend, while bit-AND can't produce a negative
    result).
  * `Modulo(x, 1)` → `Copy(0, dst)` of the appropriate constant
    variant (any value mod 1 is 0).

`Copy(0, dst)` for the modulo-by-1 case requires building a typed
zero of the dst's variant; we read that off the symbol table.

Why this matters for c6502 specifically: `Multiply` lowers to a
runtime helper call (`mul8` / `mul16` / `mul32`), and any value
live across that call gets pushed into a callee-saved ZP color,
forcing prologue/epilogue save/restore. After strength reduction,
`mul` calls disappear for power-of-2 multipliers — combined with
the asm-level inline-shift-by-1 in `tac_to_asm`, the call goes
away entirely, so cross-call lifetimes shrink and the function's
prologue can collapse further.

Pure structural rewrite — no value evaluation needed beyond
checking the constant. Safe to run before or after constant
folding.
"""
from __future__ import annotations

import tac_ast


def reduce_strength(
    fn: tac_ast.Function,
    *,
    symbols=None,
) -> tac_ast.Function:
    """Rewrite each instruction whose multiplicative op has a
    power-of-2 constant operand. `symbols` is the type-checker's
    SymbolTable (or any mapping with `.get(name) -> Symbol | None`)
    — needed to construct typed zeros for the modulo-by-1 case and
    to detect the signedness of Var operands for unsigned-only
    rewrites (Divide, Modulo)."""
    out: list[tac_ast.Type_instruction] = []
    changed = False
    for instr in fn.instructions:
        replacement = _reduce(instr, symbols)
        if replacement is None:
            out.append(instr)
            continue
        changed = True
        out.append(replacement)
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=out,
    )


def _reduce(
    instr: tac_ast.Type_instruction,
    symbols,
) -> tac_ast.Type_instruction | None:
    """Try to rewrite `instr`. Returns the replacement or None if
    no rewrite applies."""
    if not isinstance(instr, tac_ast.Binary):
        return None
    op = instr.op
    src1, src2, dst = instr.src1, instr.src2, instr.dst

    if isinstance(op, tac_ast.Multiply):
        # When both sides are constants, defer to constant_folding —
        # it'll collapse the whole thing to a Copy(Constant, dst)
        # at the right value.
        if isinstance(src1, tac_ast.Constant) and isinstance(
            src2, tac_ast.Constant,
        ):
            return None
        # Multiply is commutative — try both orderings.
        for var_side, const_side in ((src1, src2), (src2, src1)):
            c = _power_of_two_const(const_side)
            if c is None:
                continue
            k, _ = c
            if k == 0:
                return tac_ast.Copy(src=var_side, dst=dst)
            count = _shift_count_const(k, var_side, symbols)
            return tac_ast.Binary(
                op=tac_ast.LeftShift(),
                src1=var_side, src2=count, dst=dst,
            )
        return None

    if isinstance(op, tac_ast.Divide):
        # Only `x / Constant` is reducible — not `Constant / x`
        # (commutativity doesn't hold for division).
        c = _power_of_two_const(src2)
        if c is None:
            return None
        k, _ = c
        if k == 0:
            # x / 1 → Copy(x, dst). Same ordering caveat as above.
            if isinstance(src1, tac_ast.Constant):
                return None
            return tac_ast.Copy(src=src1, dst=dst)
        # Signed → arithmetic right shift would round toward
        # negative infinity, but C99 truncates toward zero. Skip.
        if not _is_unsigned(src1, symbols):
            return None
        count = _shift_count_const(k, src1, symbols)
        return tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=src1, src2=count, dst=dst,
        )

    if isinstance(op, tac_ast.Modulo):
        c = _power_of_two_const(src2)
        if c is None:
            return None
        k, value = c
        if k == 0:
            # x % 1 == 0 — Copy(typed-zero, dst).
            zero = _zero_const(src1, symbols)
            if zero is None:
                return None
            return tac_ast.Copy(
                src=tac_ast.Constant(const=zero), dst=dst,
            )
        if not _is_unsigned(src1, symbols):
            return None
        # x % 2^k → x & (2^k - 1). Build the mask as a constant of
        # the same variant as src2 (which the type checker has
        # stamped to match src1's promoted type).
        mask = value - 1
        mask_const = _const_with_value(src2, mask)
        if mask_const is None:
            return None
        return tac_ast.Binary(
            op=tac_ast.BitwiseAnd(),
            src1=src1, src2=tac_ast.Constant(const=mask_const), dst=dst,
        )

    return None


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def _power_of_two_const(
    val: tac_ast.Type_val,
) -> tuple[int, int] | None:
    """If `val` is a positive integer Constant whose value is a
    power of two (1, 2, 4, 8, ...), return `(log2(value), value)`.
    Otherwise None.

    The "1" case (k=0) is included so callers can fold mul/div by
    1 to a Copy."""
    if not isinstance(val, tac_ast.Constant):
        return None
    c = val.const
    if not isinstance(c, _INT_CONST_TYPES):
        return None
    v = c.value
    # Unsigned variants store non-negative values in TAC; signed
    # variants might be negative (multiplier of -2^k is NOT a power
    # of 2 — we only optimize positive multipliers).
    if v <= 0:
        return None
    if v & (v - 1):
        return None  # not a power of two
    return v.bit_length() - 1, v


def _shift_count_const(
    k: int,
    val_side: tac_ast.Type_val,
    symbols,
) -> tac_ast.Constant:
    """Build a typed shift-count constant for `LeftShift` /
    `RightShift`'s src2. Per C99 §6.5.7.3 the shift count's type is
    its promoted self — independent of the value side. tac_to_asm
    reads only the count's low byte, so the variant width doesn't
    matter at runtime; pick `ConstInt` (the natural width for a
    small literal). Caller passes `val_side` for symmetry / future
    use."""
    return tac_ast.Constant(const=tac_ast.ConstInt(value=k))


def _zero_const(
    val: tac_ast.Type_val,
    symbols,
) -> tac_ast.Type_const | None:
    """Build a typed zero matching the variant of `val`. For Var,
    looks up the symbol table for its c99 type; for Constant, uses
    the constant's variant. Returns None if we can't determine the
    variant (no symbols, missing entry, etc.)."""
    if isinstance(val, tac_ast.Constant):
        variant = type(val.const)
        if variant in _ZERO_FOR_VARIANT:
            return _ZERO_FOR_VARIANT[variant]()
        return None
    if isinstance(val, tac_ast.Var):
        if symbols is None:
            return None
        sym = symbols.get(val.name)
        if sym is None:
            return None
        return _zero_for_c99_type(sym.type)
    return None


def _const_with_value(
    template: tac_ast.Type_val, value: int,
) -> tac_ast.Type_const | None:
    """Build a Constant of the same variant as `template` (which
    must be a Constant) carrying `value`."""
    if not isinstance(template, tac_ast.Constant):
        return None
    variant = type(template.const)
    if variant not in _INT_CONST_TYPES:
        return None
    return variant(value=value)


def _is_unsigned(val: tac_ast.Type_val, symbols) -> bool:
    """True iff `val` is unsigned. For Constants, the variant
    determines it; for Vars, the symbol-table c99 type. Mirrors
    `tac_to_asm._is_unsigned_val` so signedness-driven decisions
    agree across passes."""
    if isinstance(val, tac_ast.Constant):
        return isinstance(val.const, _UNSIGNED_INT_CONST_TYPES)
    if isinstance(val, tac_ast.Var):
        if symbols is None:
            return False
        sym = symbols.get(val.name)
        if sym is None:
            return False
        import c99_ast
        return isinstance(sym.type, (
            c99_ast.UInt, c99_ast.ULong, c99_ast.ULongLong,
            c99_ast.Char, c99_ast.UChar, c99_ast.Pointer,
        ))
    return False


def _zero_for_c99_type(t):
    """Map a c99 type to its TAC zero-Constant variant. Mirrors the
    typed-zero construction in c99_to_tac (`_tac_const_for`)."""
    import c99_ast
    if isinstance(t, c99_ast.Int):
        return tac_ast.ConstInt(value=0)
    if isinstance(t, c99_ast.Long):
        return tac_ast.ConstLong(value=0)
    if isinstance(t, c99_ast.LongLong):
        return tac_ast.ConstLongLong(value=0)
    if isinstance(t, c99_ast.UInt):
        return tac_ast.ConstUInt(value=0)
    if isinstance(t, c99_ast.ULong):
        return tac_ast.ConstULong(value=0)
    if isinstance(t, c99_ast.ULongLong):
        return tac_ast.ConstULongLong(value=0)
    if isinstance(t, c99_ast.Char):
        return tac_ast.ConstUInt(value=0)  # char promotes to int
    if isinstance(t, c99_ast.SChar):
        return tac_ast.ConstInt(value=0)
    if isinstance(t, c99_ast.UChar):
        return tac_ast.ConstUInt(value=0)
    return None


_INT_CONST_TYPES: tuple[type, ...] = (
    tac_ast.ConstChar, tac_ast.ConstUChar,
    tac_ast.ConstInt, tac_ast.ConstLong, tac_ast.ConstLongLong,
    tac_ast.ConstUInt, tac_ast.ConstULong, tac_ast.ConstULongLong,
)

_UNSIGNED_INT_CONST_TYPES: tuple[type, ...] = (
    tac_ast.ConstUChar,
    tac_ast.ConstUInt, tac_ast.ConstULong, tac_ast.ConstULongLong,
)

_ZERO_FOR_VARIANT: dict[type, type] = {
    tac_ast.ConstChar: lambda: tac_ast.ConstChar(value=0),
    tac_ast.ConstUChar: lambda: tac_ast.ConstUChar(value=0),
    tac_ast.ConstInt: lambda: tac_ast.ConstInt(value=0),
    tac_ast.ConstLong: lambda: tac_ast.ConstLong(value=0),
    tac_ast.ConstLongLong: lambda: tac_ast.ConstLongLong(value=0),
    tac_ast.ConstUInt: lambda: tac_ast.ConstUInt(value=0),
    tac_ast.ConstULong: lambda: tac_ast.ConstULong(value=0),
    tac_ast.ConstULongLong: lambda: tac_ast.ConstULongLong(value=0),
}
