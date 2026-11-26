"""Simulates the inline lowerings of byte division, variable shifts,
and the 8-by-8 product against Python, in both pipelines.

`tac_to_asm` lowers an unsigned byte divided by a constant as a loop of
subtractions (`_inline_byte_divide`), a 1- or 2-byte shift by a
variable count, or by a constant count that `inline_shift_ok` rejects,
as a loop of one-bit shifts (`_shift_loop`), and a 16-bit product of
two bytes as a shift-and-add loop (`_inline_byte_product`). Under -O,
`narrow_byte_range` makes the divides, remainders, right shifts, and
products of promoted bytes into those forms, and strength reduction
makes a signed divide or remainder of a nonnegative value by a power
of two a shift or a mask.

Each case is a function of one or two arguments whose results a `main`
stores into an array, for every byte value of the first argument (or
a spread of 16-bit values) and a list of values of the second. A
count of the width or more is included, and the expected result for
it is the shift helpers', 0 for a left or logical shift and every bit
a copy of the sign for an arithmetic one, which C leaves undefined. An
unsigned `_BitInt(8)` isn't promoted, so it reaches the byte lowerings
in the unoptimized pipeline too.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6)."""

from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp

_BYTES = list(range(256))
# 16-bit values with every byte pattern that matters for a shift: the
# sign bit, carries between the bytes, and both bytes nonzero.
_WORDS = sorted({
    0, 1, 2, 3, 0x7F, 0x80, 0xFF, 0x100, 0x101, 0x1234, 0x4000, 0x7FFF,
    0x8000, 0x8001, 0xA5C3, 0xC000, 0xFF00, 0xFFFE, 0xFFFF,
    *((k * 0x9E37 + 0x79B9) & 0xFFFF for k in range(45)),
})


def _wrap(v: int, bits: int) -> int:
    return v & ((1 << bits) - 1)


def _signed(v: int, bits: int) -> int:
    v = _wrap(v, bits)
    return v - (1 << bits) if v >> (bits - 1) else v


def _shl(v, n, bits):
    return 0 if n >= bits else _wrap(v << n, bits)


def _lsr(v, n, bits):
    return 0 if n >= bits else _wrap(v, bits) >> n


def _asr(v, n, bits):
    return _wrap(_signed(v, bits) >> min(n, bits - 1), bits)


# (name, C declaration of the function, the result's bytes, the first
# argument's values, the second's (or None), the Python model)
_CASES = []


def _case(name, decl, size, xs, ys, model):
    _CASES.append((name, decl, size, xs, ys, model))


