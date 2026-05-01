"""End-to-end smoke tests for tac_sim.

Drives short C programs through the real pipeline (parse → resolve →
type-check → c99_to_tac) and runs the resulting TAC in `tac_sim`,
asserting the simulator's return value matches what C would produce.
This pins TAC behavior independently of the (still-landing) 6502
runtime helpers.
"""
from __future__ import annotations

import unittest

from c99_to_tac import translate_program as translate_to_tac
from parser import parse
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.string_lifting import lift_program as lift_strings
from passes.type_checking import check_program as type_check_program
from tac_sim import Simulator


def _compile_to_tac(source: str):
    resolved = label_loops(resolve_labels(lift_strings(
        resolve_identifiers(parse(source)),
    )))
    prog, symbols, types = type_check_program(resolved)
    tac = translate_to_tac(prog, symbols, types)
    return tac, symbols


def _run(source: str, fn: str = "main", args: list[int] | None = None) -> int:
    tac, symbols = _compile_to_tac(source)
    sim = Simulator(tac, symbols)
    return sim.call(fn, args or [])


class TestTacSim(unittest.TestCase):
    def test_arithmetic(self):
        self.assertEqual(_run("int main(void) { return 1 + 2 * 3; }"), 7)

    def test_signed_div_truncates_toward_zero(self):
        # C99 §6.5.5.6: -7 / 2 == -3 (truncate, not floor).
        self.assertEqual(_run("int main(void) { return -7 / 2; }"), -3)
        self.assertEqual(_run("int main(void) { return -7 % 2; }"), -1)

    def test_long_overflow_wraps(self):
        # Long is 2 bytes signed, range -32768..32767. 30000 + 30000
        # overflows to -5536 in two's complement.
        self.assertEqual(
            _run("long main(void) { long a = 30000; return a + a; }"),
            -5536,
        )

    def test_unsigned_compare(self):
        # 0xFF as unsigned char would compare > 1; our 1-byte unsigned
        # is UInt. Make the comparison explicit.
        src = """
        int main(void) {
            unsigned int a = 200u;
            unsigned int b = 1u;
            return a > b;
        }
        """
        self.assertEqual(_run(src), 1)

    def test_signed_compare(self):
        # As `int` (signed 1-byte), 200 wraps to -56, which is < 1.
        src = """
        int main(void) {
            int a = 200;
            int b = 1;
            return a > b;
        }
        """
        self.assertEqual(_run(src), 0)

    def test_if_else(self):
        src = """
        int main(void) {
            long x = 5;
            if (x > 3) return 11; else return 22;
        }
        """
        self.assertEqual(_run(src), 11)

    def test_while_loop(self):
        src = """
        int main(void) {
            long sum = 0;
            long i = 0;
            while (i < 10) { sum = sum + i; i = i + 1; }
            return sum;
        }
        """
        self.assertEqual(_run(src), 45)

    def test_recursive_factorial(self):
        src = """
        long fact(long n) {
            if (n < 2) return 1;
            return n * fact(n - 1);
        }
        long main(void) { return fact(7); }
        """
        # 7! = 5040, fits in Long.
        self.assertEqual(_run(src), 5040)

    def test_call_with_args(self):
        src = """
        long add(long a, long b) { return a + b; }
        int main(void) { return add(40, 2); }
        """
        self.assertEqual(_run(src), 42)

    def test_continue_and_break(self):
        # Sum 1..10 skipping evens, stop at 7.
        src = """
        int main(void) {
            long sum = 0;
            long i = 0;
            while (i < 100) {
                i = i + 1;
                if (i > 7) break;
                if ((i & 1) == 0) continue;
                sum = sum + i;
            }
            return sum;
        }
        """
        # 1 + 3 + 5 + 7 = 16
        self.assertEqual(_run(src), 16)

    def test_signed_to_unsigned_widening(self):
        # int -1 -> SignExtend to 0xFFFF, reinterpreted as ULong is
        # 65535. main returns long (signed 2-byte), so 0xFFFF
        # reinterpreted as signed is -1.
        src = """
        long main(void) {
            int x = -1;
            unsigned long y = (unsigned long) x;
            return y;
        }
        """
        self.assertEqual(_run(src), -1)

    def test_truncate(self):
        # Long 4660 (0x1234) truncated to int = 0x34 = 52.
        src = """
        int main(void) {
            long x = 4660;
            return (int) x;
        }
        """
        self.assertEqual(_run(src), 0x34)


if __name__ == "__main__":
    unittest.main()
