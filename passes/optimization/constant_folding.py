"""TAC constant folding pass.

For `Unary` / `Binary` whose every val operand is a `Constant`, and
for `JumpIfTrue` / `JumpIfFalse` whose condition is a `Constant`,
evaluate the operation in Python with arithmetic that matches what
the 6502 lowering would compute, then rewrite the instruction:

  - Unary / Binary  → Copy(Constant(result), dst)  (preserves dst)
  - JumpIfTrue(true)   /  JumpIfFalse(false)      → Jump(target)
  - JumpIfTrue(false)  /  JumpIfFalse(true)       → dropped

Integer width and signedness, matching `tac_to_asm`:

  - Integer constants carry width via the `const` variant: ConstInt
    is 8 bits, ConstLong 16, ConstLongLong 32.
  - Each integer fold result is masked to the result variant's bit
    width and reinterpreted as a two's-complement signed integer in
    that width — so `~0` and `-1` produce the same `ConstInt(-1)`,
    keeping the optimizer's structural-equality fixed-point check
    well-behaved.
  - Arithmetic / bitwise Binary ops: result variant matches src1
    (equal to src2 post the type checker's usual arithmetic
    conversions). Shift ops: result variant matches src1; the
    count operand (src2) is read at its own width and bounds-
    checked separately.
  - Comparison Binary ops always yield ConstInt and interpret
    operands as signed — c6502's V-corrected SBC sequence is used
    for every integer ordering today, so unsigned wrap-around above
    the half-width threshold isn't honored at codegen and shouldn't
    be honored here either.
  - Right shifts fold arithmetically (sign-preserving) — matches
    `tac_to_asm`'s `asr8` / `asr16` / `asr32` dispatch.
  - `Unary(LogicalNot)` always yields ConstInt regardless of the
    source operand's variant (per C99 §6.5.3.3.5: `!` returns int).

Floating-point semantics, via `fp_arith` (numpy-backed at the
operand precision):

  - `Negate` is a sign-bit flip (exact; preserves NaN payloads,
    swaps ±0).
  - `Add` / `Subtract` / `Multiply` / `Divide` round to nearest-
    even at the operand variant's precision; overflow → ±inf;
    invalid (e.g. `0/0`, `inf - inf`) → NaN.
  - Comparisons follow IEEE 754 §5.11: `+0 == -0`; any comparison
    against a NaN is unordered (== returns false; != returns true;
    <, >, <=, >= all return false). Result is `ConstInt`.
  - `JumpIf` truthiness follows C99 §6.3.1.2: a value compares
    truthy iff it compares unequal to 0. Both ±0 are falsy; NaN is
    truthy (NaN != 0 by definition). `LogicalNot` follows the
    same rule.

Cases left unfolded:

  - `Divide` / `Modulo` (integer) with a zero divisor: undefined;
    let the runtime helper decide (or trap). FP `Divide` by zero
    DOES fold — IEEE 754 makes it well-defined (±inf or NaN).
  - Integer shifts where the count is negative or ≥ the operand's
    width: UB per C99 §6.5.7.3, and the helpers' behavior at such
    counts isn't part of the contract yet.
  - Binary ops with mismatched src1 / src2 variants: shouldn't
    happen after type checking, but bail rather than guess at the
    result width / precision.
  - `Complement` / `Modulo` / `BitwiseAnd|Or|Xor` / `LeftShift` /
    `RightShift` on FP operands: all integer-only in C; the type
    checker rejects them, but the guards stay defensive.
"""

from __future__ import annotations

import fp_arith
import tac_ast


# Bit width per integer const variant. FP variants are handled
# separately via `fp_arith` — they have precision, not bit width
# in the same sense.
_INTEGER_CONST_BITS: dict[type, int] = {
    tac_ast.ConstInt: 8,
    tac_ast.ConstLong: 16,
    tac_ast.ConstLongLong: 32,
}


