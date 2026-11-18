"""C23's bit-precise integer types, `_BitInt(N)` and `unsigned
_BitInt(N)` (N3220 §6.2.5p5 and p8, §6.7.3.1), and the facts about
them that every stage shares.

A `_BitInt(N)` has N bits, the sign bit included, and an `unsigned
_BitInt(N)` has N value bits. c6502 takes N from 2 (signed) or 1
(unsigned) up to `BITINT_MAXWIDTH`, which is 64, the least the
standard allows (§5.2.5.3.2), because the runtime's helpers for
multiply, divide, shifts, and conversions to and from floating types
stop at 8 bytes.

A value of either type takes `storage_bytes(N)` bytes, ceil(N/8),
stored low byte first like every other integer. When N isn't a
multiple of 8, the top `8 * storage_bytes(N) - N` bits of the top
byte are padding bits, whose values the standard leaves unspecified
(§6.2.6.2p1 and p3). c6502 keeps them equal to the value's sign
extension for a signed type and to zero for an unsigned one. The
bytes then hold the value as an ordinary integer of the storage
width, so comparisons, right shifts, division, and conversions to
other types work on them unchanged. `c99_to_tac` restores the
padding after each operation that can change it (`+`, `-`, `*`, `<<`,
unary `-`, `~` of an unsigned value, a signed `/`, and a conversion
into the type from one with values it can't hold) by masking for an
unsigned type and by `((x & m) ^ s) - s` for a signed one, where `m`
covers the N bits and `s` is the sign bit. `&`, `|`, `^`, `%`, `>>`,
and an unsigned `/` of two values whose padding is right give a
result whose padding is right. Overflow of a signed bit-precise type
is undefined, and c6502 wraps it modulo 2^N, as it does a conversion
of an out-of-range value (implementation-defined by §6.3.1.3p3).

After translation to TAC, `c99_to_tac` replaces each bit-precise type
by the integer type of its storage (`storage_type`): a standard type
for 1, 2, 4, and 8 bytes, and `BitInt(8 * bytes)` for 3, 5, 6, and 7
bytes. The back end therefore sees only whole bytes, and the TAC
constants of the odd sizes are `ConstInt24` to `ConstUInt56`.
"""
from __future__ import annotations

import c99_ast
import tac_ast

# The widest N of `_BitInt(N)` (C23 §5.2.5.3.2), which <limits.h>
# defines as BITINT_MAXWIDTH.
BITINT_MAXWIDTH = 64

# The TAC constant class of each odd byte count, keyed by `(bytes,
# signed)`.
ODD_CONST_CLASSES: dict[tuple[int, bool], type] = {
    (3, True): tac_ast.ConstInt24, (3, False): tac_ast.ConstUInt24,
    (5, True): tac_ast.ConstInt40, (5, False): tac_ast.ConstUInt40,
    (6, True): tac_ast.ConstInt48, (6, False): tac_ast.ConstUInt48,
    (7, True): tac_ast.ConstInt56, (7, False): tac_ast.ConstUInt56,
}

# `(bytes, signed)` of each of those classes.
ODD_CONST_LAYOUT: dict[type, tuple[int, bool]] = {
    cls: key for key, cls in ODD_CONST_CLASSES.items()
}

# `(bytes, signed)` of every TAC integer constant class.
INTEGER_CONST_LAYOUT: dict[type, tuple[int, bool]] = {
    tac_ast.ConstChar: (1, True), tac_ast.ConstUChar: (1, False),
    tac_ast.ConstInt: (2, True), tac_ast.ConstUInt: (2, False),
    tac_ast.ConstLong: (4, True), tac_ast.ConstULong: (4, False),
    tac_ast.ConstLongLong: (8, True), tac_ast.ConstULongLong: (8, False),
    **ODD_CONST_LAYOUT,
}

# The TAC integer constant class of each `(bytes, signed)`.
INTEGER_CONST_CLASSES: dict[tuple[int, bool], type] = {
    key: cls for cls, key in INTEGER_CONST_LAYOUT.items()
}

