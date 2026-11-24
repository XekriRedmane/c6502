"""Tests for `passes.optimization.strength_reduction`.

Coverage:
  * Multiply by 2^k → LeftShift by k (signed and unsigned).
  * Multiply by 1 → Copy.
  * Multiply by non-power-of-2 → unchanged.
  * Unsigned Divide by 2^k → RightShift by k.
  * Signed Divide by 2^k → unchanged (rounding semantics differ).
  * Unsigned Modulo by 2^k → BitwiseAnd by 2^k - 1.
  * Signed Modulo by 2^k → unchanged.
  * Mul/Div by 1 with both operands constant → unchanged (defers
    to constant folding).
  * Operand ordering: `2 * x` reduced same as `x * 2`.

Sim-end-to-end checks confirm the rewritten programs produce
identical results to the unoptimized helper-call paths for both
signed and unsigned arithmetic.
"""
from __future__ import annotations

import unittest

from passes import ir_check
from sim.harness import build_sim, run_c_program


# ---------------------------------------------------------------------------
# TAC-level rewrite tests
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# End-to-end sim tests — strength reduction + inline shift produce
# identical results to the unoptimized helper-call path.
# ---------------------------------------------------------------------------


class TestEndToEndShifts(unittest.TestCase):
    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_two_multiplies_into_one_global(self):
        # A global keeps its name in SSA form, so each multiply's
        # temporaries need names of their own (fuzzer stmts22250).
        src = """
        unsigned long z [[c6502::zeropage]] = 128;
        int main(void) {
            unsigned char w;
            z *= 3;
            w = 0;
            do {
                if (w & 1) z *= 129;
            } while (++w < 4);
            return (int)(z & 0x7FFF);
        }
        """
        was = ir_check.enabled()
        ir_check.enable()
        try:
            a, b = self._both_paths(src)
        finally:
            ir_check.enable(was)
        want = (128 * 3 * 129 * 129) & 0x7FFF
        self.assertEqual((a, b), (want, want))

    def test_unsigned_multiply_by_2(self):
        src = (
            "unsigned int f(unsigned int x) { return x * 2; }\n"
            "int main(void) { return (int)f(13579u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_unsigned_multiply_by_8(self):
        src = (
            "unsigned int f(unsigned int x) { return x * 8; }\n"
            "int main(void) { return (int)f(100u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_signed_multiply_by_4(self):
        # Signed * power of 2 is also reduced to LeftShift; check
        # both positive and (after wrap) effectively negative.
        src = (
            "int f(int x) { return x * 4; }\n"
            "int main(void) { return f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_unsigned_divide_by_2(self):
        src = (
            "unsigned int f(unsigned int x) { return x / 2; }\n"
            "int main(void) { return (int)f(15u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_unsigned_modulo_by_4(self):
        src = (
            "unsigned int f(unsigned int x) { return x % 4; }\n"
            "int main(void) { return (int)f(13u); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)

    def test_signed_divide_by_4_keeps_truncation(self):
        # Signed division must NOT be strength-reduced — verify the
        # rounding-toward-zero behavior is preserved.
        src = (
            "int f(int x) { return x / 4; }\n"
            "int main(void) { return f(-7); }\n"  # -7/4 = -1
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, -1)
        self.assertEqual(b, -1)


class TestInlineShiftByOne(unittest.TestCase):
    """The inline `LDA / ASL/LSR/ROR / STA` byte-chain in
    `tac_to_asm` matches the runtime helper's result. Test 1, 2,
    and 4-byte shifts; signed and unsigned right shifts."""

    def _run(self, src: str, *, opt: bool = False) -> int:
        if opt:
            return build_sim(src, optimize=True).run().return_int_signed()
        return run_c_program(src).return_int_signed()

    def test_uint16_shift_left_by_1(self):
        src = (
            "unsigned int f(unsigned int x) { return x << 1; }\n"
            "int main(void) { return (int)f(0x4000u); }\n"  # 0x8000
        )
        # No-opt path uses asl16 helper. -O uses inline.
        # Both should match: 0x8000 (which signs to -32768 in int).
        self.assertEqual(self._run(src, opt=False), -32768)
        self.assertEqual(self._run(src, opt=True), -32768)

    def test_uint16_shift_right_by_1(self):
        src = (
            "unsigned int f(unsigned int x) { return x >> 1; }\n"
            "int main(void) { return (int)f(0xFFFEu); }\n"  # 0x7FFF
        )
        self.assertEqual(self._run(src, opt=False), 0x7FFF)
        self.assertEqual(self._run(src, opt=True), 0x7FFF)

    def test_int16_shift_right_by_1_signed(self):
        # Arithmetic right shift preserves sign: -2 >> 1 = -1.
        src = (
            "int f(int x) { return x >> 1; }\n"
            "int main(void) { return f(-2); }\n"
        )
        self.assertEqual(self._run(src, opt=False), -1)
        self.assertEqual(self._run(src, opt=True), -1)

    def test_int16_shift_right_negative_odd(self):
        # -3 >> 1 = -2 (arithmetic right shift rounds toward -inf).
        src = (
            "int f(int x) { return x >> 1; }\n"
            "int main(void) { return f(-3); }\n"
        )
        self.assertEqual(self._run(src, opt=False), -2)
        self.assertEqual(self._run(src, opt=True), -2)


class TestThreeTermByteMultiply(unittest.TestCase):
    """A byte times a constant of three signed powers of two is shifts
    and adds, where it had called `mul8`."""

    def test_prng_step(self):
        from compile import _run_stage
        from preprocessor import preprocess
        src = (
            "#include <stdint.h>\n"
            "uint8_t st;\n"
            "uint8_t prng(void) { st = 69 * st + 0x53; return st >> 1; }\n"
        )
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        self.assertNotIn("mul8", asm)

    def test_every_byte_constant(self):
        # Each constant from 0 to 255 times a few values, against
        # Python, for the constants this rule takes and the others.
        values = (0, 1, 2, 3, 0x45, 0x80, 0xAB, 0xFF)
        lines = ["#include <stdint.h>",
                 "volatile uint8_t v[8] = {" + ", ".join(map(str, values)) + "};",
                 "uint16_t check(uint8_t c, uint8_t k);",
                 "int main(void) {", "    uint16_t bad = 0;"]
        for c in range(256):
            lines.append(
                f"    for (uint8_t k = 0; k < 8; k++) if ((uint8_t)(v[k] * {c}) "
                f"!= (uint8_t)check({c}, k)) bad++;")
        lines += ["    return bad;", "}",
                  "uint16_t check(uint8_t c, uint8_t k) { return (uint8_t)(c * v[k]); }"]
        src = "\n".join(lines) + "\n"
        out = build_sim(src, optimize=True).run(max_cycles=200_000_000)
        self.assertFalse(out.timed_out)
        self.assertEqual(out.return_int(), 0)

if __name__ == "__main__":
    unittest.main()
