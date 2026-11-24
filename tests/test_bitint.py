"""Tests for C23's bit-precise integer types, `_BitInt(N)` and
`unsigned _BitInt(N)` (N3220 §6.2.5p5 and p8, §6.2.6.2, §6.3.1.1,
§6.3.1.3, §6.3.1.8, §6.4.4.2, §6.7.3.1, §5.2.5.3.2; bit_precise.py),
in the front end and the translation to TAC. tests/test_bitint_sim.py
runs every operator on every width in the simulator.

Coverage:

  * `_BitInt(N)`, `signed _BitInt(N)`, and `unsigned _BitInt(N)` parse
    with N any integer constant expression, and the errors for N
    below 2 (signed) or 1 (unsigned), above BITINT_MAXWIDTH, not
    constant, or not an integer, and for another type specifier with
    `_BitInt`.
  * `wb` and `uwb` constants, in every letter case and order, with
    digit separators and every base, take the least width that holds
    them (with the sign bit for `wb`), and `#if` reads them as
    `intmax_t` and `uintmax_t`.
  * The type rules: no integer promotion, the rank by width with a
    standard type above a bit-precise one of the same width, the usual
    arithmetic conversions for mixed operands, the results of unary
    operators, shifts, comparisons, and `?:`, and `sizeof`.
  * Constant expressions wrap an unsigned type at N bits, and static
    initializers convert to N bits.
  * BITINT_MAXWIDTH is 64 in <limits.h>.
  * The TAC restores the padding bits after `+` on a type with padding
    and leaves a type without padding alone, and the back end sees
    only whole bytes.
  * The type-generic <stdbit.h> functions take `unsigned _BitInt(N)` of
    8, 16, 32, and 64 bits, and the packed BCD builtins reject a
    bit-precise operand.
  * A program with switch, arrays, struct members, pointers, static
    initializers, and calls on bit-precise values runs to the same
    result in both pipelines.
"""
from __future__ import annotations

import shutil
import unittest

from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestConstants(unittest.TestCase):

    def test_preprocessor_if(self) -> None:
        src = (
            "#if 1wb && -1wb < 0 && -1uwb > 0 && 0xFFuwb == 255\n"
            "int yes;\n#else\nint no;\n#endif\n"
        )
        self.assertIn("int yes;", preprocess(src))

    def test_preprocessor_if_too_wide(self) -> None:
        from preprocessor import PreprocessorError
        with self.assertRaises(PreprocessorError) as cm:
            preprocess("#if 18446744073709551616uwb\n#endif\n")
        self.assertIn("needs a unsigned _BitInt(65)", str(cm.exception))


_PROGRAM = r"""
#include <stdbit.h>
typedef unsigned _BitInt(24) u24;
typedef _BitInt(12) s12;

struct rec { unsigned char tag; s12 x; u24 big; _BitInt(5) small; };
struct rec recs[3] = {{1, -5, 0xABCDEF, -16}, {2, 2047, 7, 15}};
static const _BitInt(40) table[4] = {1, -2, 0x7FFFFFFFFFwb, -0x8000000000wb};
u24 counter = 0xFFFFFE;
_BitInt(5) small = 100;
long results[16];

u24 add24(u24 a, u24 b) { return a + b; }
s12 twice(s12 *p) { *p *= 2; return *p; }
_BitInt(40) pick(_BitInt(3) i) { return table[i & 3]; }

int classify(_BitInt(5) v) {
    switch (v) {
    case -16: return 1;
    case 15: return 2;
    case 0: return 3;
    default: return 4;
    }
}

void more(long *r);

int main(void) {
    long *r = results;
    s12 x = recs[0].x;
    *r++ = add24(counter, 3);
    counter++;
    *r++ = counter;
    *r++ = twice(&x);
    *r++ = twice(&recs[1].x);
    *r++ = pick(2);
    *r++ = (long)(pick(3) >> 8);
    *r++ = classify(recs[0].small) * 10 + classify(recs[1].small);
    *r++ = classify(small) * 10 + classify(small - 4);
    more(r);
    return 0;
}

void more(long *r) {
    *r++ = recs[0].big >> 4;
    *r++ = (unsigned _BitInt(4))(recs[0].tag + 20);
    *r++ = stdc_bit_width((unsigned _BitInt(16))1000);
    *r++ = sizeof(struct rec);
    *r++ = (u24)-1 > 0;
    *r++ = (s12)-1 < 0;
    *r++ = (_BitInt(20))(double)-123456.75;
    *r++ = (long)(float)(u24)0x123456;
}
"""

_EXPECTED = [
    1, 0xFFFFFF, -10, -2, 0x7FFFFFFFFF & 0xFFFFFFFF,
    (-0x8000000000 >> 8) & 0xFFFFFFFF, 12, 43, 0xABCDE, 5, 10, 7, 1, 1,
    -123456, 0x123456,
]


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestProgram(unittest.TestCase):
    def test_program(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                addr = sim.symbols["results"]
                mem = sim.mpu.memory
                got = [
                    int.from_bytes(bytes(mem[addr + 4 * k:addr + 4 * k + 4]),
                                   "little", signed=True)
                    for k in range(len(_EXPECTED))
                ]
                want = [v - (1 << 32) if v >= 1 << 31 else v
                        for v in (e & 0xFFFFFFFF for e in _EXPECTED)]
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