def constant_fold(fn: tac_ast.Function) -> tac_ast.Function:
    out: list[tac_ast.Type_instruction] = []
    for instr in fn.instructions:
        folded = _fold(instr)
        if folded is not None:
            out.append(folded)
    return tac_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=out,
    )


def _fold(
    instr: tac_ast.Type_instruction,
) -> tac_ast.Type_instruction | None:
    """Return the rewritten instruction, the original instruction if
    nothing folds, or None to drop the instruction (only happens for
    a JumpIf that's never taken)."""
    match instr:
        case tac_ast.Unary(
            op=op,
            src=tac_ast.Constant(const=c),
            dst=dst,
        ):
            res = _fold_unary(op, c)
            if res is None:
                return instr
            return tac_ast.Copy(src=tac_ast.Constant(const=res), dst=dst)
        case tac_ast.Binary(
            op=op,
            src1=tac_ast.Constant(const=c1),
            src2=tac_ast.Constant(const=c2),
            dst=dst,
        ):
            res = _fold_binary(op, c1, c2)
            if res is None:
                return instr
            return tac_ast.Copy(src=tac_ast.Constant(const=res), dst=dst)
        case tac_ast.JumpIfTrue(
            condition=tac_ast.Constant(const=c), target=t,
        ):
            tv = _truth_value(c)
            if tv is None:
                return instr
            return tac_ast.Jump(target=t) if tv else None
        case tac_ast.JumpIfFalse(
            condition=tac_ast.Constant(const=c), target=t,
        ):
            tv = _truth_value(c)
            if tv is None:
                return instr
            return None if tv else tac_ast.Jump(target=t)
    return instr


def _is_integer_const(c: tac_ast.Type_const) -> bool:
    return isinstance(c, tuple(_INTEGER_CONST_BITS.keys()))


def _to_signed(value: int, bits: int) -> int:
    """Interpret `value` as a `bits`-wide two's-complement signed
    integer. Idempotent: a value already in the signed range is
    returned unchanged."""
    mask = (1 << bits) - 1
    value &= mask
    if value & (1 << (bits - 1)):
        return value - (1 << bits)
    return value


def _wrap_int(variant: type, value: int) -> tac_ast.Type_const:
    """Build a const of `variant` from an unbounded Python int,
    canonicalized to the variant's signed range."""
    bits = _INTEGER_CONST_BITS[variant]
    return variant(int=_to_signed(value, bits))


def _truth_value(c: tac_ast.Type_const) -> bool | None:
    """Truth value of a constant per C99 §6.3.1.2 (compares unequal
    to 0). For FP, ±0 are falsy and NaN is truthy. Returns None for
    constants we can't classify."""
    if _is_integer_const(c):
        return c.int != 0
    if isinstance(c, tac_ast.ConstFloat):
        return fp_arith.single_is_truthy(c.bits)
    if isinstance(c, tac_ast.ConstDouble):
        return fp_arith.double_is_truthy(c.bits)
    return None


