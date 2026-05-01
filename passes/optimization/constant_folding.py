"""TAC constant folding pass.

For `Unary` / `Binary` whose every val operand is a `Constant`, and
for `JumpIfTrue` / `JumpIfFalse` whose condition is a `Constant`,
evaluate the operation in Python with arithmetic that matches what
the 6502 lowering would compute, then rewrite the instruction:

  - Unary / Binary  → Copy(Constant(result), dst)  (preserves dst)
  - JumpIfTrue(true)   /  JumpIfFalse(false)      → Jump(target)
  - JumpIfTrue(false)  /  JumpIfFalse(true)       → dropped

Width and signedness conventions, matching `tac_to_asm`:

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

Cases left unfolded:

  - Floating constants (ConstFloat / ConstDouble): the FP runtime
    helpers aren't in this repo yet, so the bit-pattern semantics
    of every FP op aren't pinned.
  - `Divide` / `Modulo` with a zero divisor: undefined; let the
    runtime helper decide (or trap).
  - Shifts where the count is negative or ≥ the operand's width:
    UB per C99 §6.5.7.3, and the helpers' behavior at such counts
    isn't part of the contract yet.
  - Binary arithmetic / bitwise with mismatched src1 / src2
    variants: shouldn't happen after type checking, but bail
    rather than guess at the result width.
"""

from __future__ import annotations

import tac_ast


# Bit width per integer const variant. Float / Double aren't here on
# purpose — see the module docstring for why FP is skipped.
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
    """Truth value of a constant, or None if we won't decide. FP is
    deferred until the FP runtime is wired up."""
    if _is_integer_const(c):
        return c.int != 0
    return None


def _fold_unary(
    op: tac_ast.Type_unary_operator, c: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    if not _is_integer_const(c):
        return None
    variant = type(c)
    bits = _INTEGER_CONST_BITS[variant]
    src = _to_signed(c.int, bits)
    match op:
        case tac_ast.Negate():
            return _wrap_int(variant, -src)
        case tac_ast.Complement():
            return _wrap_int(variant, ~src)
        case tac_ast.LogicalNot():
            # Per C99 §6.5.3.3.5: `!` always returns int, regardless
            # of operand width.
            return tac_ast.ConstInt(int=1 if src == 0 else 0)
    return None


def _fold_binary(
    op: tac_ast.Type_binary_operator,
    c1: tac_ast.Type_const, c2: tac_ast.Type_const,
) -> tac_ast.Type_const | None:
    # Comparisons: signed, always yield ConstInt.
    if isinstance(op, (
        tac_ast.Equal, tac_ast.NotEqual,
        tac_ast.LessThan, tac_ast.GreaterThan,
        tac_ast.LessOrEqual, tac_ast.GreaterOrEqual,
    )):
        return _fold_comparison(op, c1, c2)

    # Shifts: src2 (count) may have a different width than src1.
    if isinstance(op, (tac_ast.LeftShift, tac_ast.RightShift)):
        return _fold_shift(op, c1, c2)

    # Arithmetic / bitwise: src1 and src2 share the result variant.
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
    if not _is_integer_const(c1) or not _is_integer_const(c2):
        return None
    if type(c1) is not type(c2):
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
