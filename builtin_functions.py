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

The two builtins so far do packed BCD arithmetic with the 6502's
decimal mode (docs/builtins.md).

  `__c6502_bcd_add(a, b)` is `a + b` in packed BCD.
  `__c6502_bcd_sub(a, b)` is `a - b` in packed BCD.

Packed BCD holds two decimal digits in each byte, the higher digit
in the high nibble, and a multi-byte value keeps its low byte first
like any c6502 integer, so 0x0250 is 250. Each builtin lowers to
TAC `Binary(BcdAdd)` or `Binary(BcdSubtract)`, and tac_to_asm emits
one asm `DecimalArith` for it, which `asm_to_asm2` expands into
`SED; CLC; LDA; ADC; STA; ...; CLD`.

`adc_byte` and `sbc_byte` copy py65's `opADC` and `opSBC` in decimal
mode (py65/devices/mpu6502.py), so constant folding, tac_sim, and
the simulator give the same result for every pair of bytes, digits
that aren't 0-9 included. For those, py65's accumulator and carry
match the NMOS 6502's algorithm in 6502.org's "Decimal Mode" tutorial
(Appendix A). That comes from comparing the two algorithms, and it
hasn't been checked on the hardware.
"""
from __future__ import annotations

BCD_ADD = "__c6502_bcd_add"
BCD_SUB = "__c6502_bcd_sub"

# Each builtin's name and the number of arguments it takes.
BUILTINS: dict[str, int] = {
    BCD_ADD: 2,
    BCD_SUB: 2,
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
