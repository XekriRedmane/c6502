"""Focused unit tests for the asm-level simulator: small C snippets
that target one feature at a time. Faster than the chapter walk and
self-documenting as a feature checklist.

These tests also exercise the Python-implemented runtime helpers
(`mul*`, `divmod*`, `asl*`, `asr*`, `lsr*`) — the simulator's stand-in
until the real 6502 helpers land in the runtime header.
"""

from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim, run_c_program


def _signed_byte(v: int) -> int:
    return v - 0x100 if v & 0x80 else v


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestBasics(unittest.TestCase):
    def test_constant_return(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 42; }").return_int(), 42)

    def test_return_zero(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 0; }").return_int(), 0)

    def test_negative_return(self) -> None:
        # -1 → 0xFFFF; signed view = -1.
        self.assertEqual(run_c_program(
            "int main(void) { return -1; }").return_int_signed(), -1)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestArithmetic(unittest.TestCase):
    def test_add(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 5 + 7; }").return_int(), 12)

    def test_sub(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 20 - 5; }").return_int(), 15)

    def test_mul_via_helper(self) -> None:
        # Hits the mul8 trap — verifies helper hook + RTS synthesis.
        self.assertEqual(run_c_program(
            "int main(void) { return 6 * 7; }").return_int(), 42)

    def test_div_unsigned(self) -> None:
        # Both operands positive — divmod8 hook gives the right answer.
        self.assertEqual(run_c_program(
            "int main(void) { return 100 / 7; }").return_int(), 14)

    def test_mod_unsigned(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 100 % 7; }").return_int(), 2)

    def test_unary_neg(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { int x = 7; return -x; }").return_int_signed(), -7)

    def test_bitwise(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 0x55 & 0x0F; }").return_int(), 0x05)
        self.assertEqual(run_c_program(
            "int main(void) { return 0x55 | 0xAA; }").return_int(), 0xFF)
        self.assertEqual(run_c_program(
            "int main(void) { return 0x55 ^ 0x0F; }").return_int(), 0x5A)

    def test_shift_int(self) -> None:
        # 1-byte shifts via asl8 / asr8 hooks.
        self.assertEqual(run_c_program(
            "int main(void) { return 3 << 2; }").return_int(), 12)
        self.assertEqual(run_c_program(
            "int main(void) { return 100 >> 2; }").return_int(), 25)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestComparisons(unittest.TestCase):
    def test_equal(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 5 == 5; }").return_int(), 1)
        self.assertEqual(run_c_program(
            "int main(void) { return 5 == 6; }").return_int(), 0)

    def test_lt_gt(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 3 < 5; }").return_int(), 1)
        self.assertEqual(run_c_program(
            "int main(void) { return 5 > 3; }").return_int(), 1)

    def test_short_circuit_and(self) -> None:
        # Right side wouldn't divide-by-zero because && short-circuits.
        self.assertEqual(run_c_program(
            "int main(void) { return 0 && (1 / 0); }").return_int(), 0)

    def test_short_circuit_or(self) -> None:
        self.assertEqual(run_c_program(
            "int main(void) { return 1 || (1 / 0); }").return_int(), 1)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestControlFlow(unittest.TestCase):
    def test_if_else(self) -> None:
        src = """
            int main(void) {
                int x = 10;
                if (x > 5) return 1;
                else return 0;
            }
        """
        self.assertEqual(run_c_program(src).return_int(), 1)

    def test_while_loop(self) -> None:
        src = """
            int main(void) {
                int s = 0; int i = 1;
                while (i <= 10) { s = s + i; i = i + 1; }
                return s;
            }
        """
        self.assertEqual(run_c_program(src).return_int(), 55)

    def test_for_loop(self) -> None:
        src = """
            int main(void) {
                int s = 0;
                for (int i = 1; i <= 10; i++) s += i;
                return s;
            }
        """
        self.assertEqual(run_c_program(src).return_int(), 55)

    def test_break_continue(self) -> None:
        src = """
            int main(void) {
                int s = 0;
                for (int i = 1; i <= 10; i++) {
                    if (i == 5) continue;
                    if (i == 8) break;
                    s += i;
                }
                return s;
            }
        """
        # 1+2+3+4+6+7 = 23
        self.assertEqual(run_c_program(src).return_int(), 23)

    def test_goto(self) -> None:
        src = """
            int main(void) {
                int x = 0;
                goto end;
                x = 1;
              end:
                return x;
            }
        """
        self.assertEqual(run_c_program(src).return_int(), 0)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestFunctions(unittest.TestCase):
    def test_simple_call(self) -> None:
        src = """
            int add(int a, int b) { return a + b; }
            int main(void) { return add(3, 4); }
        """
        self.assertEqual(run_c_program(src).return_int(), 7)

    def test_recursion(self) -> None:
        src = """
            int fib(int n) {
                if (n < 2) return n;
                return fib(n - 1) + fib(n - 2);
            }
            int main(void) { return fib(10); }
        """
        self.assertEqual(run_c_program(src).return_int(), 55)

    def test_calling_convention_ssp_rewinds(self) -> None:
        """After main returns to the boot stub's BRK, SSP should be
        back at $7FFF — the value the boot stub set before JSR. This
        catches a class of epilogue bugs that wouldn't show up just by
        looking at A."""
        src = "int main(void) { int x = 1; int y = 2; return x + y; }"
        sim = build_sim(src)
        sim.run()
        # SSP at zero-page $00/$01.
        ssp = sim.mpu.memory[0x00] | (sim.mpu.memory[0x01] << 8)
        self.assertEqual(ssp, 0x7FFF, f"SSP not rewound: ${ssp:04X}")


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStaticsAndPointers(unittest.TestCase):
    def test_file_scope_static(self) -> None:
        src = """
            int counter = 100;
            int main(void) { counter = counter + 5; return counter; }
        """
        self.assertEqual(run_c_program(src).return_int(), 105)

    def test_pointer_write(self) -> None:
        src = """
            int main(void) {
                int x = 42;
                int *p = &x;
                *p = *p + 1;
                return x;
            }
        """
        self.assertEqual(run_c_program(src).return_int(), 43)

    def test_array_subscript(self) -> None:
        src = """
            int main(void) {
                int a[5] = {10, 20, 30, 40, 50};
                return a[0] + a[2] + a[4];
            }
        """
        self.assertEqual(run_c_program(src).return_int(), 90)


if __name__ == "__main__":
    unittest.main()
