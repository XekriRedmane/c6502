"""The largest value a TAC operand can hold, when it can't be negative.

`max_value` bounds an integer operand from its type and, for an
SSA-renamed Var, from the instruction that defines it. A bound means
the value lies between 0 and the bound. It returns None when the
value may be negative or nothing bounds it.

The rules for a definition are these, where `max(x)` is the bound of
the operand `x`.

- `ZeroExtend(x)`: `max(x)`, or the largest value of x's width.
- `SignExtend(x)`, `Copy(x)`, `Truncate(x)`: `max(x)`, when it fits
  the destination's type. A Truncate to an unsigned type is bounded
  by its type even when x isn't.
- `a & b`: the smaller bound, since the result has no bit the bounded
  operand lacks. One bound is enough.
- `a | b`, `a ^ b`: all ones up to the higher bit of the two bounds.
- `a + b`, `a * b`, `a << k`: the sum, the product, or the shifted
  bound, when that fits the destination's type, so that the result
  doesn't wrap.
- `a >> n`: `max(a)` shifted right by a constant `n`, or `max(a)` for
  a variable count. A nonnegative value shifts the same way whatever
  its type's signedness.
- `a / b`: `max(a)` divided by a positive constant `b`, or `max(a)`
  for any divisor whose bound is known, since a quotient of
  nonnegative values is no larger than its dividend.
- `a % b`: the smaller of `max(a)` and `b - 1` for a positive
  constant `b`, or `max(a)`.
- `Phi`: the largest bound of its arguments.

A Var's type bounds it too: an unsigned type by its largest value.
A signed type gives no bound, since its values may be negative.

The walk back through definitions stops at a depth of 12, and at a
Var it is already walking (a loop's Phi), where the type bound is
all that's left.

`passes/optimization/narrow_byte_range.py` narrows a divide, a
remainder, a right shift, and a multiply whose operands are bounded
by 255, and `strength_reduction.py` treats a signed divide or
remainder by a power of two as an unsigned one when the dividend is
bounded.
"""
from __future__ import annotations

from collections.abc import Callable

import c99_ast
import tac_ast

_MAX_DEPTH = 12

_INT_CONSTS = (
    tac_ast.ConstChar, tac_ast.ConstUChar, tac_ast.ConstInt,
    tac_ast.ConstUInt, tac_ast.ConstLong, tac_ast.ConstULong,
    tac_ast.ConstLongLong, tac_ast.ConstULongLong,
)

_UNSIGNED_TYPES = (
    c99_ast.Char, c99_ast.UChar, c99_ast.UInt, c99_ast.ULong,
    c99_ast.ULongLong,
)
_SIGNED_TYPES = (
    c99_ast.SChar, c99_ast.Int, c99_ast.Long, c99_ast.LongLong,
)


def _strip(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def int_width(t) -> int | None:
    """The byte width of an integer type, or None for anything else.
    A pointer, a floating type, and an aggregate are None."""
    t = _strip(t)
    if isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar)):
        return 1
    if isinstance(t, (c99_ast.Int, c99_ast.UInt)):
        return 2
    if isinstance(t, (c99_ast.Long, c99_ast.ULong)):
        return 4
    if isinstance(t, (c99_ast.LongLong, c99_ast.ULongLong)):
        return 8
    return None


def type_max(t) -> int | None:
    """The largest value of the integer type `t`, or None for a type
    that isn't an integer type."""
    w = int_width(t)
    if w is None:
        return None
    t = _strip(t)
    if isinstance(t, _UNSIGNED_TYPES):
        return (1 << (8 * w)) - 1
    return (1 << (8 * w - 1)) - 1


def is_unsigned_type(t) -> bool:
    return isinstance(_strip(t), _UNSIGNED_TYPES)


def max_value(
    val: tac_ast.Type_val,
    def_of: Callable[[tac_ast.Var], object],
    symbols,
    ssa_dsts: set[str] | None,
) -> int | None:
    """The largest value `val` can hold, when it is never negative, or
    None. `def_of` returns the instruction that defines a Var, and is
    consulted only for the names in `ssa_dsts`, which have one
    definition. See the module docstring for the rules."""
    return _max(val, def_of, symbols, ssa_dsts or set(), 0, set())