ODD_CONSTS: tuple[type, ...] = tuple(ODD_CONST_LAYOUT)
ODD_UNSIGNED_CONSTS: tuple[type, ...] = tuple(
    cls for cls, (_, signed) in ODD_CONST_LAYOUT.items() if not signed
)
ODD_SIGNED_CONSTS: tuple[type, ...] = tuple(
    cls for cls, (_, signed) in ODD_CONST_LAYOUT.items() if signed
)


def storage_bytes(width: int) -> int:
    """The bytes a value of `_BitInt(width)` occupies, ceil(width/8)."""
    return (width + 7) // 8


def min_width(signed: bool) -> int:
    """The least N of `_BitInt(N)` (2) or `unsigned _BitInt(N)` (1),
    by C23 §6.7.3.1p4."""
    return 2 if signed else 1


def wrap(value: int, width: int, signed: bool) -> int:
    """`value` modulo 2^width, read as a signed or unsigned value of
    that many bits."""
    value &= (1 << width) - 1
    if signed and value >> (width - 1):
        value -= 1 << width
    return value


def value_range(width: int, signed: bool) -> tuple[int, int]:
    """The least and greatest values of the type."""
    if signed:
        return -(1 << (width - 1)), (1 << (width - 1)) - 1
    return 0, (1 << width) - 1


def constant_width(value: int, signed: bool) -> int:
    """The width N of the type of the constant `value` with a `wb`
    (signed) or `uwb` suffix: the smallest N greater than 1 that holds
    the value and the sign bit, or the smallest N greater than 0 that
    holds the value (C23 §6.4.4.2p6). `value` isn't negative."""
    if signed:
        return max(2, value.bit_length() + 1)
    return max(1, value.bit_length())


def is_bit_precise(t) -> bool:
    """True iff `t`, without its qualifiers, is a bit-precise type."""
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, c99_ast.BitInt)


def has_padding(t: c99_ast.BitInt) -> bool:
    """True iff the top byte of `t` holds padding bits."""
    return t.width % 8 != 0


def storage_type(t: c99_ast.BitInt) -> c99_ast.Type_data_type:
    """The integer type of `t`'s storage, which the back end handles:
    the standard type of the same size and signedness for 1, 2, 4,
    and 8 bytes, and `BitInt(8 * bytes)` for the other sizes."""
    n = storage_bytes(t.width)
    s = t.is_signed
    match n:
        case 1:
            return c99_ast.SChar() if s else c99_ast.UChar()
        case 2:
            return c99_ast.Int() if s else c99_ast.UInt()
        case 4:
            return c99_ast.Long() if s else c99_ast.ULong()
        case 8:
            return c99_ast.LongLong() if s else c99_ast.ULongLong()
    return c99_ast.BitInt(width=8 * n, is_signed=s)


def odd_bytes(t) -> int | None:
    """The byte count of `t` when it's a bit-precise type (in the back
    end, one of 3, 5, 6, or 7 bytes), else None. Qualifiers are
    transparent."""
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    if isinstance(t, c99_ast.BitInt):
        return storage_bytes(t.width)
    return None


def const_value(c) -> int:
    """The value of the TAC integer constant `c`, read at its class's
    width and signedness."""
    n, signed = INTEGER_CONST_LAYOUT[type(c)]
    return wrap(c.value, 8 * n, signed)


def make_const(n: int, signed: bool, value: int):
    """The TAC integer constant of `n` bytes and that signedness
    holding `value` modulo 2^(8n) as its bit pattern, which isn't
    negative. That's the form the type checker gives its constants,
    and some 1-byte lowerings put a constant's value straight into an
    immediate operand."""
    return INTEGER_CONST_CLASSES[(n, signed)](
        value=value & ((1 << (8 * n)) - 1),
    )


def helper_bytes(n: int) -> int:
    """The operand width of the runtime helper that a multiply, divide,
    remainder, shift by a variable, or conversion to or from a floating
    type of an `n`-byte integer uses: the least of 1, 2, 4, and 8 that
    is at least `n`."""
    for w in (1, 2, 4, 8):
        if w >= n:
            return w
    raise ValueError(f"no runtime helper takes {n}-byte operands")