def _fold_unary(
    op: tac_ast.Type_unary_operator, c: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    # LogicalNot is defined for both integer and FP operands and
    # always yields ConstInt (per C99 §6.5.3.3.5).
    if isinstance(op, tac_ast.LogicalNot):
        if _is_integer_const(c):
            return tac_ast.ConstInt(int=1 if c.int == 0 else 0)
        if isinstance(c, tac_ast.ConstFloat):
            return tac_ast.ConstInt(
                int=1 if fp_arith.single_is_zero(c.bits) else 0,
            )
        if isinstance(c, tac_ast.ConstDouble):
            return tac_ast.ConstInt(
                int=1 if fp_arith.double_is_zero(c.bits) else 0,
            )
        return None
    # Negate works on integers and FP; `Complement` (~) is
    # integer-only — the C grammar forbids `~` on FP, so we don't
    # define a meaning for FP here.
    if isinstance(op, tac_ast.Negate):
        if _is_integer_const(c):
            variant = type(c)
            bits = _INTEGER_CONST_BITS[variant]
            return _wrap_int(variant, -_to_signed(c.int, bits))
        if isinstance(c, tac_ast.ConstFloat):
            return tac_ast.ConstFloat(bits=fp_arith.single_negate(c.bits))
        if isinstance(c, tac_ast.ConstDouble):
            return tac_ast.ConstDouble(bits=fp_arith.double_negate(c.bits))
        return None
    if isinstance(op, tac_ast.Complement):
        if not _is_integer_const(c):
            return None
        variant = type(c)
        bits = _INTEGER_CONST_BITS[variant]
        return _wrap_int(variant, ~_to_signed(c.int, bits))
    return None


def _fold_binary(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    # Comparisons: integer or FP, always yield ConstInt.
    if isinstance(op, (
        tac_ast.Equal, tac_ast.NotEqual,
        tac_ast.LessThan, tac_ast.GreaterThan,
        tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
    )):
        return _fold_comparison(op, c1, c2)

    # Shifts: src2 (count) may have a different width than src1.
    # Shifts are integer-only; `_fold_shift` bails on FP operands.
    if isinstance(op, (tac_ast.LeftShift, tac_ast.RightShift)):
        return _fold_shift(op, c1, c2)

    # FP arithmetic — only Add / Sub / Mul / Div are valid for FP
    # in C. The other binary ops (`%`, `& | ^`, shifts) are
    # integer-only at the C grammar level and fall through to the
    # integer path below, which will bail on FP operands.
    if isinstance(op, (tac_ast.Add, tac_ast.Subtract,
                       tac_ast.Multiply, tac_ast.Divide)):
        fp_result = _fold_fp_arith(op, c1, c2)
        if fp_result is not None:
            return fp_result

    # Integer arithmetic / bitwise: src1 and src2 share the result
    # variant.
    if not _is_integer_const(c1) or not _is_integer_const(c2):
        return None
    if type(c1) is not type(c2):
        return None
    variant = type(c1)
    bits = _INTEGER_CONST_BITS[variant]
    a = _to_signed(c1.int, bits)
    b = _to_signed(c2.int, bits)
    match op:
        case tac_ast.Add():
            return _wrap_int(variant, a + b)
        case tac_ast.Subtract():
            return _wrap_int(variant, a - b)
        case tac_ast.Multiply():
            return _wrap_int(variant, a * b)
        case tac_ast.Divide():
            if b == 0:
                return None
            return _wrap_int(variant, _trunc_div(a, b))
        case tac_ast.Modulo():
            if b == 0:
                return None
            return _wrap_int(variant, _trunc_mod(a, b))
        case tac_ast.BitwiseAnd():
            return _wrap_int(variant, a & b)
        case tac_ast.BitwiseOr():
            return _wrap_int(variant, a | b)
        case tac_ast.BitwiseXor():
            return _wrap_int(variant, a ^ b)
    return None


def _fold_fp_arith(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    """IEEE 754 arithmetic at the operand precision. Returns None
    if either operand isn't FP, or if the variants don't match
    (mismatched precision shouldn't happen post-type-check)."""
    if isinstance(c1, tac_ast.ConstFloat) and isinstance(
        c2, tac_ast.ConstFloat,
    ):
        match op:
            case tac_ast.Add():
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_add(c1.bits, c2.bits),
                )
            case tac_ast.Subtract():
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_sub(c1.bits, c2.bits),
                )
            case tac_ast.Multiply():
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_mul(c1.bits, c2.bits),
                )
            case tac_ast.Divide():
                # IEEE 754 division by zero is well-defined: ±inf
                # for nonzero numerator, NaN for 0/0.
                return tac_ast.ConstFloat(
                    bits=fp_arith.single_div(c1.bits, c2.bits),
                )
        return None
    if isinstance(c1, tac_ast.ConstDouble) and isinstance(
        c2, tac_ast.ConstDouble,
    ):
        match op:
            case tac_ast.Add():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_add(c1.bits, c2.bits),
                )
            case tac_ast.Subtract():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_sub(c1.bits, c2.bits),
                )
            case tac_ast.Multiply():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_mul(c1.bits, c2.bits),
                )
            case tac_ast.Divide():
                return tac_ast.ConstDouble(
                    bits=fp_arith.double_div(c1.bits, c2.bits),
                )
        return None
    return None


