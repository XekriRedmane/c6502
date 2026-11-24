"""Tests for compares that branch on flags another instruction set,
or that fold into fewer tests.

- `CMP #$80; BCS` is `BMI` and `CMP #1; BCC` is `BEQ` after a load
  (`redundant_compare._compares_as_flags`), and so is `SEC; SBC #1;
  BCC` when A is dead.
- A signed byte compare of a value a `BPL` showed nonnegative drops
  its overflow test (`redundant_compare._signed_compare_of_nonnegative`).
- A signed ordering compare of two values that can't be negative is
  unsigned (`nonnegative_compare.NONNEGATIVE_COMPARE_RULE`).
- `e == 0 || e == 1` is one test of `e & ~1`
  (`short_circuit_jump_fold.merge_equality_jumps`).
- `!b` of a `bool` is `b ^ 1` (`lnot_jump_fold.LNOT_OF_ZERO_OR_ONE_RULE`),
  and `(c ? 3 : 2) + 1` joins 4 and 3 (`phi_constant_fold`).
- An unsigned `e > 0xFF` is `e >= 0x100`, a test of the high byte
  (`tac_to_asm._variable_first`).

Each compiled shape is checked, and each program runs in the
simulator in both pipelines against values computed in Python.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest

from tests.test_register_arguments import _body, _codegen
from sim.harness import build_sim


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestCompiled(unittest.TestCase):
    def test_bit_7_compare(self) -> None:
        src = ("#include <stdint.h>\n"
               "extern volatile uint8_t KBD [[c6502::address(0xC000)]];"
               "uint8_t k(void) { uint8_t c; if ((c = KBD) < 0x80) return 0;"
               " return c; }")
        self.assertEqual(_body(_codegen(src), "k")[:2],
                         ["LDA KBD", "BMI .if_end@0"])

    def test_two_values_one_test(self) -> None:
        src = ("#include <stdint.h>\n"
               "uint16_t find(void); void draw(void);"
               "void f(void) { uint16_t e = find();"
               " if (e == 0 || e == 1) return; draw(); }")
        body = _body(_codegen(src), "f")
        self.assertEqual(body[1], "AND #$FE")
        self.assertNotIn("CMP #$01", body)

    def test_not_of_bool(self) -> None:
        src = ("#include <stdint.h>\n"
               "static bool flag;"
               "extern uint8_t A [[c6502::address(0x5A00)]];"
               "extern uint8_t B [[c6502::address(0x5A01)]];"
               "void f(void) { A = !flag; B = (flag ? 3 : 2) + 1; }")
        body = _body(_codegen(src), "f")
        self.assertEqual(body[:3], ["LDA flag", "EOR #$01", "STA A"])
        self.assertIn("LDA #$04", body)
        self.assertNotIn("ADC #$01", body)

    def test_byte_plus_one_unsigned(self) -> None:
        src = ("#include <stdint.h>\n"
               "extern uint8_t W [[c6502::zeropage(0x23)]];"
               "void copy(uint8_t row);"
               "void f(uint8_t top) { for (uint8_t r = top; r + 1 < W; r++)"
               " copy(r); }")
        body = _body(_codegen(src), "f")
        self.assertNotIn("EOR #$80", body)

    def test_high_byte_test(self) -> None:
        src = ("#include <stdint.h>\n"
               "uint16_t find(void);"
               "uint8_t g(void) { uint16_t e = find();"
               " if (e > 0xFF) return 1; return 2; }")
        body = _body(_codegen(src), "g")
        self.assertEqual(body[:3], ["JSR find", "TXA", "BEQ .if_end@0"])


_PROGRAM = r"""
#include <stdint.h>
uint8_t checks[17];
static bool flag;
uint8_t A, B;
uint8_t bit7(uint8_t v) { if (v < 0x80) return 1; return 2; }
uint8_t one(uint8_t v) { if (v >= 1) return 3; return 4; }
uint8_t pos(int8_t c) { if (c < 0) return 5; if (c > 0x1C) return 6; return 7; }
uint8_t rows(uint8_t top, uint8_t btm) {
    uint8_t n = 0;
    for (uint8_t r = top; r + 1 < btm; r++) n++;
    return n;
}
uint8_t pair(uint16_t e) { if (e == 0 || e == 1) return 8; return 9; }
uint8_t hi(uint16_t e) {
    if (e & 0x8000) return 1;
    if (!(e > 0xFF && !(e & 0x8000))) return 2;
    return 3;
}
void nots(void) { A = !flag; B = (flag ? 3 : 2) + 1; }
int main(void) {
    uint16_t sum = 0;
    for (uint16_t v = 0; v < 256; v++) {
        sum = sum * 3 + bit7((uint8_t)v);
        sum = sum * 3 + one((uint8_t)v);
        sum = sum * 3 + pos((int8_t)v);
    }
    checks[0] = rows(0, 0); checks[1] = rows(0, 1); checks[2] = rows(0, 5);
    checks[3] = rows(3, 255); checks[4] = rows(254, 255);
    checks[5] = rows(255, 255); checks[6] = rows(250, 2);
    checks[7] = pair(0); checks[8] = pair(1); checks[9] = pair(2);
    checks[10] = pair(0x100) + pair(0x101) * 16;
    checks[11] = hi(0) + hi(0xFF) * 4 + hi(0x100) * 16;
    checks[12] = hi(0x8000) + hi(0xFFFF) * 4 + hi(0x7FFF) * 16;
    flag = false; nots(); checks[13] = A * 16 + B;
    flag = true; nots(); checks[14] = A * 16 + B;
    checks[15] = (uint8_t)sum; checks[16] = sum >> 8;
    return 0;
}
"""


def _expected():
    """The values `_PROGRAM` computes, in Python."""
    s = 0
    for v in range(256):
        c = v - 256 if v & 0x80 else v
        s = (s * 3 + (1 if v < 0x80 else 2)) & 0xFFFF
        s = (s * 3 + (3 if v >= 1 else 4)) & 0xFFFF
        s = (s * 3 + (5 if c < 0 else 6 if c > 0x1C else 7)) & 0xFFFF

    def rows(top, btm):
        n, r = 0, top
        while r + 1 < btm:
            n, r = n + 1, (r + 1) & 0xFF
        return n & 0xFF

    def pair(e):
        return 8 if e in (0, 1) else 9

    def hi(e):
        if e & 0x8000:
            return 1
        return 2 if not (e > 0xFF) else 3

    checks = [
        rows(0, 0), rows(0, 1), rows(0, 5), rows(3, 255), rows(254, 255),
        rows(255, 255), rows(250, 2), pair(0), pair(1), pair(2),
        (pair(0x100) + pair(0x101) * 16) & 0xFF,
        (hi(0) + hi(0xFF) * 4 + hi(0x100) * 16) & 0xFF,
        (hi(0x8000) + hi(0xFFFF) * 4 + hi(0x7FFF) * 16) & 0xFF,
        1 * 16 + 3, 0 * 16 + 4, s & 0xFF, s >> 8,
    ]
    return checks


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestSimulated(unittest.TestCase):
    def test_both_pipelines(self) -> None:
        checks = _expected()
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                result = sim.run(max_cycles=20_000_000)
                self.assertFalse(result.timed_out)
                base = sim.symbols["checks"]
                got = list(result.memory[base:base + len(checks)])
                self.assertEqual(got, checks)


if __name__ == "__main__":
    unittest.main()