for _c in (1, 2, 3, 5, 7, 8, 10, 20, 37, 100, 128, 255):
    _case(f"udiv{_c}", f"uint8_t f(uint8_t x) {{ return x / {_c}; }}",
          1, _BYTES, None, lambda x, c=_c: x // c)
    _case(f"umod{_c}", f"uint8_t f(uint8_t x) {{ return x % {_c}; }}",
          1, _BYTES, None, lambda x, c=_c: x % c)
for _c in (3, 10, 20):
    # A quotient and a remainder of the same operands share one loop.
    _case(f"udivmod{_c}",
          f"uint16_t f(uint8_t x) {{ uint8_t q = x / {_c}; "
          f"uint8_t r = x % {_c}; return r | (uint16_t)q << 8; }}",
          2, _BYTES, None, lambda x, c=_c: (x % c) | (x // c) << 8)
    _case(f"bdiv{_c}",
          f"unsigned _BitInt(8) f(unsigned _BitInt(8) x) {{ "
          f"return x / {_c}uwb; }}",
          1, _BYTES, None, lambda x, c=_c: x // c)
    _case(f"bmod{_c}",
          f"unsigned _BitInt(8) f(unsigned _BitInt(8) x) {{ "
          f"return x % {_c}uwb; }}",
          1, _BYTES, None, lambda x, c=_c: x % c)
_case("udiv_var", "uint8_t f(uint8_t x, uint8_t y) { return x / y; }",
      1, _BYTES, [1, 2, 3, 7, 16, 200, 255], lambda x, y: x // y)
_case("umod_var", "uint8_t f(uint8_t x, uint8_t y) { return x % y; }",
      1, _BYTES, [1, 2, 3, 7, 16, 200, 255], lambda x, y: x % y)

# Signed divides of a nonnegative dividend.
_case("half_up", "uint8_t f(uint8_t s) { return (s + 1) / 2; }",
      1, _BYTES, None, lambda s: (s + 1) // 2)
_case("mod_after_add", "int f(uint8_t s) { return (s + 3) % 4; }",
      2, _BYTES, None, lambda s: (s + 3) % 4)
_case("div_wide", "int f(uint8_t s) { return (s + 100) / 16; }",
      2, _BYTES, None, lambda s: (s + 100) // 16)

_COUNTS8 = list(range(11))
_COUNTS_PROMOTED = list(range(16))
_COUNTS16 = list(range(18))

_case("shl_u8", "uint8_t f(uint8_t x, uint8_t n) { return x << n; }",
      1, _BYTES, _COUNTS_PROMOTED, lambda x, n: _shl(x, n, 8))
_case("shr_u8", "uint8_t f(uint8_t x, uint8_t n) { return x >> n; }",
      1, _BYTES, _COUNTS_PROMOTED, lambda x, n: _lsr(x, n, 8))
_case("shl_b8",
      "unsigned _BitInt(8) f(unsigned _BitInt(8) x, uint8_t n) "
      "{ return x << n; }",
      1, _BYTES, _COUNTS8, lambda x, n: _shl(x, n, 8))
_case("lsr_b8",
      "unsigned _BitInt(8) f(unsigned _BitInt(8) x, uint8_t n) "
      "{ return x >> n; }",
      1, _BYTES, _COUNTS8, lambda x, n: _lsr(x, n, 8))
_case("asr_b8",
      "_BitInt(8) f(_BitInt(8) x, uint8_t n) { return x >> n; }",
      1, _BYTES, _COUNTS8, lambda x, n: _asr(x, n, 8))
_case("asr_b8_3", "_BitInt(8) f(_BitInt(8) x) { return x >> 3; }",
      1, _BYTES, None, lambda x: _asr(x, 3, 8))
for _k in (2, 3, 4, 6):
    # A promoted `signed char` shifted and truncated narrows to a byte
    # shift unless only the wide one is inline.
    _case(f"asr_s8_{_k}",
          f"int8_t f(int8_t x) {{ return x >> {_k}; }}",
          1, _BYTES, None, lambda x, k=_k: _asr(x, k, 8))
_case("shl_int_wide",
      "unsigned int f(uint8_t x, uint8_t n) { return x << n; }",
      2, _BYTES, _COUNTS_PROMOTED, lambda x, n: _shl(x, n, 16))
_case("shl_u16",
      "unsigned int f(unsigned int x, uint8_t n) { return x << n; }",
      2, _WORDS, _COUNTS16, lambda x, n: _shl(x, n, 16))
_case("lsr_u16",
      "unsigned int f(unsigned int x, uint8_t n) { return x >> n; }",
      2, _WORDS, _COUNTS16, lambda x, n: _lsr(x, n, 16))
_case("asr_i16", "int f(int x, uint8_t n) { return x >> n; }",
      2, _WORDS, _COUNTS16, lambda x, n: _asr(x, n, 16))
_case("asr_i16_5", "int f(int x) { return x >> 5; }",
      2, _WORDS, None, lambda x: _asr(x, 5, 16))
_case("lsr_u16_6", "unsigned int f(unsigned int x) { return x >> 6; }",
      2, _WORDS, None, lambda x: _lsr(x, 6, 16))
_case("shl_u16_6", "unsigned int f(unsigned int x) { return x << 6; }",
      2, _WORDS, None, lambda x: _shl(x, 6, 16))
_case("mask_shift",
      "uint8_t f(uint8_t n) { return (0x7F >> n) << n; }",
      1, list(range(10)), None,
      lambda n: _wrap((0x7F >> n) << n, 8))

# A left shift whose result stays below 256 is a byte shift
# (`narrow_byte_range`), as in Ali Baba's `hrcg_row_address`.
_case("row_address",
      "uint16_t f(uint8_t row) "
      "{ return 0x80 * (row % 8) + 0x28 * (row / 8); }",
      2, _BYTES, None, lambda r: 0x80 * (r % 8) + 0x28 * (r // 8))
_case("shift_back", "unsigned int f(uint8_t b) { return (b >> 3) << 3; }",
      2, _BYTES, None, lambda b: (b >> 3) << 3)

_MULTIPLIERS = [0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 100, 127, 128, 200, 254,
                255]
_case("mul8x8", "uint16_t f(uint8_t a, uint8_t b) { return a * b; }",
      2, _BYTES, _MULTIPLIERS, lambda a, b: a * b)
_case("mul_tier",
      "uint16_t f(uint8_t p, uint8_t s) "
      "{ return p * (((s >> 5) & 3) + 1); }",
      2, _BYTES, [0x00, 0x1F, 0x20, 0x45, 0x60, 0x9F, 0xE0, 0xFF],
      lambda p, s: p * (((s >> 5) & 3) + 1))

# A 16-bit quotient and remainder of the same operands share one
# helper call.
_DIVISORS16 = [1, 3, 10, 255, 256, 1000, 0x7FFF, 0x8000, 0xFFFF]
_case("udivmod16",
      "uint16_t q; uint16_t r;\n"
      "uint16_t f(uint16_t v, uint16_t p) "
      "{ q = v / p; v %= p; return q ^ v; }",
      2, _WORDS, _DIVISORS16, lambda v, p: (v // p) ^ (v % p))


def _program(decl: str, size: int, xs, ys) -> str:
    rtype = {1: "uint8_t", 2: "uint16_t"}[size]
    big = max(xs) > 0xFF
    xtype = "uint16_t" if big else "uint8_t"
    lines = ["#include <stdint.h>", decl,
             f"static const {xtype} XS[{len(xs)}] = {{"
             + ", ".join(map(str, xs)) + "};"]
    if ys is None:
        lines.append(f"{rtype} OUT[{len(xs)}];")
        body = (f"    for (uint16_t i = 0; i < {len(xs)}; i++)\n"
                "        OUT[i] = f(XS[i]);\n")
    else:
        ytype = "uint16_t" if max(ys) > 0xFF else "uint8_t"
        lines.append(f"static const {ytype} YS[{len(ys)}] = {{"
                     + ", ".join(map(str, ys)) + "};")
        lines.append(f"{rtype} OUT[{len(xs) * len(ys)}];")
        body = (f"    uint16_t k = 0;\n"
                f"    for (uint16_t i = 0; i < {len(xs)}; i++)\n"
                f"        for (uint8_t j = 0; j < {len(ys)}; j++)\n"
                "            OUT[k++] = f(XS[i], YS[j]);\n")
    lines.append("int main(void) {\n" + body + "    return 0;\n}")
    return "\n".join(lines) + "\n"


@needs_c6502cpp
class TestByteArith(unittest.TestCase):

    def _check(self, optimize: bool) -> None:
        for name, decl, size, xs, ys, model in _CASES:
            with self.subTest(case=name):
                src = _program(decl, size, xs, ys)
                sim = build_sim(src, optimize=optimize)
                result = sim.run(max_cycles=40_000_000)
                self.assertFalse(result.timed_out)
                base = sim.symbols["OUT"]
                mem = sim.mpu.memory
                pairs = ([(x, None) for x in xs] if ys is None
                         else [(x, y) for x in xs for y in ys])
                for k, (x, y) in enumerate(pairs):
                    got = int.from_bytes(
                        bytes(mem[base + k * size:base + (k + 1) * size]),
                        "little")
                    want = _wrap(model(x) if y is None else model(x, y),
                                 8 * size)
                    if got != want:
                        self.fail(f"{name}({x}, {y}): got {got:#x}, "
                                  f"want {want:#x}")

    def test_unoptimized(self) -> None:
        self._check(optimize=False)

    def test_optimized(self) -> None:
        self._check(optimize=True)


if __name__ == "__main__":
    unittest.main()