def _trunc_div(a: int, b: int) -> int:
    """C99 §6.5.5.6 integer division: truncate toward zero. (Python's
    `//` truncates toward negative infinity, so we can't use it
    directly when signs differ.)"""
    q = abs(a) // abs(b)
    if (a < 0) != (b < 0):
        q = -q
    return q


def _trunc_mod(a: int, b: int) -> int:
    """C99 §6.5.5.6 modulo: a - (a/b)*b, with `/` being truncation
    toward zero. Sign of the result matches the dividend."""
    return a - _trunc_div(a, b) * b


def _fold_shift(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    if not _is_integer_const(c1) or not _is_integer_const(c2):
        return None
    variant = type(c1)
    bits = _INTEGER_CONST_BITS[variant]
    a = _to_signed(c1.int, bits)
    # The count's width is its own variant's width — can differ
    # from the value's width.
    count = _to_signed(c2.int, _INTEGER_CONST_BITS[type(c2)])
    if count < 0 or count >= bits:
        # C99 §6.5.7.3: undefined. The c6502 helpers (asl8/16/32 and
        # asr8/16/32) explicitly document this case as UB too.
        return None
    match op:
        case tac_ast.LeftShift():
            return _wrap_int(variant, a << count)
        case tac_ast.RightShift():
            # Arithmetic right shift — matches asr*. Python's `>>`
            # is sign-preserving on negative ints already.
            return _wrap_int(variant, a >> count)
    return None


def _fold_comparison(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    if type(c1) is not type(c2):
        return None
    if isinstance(c1, tac_ast.ConstFloat):
        ord_ = fp_arith.single_compare(c1.bits, c2.bits)
        return _fp_comparison_result(op, ord_)
    if isinstance(c1, tac_ast.ConstDouble):
        ord_ = fp_arith.double_compare(c1.bits, c2.bits)
        return _fp_comparison_result(op, ord_)
    if not _is_integer_const(c1):
        return None
    bits = _INTEGER_CONST_BITS[type(c1)]
    a = _to_signed(c1.int, bits)
    b = _to_signed(c2.int, bits)
    match op:
        case tac_ast.Equal():
            r = a == b
        case tac_ast.NotEqual():
            r = a != b
        case tac_ast.LessThan():
            r = a < b
        case tac_ast.GreaterThan():
            r = a > b
        case tac_ast.LessOrEqual():
            r = a <= b
        case tac_ast.GreaterOrEqual():
            r = a >= b
        case _:
            return None
    return tac_ast.ConstInt(int=1 if r else 0)


def _fp_comparison_result(
    op: tac_ast.Type_binary_operator, order: str,
) -> tac_ast.Type_const | None:
    """Map an `fp_arith.*_compare` outcome (`lt` / `eq` / `gt` /
    `unordered`) plus a TAC comparison op to a ConstInt 0/1 result.
    Per IEEE 754: any comparison against NaN is unordered; equality
    treats it as not-equal (so `==` → 0, `!=` → 1), and all four
    relational operators return false."""
    match op:
        case tac_ast.Equal():
            r = order == "eq"
        case tac_ast.NotEqual():
            r = order != "eq"
        case tac_ast.LessThan():
            r = order == "lt"
        case tac_ast.GreaterThan():
            r = order == "gt"
        case tac_ast.LessOrEqual():
            r = order in ("lt", "eq")
        case tac_ast.GreaterOrEqual():
            r = order in ("gt", "eq")
        case _:
            return None
    return tac_ast.ConstInt(int=1 if r else 0)