def _var_type(v: tac_ast.Var, symbols):
    if symbols is None:
        return None
    sym = symbols.get(v.name)
    return None if sym is None else sym.type


def _max(val, def_of, symbols, ssa, depth, visiting) -> int | None:
    if isinstance(val, tac_ast.Constant):
        c = val.const
        if isinstance(c, _INT_CONSTS) and c.value >= 0:
            return c.value
        return None
    if not isinstance(val, tac_ast.Var):
        return None
    t = _var_type(val, symbols)
    if t is None or int_width(t) is None:
        return None
    by_type = type_max(t) if is_unsigned_type(t) else None
    if (val.name not in ssa or depth >= _MAX_DEPTH
            or val.name in visiting):
        return by_type
    ins = def_of(val)
    if ins is None:
        return by_type
    visiting = visiting | {val.name}
    by_def = _of_def(ins, t, def_of, symbols, ssa, depth + 1, visiting)
    if by_def is None:
        return by_type
    if by_type is None:
        return by_def
    return min(by_def, by_type)


def _fits(value: int | None, t) -> int | None:
    """`value` when the type `t` holds it, else None."""
    if value is None:
        return None
    top = type_max(t)
    if top is None or value > top:
        return None
    return value


def _of_def(ins, t, def_of, symbols, ssa, depth, visiting) -> int | None:
    def m(v):
        return _max(v, def_of, symbols, ssa, depth, visiting)

    match ins:
        case tac_ast.ZeroExtend(src=s):
            st = _var_type(s, symbols) if isinstance(s, tac_ast.Var) else None
            w = int_width(st) if st is not None else None
            top = (1 << (8 * w)) - 1 if w is not None else None
            inner = m(s)
            if inner is None:
                return top
            return inner if top is None else min(inner, top)
        case tac_ast.SignExtend(src=s) | tac_ast.Copy(src=s) \
                | tac_ast.Truncate(src=s):
            return _fits(m(s), t)
        case tac_ast.Binary(op=op, src1=a, src2=b):
            return _of_binary(op, a, b, t, m)
        case tac_ast.Phi(args=args):
            bounds = [m(arg.source) for arg in args]
            if not bounds or any(x is None for x in bounds):
                return None
            return _fits(max(bounds), t)
    return None


def _const_value(v) -> int | None:
    if isinstance(v, tac_ast.Constant) and isinstance(v.const, _INT_CONSTS):
        return v.const.value
    return None


def _of_binary(op, a, b, t, m) -> int | None:
    match op:
        case tac_ast.BitwiseAnd():
            bounds = [x for x in (m(a), m(b)) if x is not None]
            return min(bounds) if bounds else None
        case tac_ast.BitwiseOr() | tac_ast.BitwiseXor():
            ma, mb = m(a), m(b)
            if ma is None or mb is None:
                return None
            return _fits((1 << max(ma, mb).bit_length()) - 1, t)
        case tac_ast.Add():
            ma, mb = m(a), m(b)
            if ma is None or mb is None:
                return None
            return _fits(ma + mb, t)
        case tac_ast.Multiply():
            ma, mb = m(a), m(b)
            if ma is None or mb is None:
                return None
            return _fits(ma * mb, t)
        case tac_ast.LeftShift():
            ma, k = m(a), _const_value(b)
            if ma is None or k is None or not 0 <= k < 64:
                return None
            return _fits(ma << k, t)
        case tac_ast.RightShift():
            ma, k = m(a), _const_value(b)
            if ma is None:
                return None
            if k is not None and k >= 0:
                return ma >> k
            return ma
        case tac_ast.Divide():
            ma, k = m(a), _const_value(b)
            if ma is None:
                return None
            if k is not None and k > 0:
                return ma // k
            if m(b) is not None:
                return ma
            return None
        case tac_ast.Modulo():
            ma, k = m(a), _const_value(b)
            if ma is None:
                return None
            if k is not None and k > 0:
                return min(ma, k - 1)
            if m(b) is not None:
                return ma
            return None
    return None
