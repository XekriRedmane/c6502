"""The compiler's builtin functions, and the arithmetic behind them.

A builtin is called like a function, but no function is defined for
it. The front end recognizes the name, the type checker types the
call by the builtin's own rules, and c99_to_tac lowers it to a TAC
instruction. The names are in the implementation's reserved
namespace (C99 §7.1.3, an identifier beginning with two
underscores), so they can't collide with a program's own names.

Adding a builtin means adding its name to `BUILTINS`, a case for it
in `type_checking._check_builtin_call`, and a case in
`c99_to_tac.Translator._translate_builtin`. Every other part of the
front end calls `is_builtin`.

Three builtins work with packed BCD in the 6502's decimal mode
(docs/builtins.md), and the rest are C23's bit and byte utilities,
which <stdbit.h> names (docs/stdbit.md).

  `__c6502_bcd_add(a, b)` is `a + b` in packed BCD.
  `__c6502_bcd_sub(a, b)` is `a - b` in packed BCD.
  `__c6502_bin_to_bcd(x)` is the 16-bit `x` as five packed BCD digits.

Packed BCD holds two decimal digits in each byte, the higher digit
in the high nibble, and a multi-byte value keeps its low byte first
like any c6502 integer, so 0x0250 is 250. Each builtin lowers to
TAC `Binary(BcdAdd)` or `Binary(BcdSubtract)`, and tac_to_asm emits
one asm `DecimalArith` for it, which `asm_to_asm2` expands into
`SED; CLC; LDA; ADC; STA; ...; CLD`. `__c6502_bin_to_bcd` lowers to
TAC `Unary(BinToBcd)`, an `unsigned int` source and an `unsigned long`
destination, which tac_to_asm lowers to a call of the runtime helper
`bin16_to_bcd` (rtlib/helpers.py), and `bin_to_bcd` gives its value
for constant folding.

`adc_byte` and `sbc_byte` copy py65's `opADC` and `opSBC` in decimal
mode (py65/devices/mpu6502.py), so constant folding, tac_sim, and
the simulator give the same result for every pair of bytes, digits
that aren't 0-9 included. For those, py65's accumulator and carry
match the NMOS 6502's algorithm in 6502.org's "Decimal Mode" tutorial
(Appendix A). That comes from comparing the two algorithms, and it
hasn't been checked on the hardware.

The <stdbit.h> builtins are `__c6502_stdc_<operation>` for the
type-generic form of each of C23's 14 operations (§7.18.3 through
§7.18.16), typed by its argument, and `__c6502_stdc_<operation>_uc`,
`_us`, `_ui`, `_ul`, and `_ull` for the forms with a parameter of
`unsigned char` through `unsigned long long`. <stdbit.h> defines each
`stdc_...` name as a macro for its builtin. `STDBIT_BUILTINS` maps each
builtin to its operation and its suffix, and `stdbit` computes an
operation, for constant folding and for the tests' model.
"""
from __future__ import annotations

BCD_ADD = "__c6502_bcd_add"
BCD_SUB = "__c6502_bcd_sub"
BIN_TO_BCD = "__c6502_bin_to_bcd"

# C23's bit and byte utilities (§7.18), in the standard's order.
STDBIT_OPERATIONS = (
    "leading_zeros", "leading_ones", "trailing_zeros", "trailing_ones",
    "first_leading_zero", "first_leading_one",
    "first_trailing_zero", "first_trailing_one",
    "count_zeros", "count_ones", "has_single_bit",
    "bit_width", "bit_floor", "bit_ceil",
)

# The suffix of each function with a parameter of one type, and the
# type, as `type_checking` names it.
STDBIT_SUFFIXES = {
    "uc": "unsigned char", "us": "unsigned short", "ui": "unsigned int",
    "ul": "unsigned long", "ull": "unsigned long long",
}

# Each <stdbit.h> builtin's name, with its operation and its suffix,
# None for the type-generic form.
STDBIT_BUILTINS: dict[str, tuple[str, str | None]] = {}
for _op in STDBIT_OPERATIONS:
    STDBIT_BUILTINS[f"__c6502_stdc_{_op}"] = (_op, None)
    for _suffix in STDBIT_SUFFIXES:
        STDBIT_BUILTINS[f"__c6502_stdc_{_op}_{_suffix}"] = (_op, _suffix)

# The name of each <stdbit.h> function with a parameter of one type,
# `stdc_leading_zeros_uc` and the rest, and its builtin.
STDBIT_FUNCTIONS: dict[str, str] = {
    name[len("__c6502_"):]: name
    for name, (_, suffix) in STDBIT_BUILTINS.items()
    if suffix is not None
}

# Each builtin's name and the number of arguments it takes.
BUILTINS: dict[str, int] = {
    BCD_ADD: 2,
    BCD_SUB: 2,
    BIN_TO_BCD: 1,
    **{name: 1 for name in STDBIT_BUILTINS},
}


def is_builtin(name: str) -> bool:
    return name in BUILTINS


def adc_byte(a: int, b: int, carry: int) -> tuple[int, int]:
    """Returns `(A, C)` after `ADC b` with the decimal flag set, A
    holding `a` and the carry `carry` (0 or 1). This is py65's
    decimal-mode `opADC`."""
    nibble0 = (b & 0xF) + (a & 0xF) + carry
    adjust0 = adjust1 = halfcarry = decimalcarry = 0
    if nibble0 > 9:
        adjust0 = 6
        halfcarry = 1
    nibble1 = ((b >> 4) & 0xF) + ((a >> 4) & 0xF) + halfcarry
    if nibble1 > 9:
        adjust1 = 6
        decimalcarry = 1
    nibble0 = ((nibble0 & 0xF) + adjust0) & 0xF
    nibble1 = ((nibble1 & 0xF) + adjust1) & 0xF
    return (nibble1 << 4) + nibble0, decimalcarry


def sbc_byte(a: int, b: int, carry: int) -> tuple[int, int]:
    """Returns `(A, C)` after `SBC b` with the decimal flag set, A
    holding `a` and the carry `carry` (1 for no borrow). This is
    py65's decimal-mode `opSBC`."""
    adjust0 = adjust1 = 0
    halfcarry = 1
    nibble0 = (a & 0xF) + (~b & 0xF) + carry
    if nibble0 <= 0xF:
        halfcarry = 0
        adjust0 = 10
    nibble1 = ((a >> 4) & 0xF) + ((~b >> 4) & 0xF) + halfcarry
    if nibble1 <= 0xF:
        adjust1 = 10 << 4
    aluresult = a + (~b & 0xFF) + carry
    decimalcarry = int(aluresult > 0xFF)
    aluresult &= 0xFF
    nibble0 = (aluresult + adjust0) & 0xF
    nibble1 = ((aluresult + adjust1) >> 4) & 0xF
    return (nibble1 << 4) + nibble0, decimalcarry


def bcd_add(a: int, b: int, nbytes: int) -> int:
    """`a + b` in packed BCD over `nbytes` bytes, as `CLC` and then an
    `ADC` per byte, low byte first, compute it. The carry out of the
    top byte is dropped."""
    out, carry = 0, 0
    for k in range(nbytes):
        r, carry = adc_byte((a >> 8 * k) & 0xFF, (b >> 8 * k) & 0xFF, carry)
        out |= r << 8 * k
    return out


def bcd_sub(a: int, b: int, nbytes: int) -> int:
    """`a - b` in packed BCD over `nbytes` bytes, as `SEC` and then an
    `SBC` per byte, low byte first, compute it. The borrow out of the
    top byte is dropped."""
    out, carry = 0, 1
    for k in range(nbytes):
        r, carry = sbc_byte((a >> 8 * k) & 0xFF, (b >> 8 * k) & 0xFF, carry)
        out |= r << 8 * k
    return out


def bin_to_bcd(value: int) -> int:
    """Returns the low 16 bits of `value` as five packed BCD digits,
    the ones in bits 0-3 and the ten-thousands in bits 16-19, as the
    runtime helper `bin16_to_bcd` computes them. The result is at most
    0x65535."""
    v = value & 0xFFFF
    out = 0
    for k in range(5):
        out |= (v % 10) << 4 * k
        v //= 10
    return out


def stdbit(operation: str, value: int, width: int) -> int:
    """The result of the <stdbit.h> `operation` (one of
    `STDBIT_OPERATIONS`) on `value` taken as an unsigned integer of
    `width` bits, as C23 §7.18 gives it. `has_single_bit` gives 0 or
    1, and `bit_floor` and `bit_ceil` give a value of the same width.

    The most significant index counts from the most significant bit,
    0, to the least significant bit, `width - 1` (§7.18.1p3), and the
    least significant index counts the other way (§7.18.1p4). Each
    `first_...` result is the index plus 1, or 0 when there's no such
    bit. `bit_ceil` gives 0 when the power of 2 doesn't fit in `width`
    bits (§7.18.16p2), and `bit_ceil(0)` is 1, the smallest power of
    2."""
    mask = (1 << width) - 1
    v = value & mask
    inverse = v ^ mask

    def trailing_zeros(x: int) -> int:
        return width if x == 0 else (x & -x).bit_length() - 1

    match operation:
        case "leading_zeros":
            return width - v.bit_length()
        case "leading_ones":
            return width - inverse.bit_length()
        case "trailing_zeros":
            return trailing_zeros(v)
        case "trailing_ones":
            return trailing_zeros(inverse)
        case "first_leading_zero":
            return 0 if v == mask else width - inverse.bit_length() + 1
        case "first_leading_one":
            return 0 if v == 0 else width - v.bit_length() + 1
        case "first_trailing_zero":
            return 0 if v == mask else trailing_zeros(inverse) + 1
        case "first_trailing_one":
            return 0 if v == 0 else trailing_zeros(v) + 1
        case "count_zeros":
            return width - bin(v).count("1")
        case "count_ones":
            return bin(v).count("1")
        case "has_single_bit":
            return int(bin(v).count("1") == 1)
        case "bit_width":
            return v.bit_length()
        case "bit_floor":
            return 0 if v == 0 else 1 << (v.bit_length() - 1)
        case "bit_ceil":
            return 1 if v <= 1 else (1 << (v - 1).bit_length()) & mask
    raise ValueError(f"unknown <stdbit.h> operation {operation!r}")
