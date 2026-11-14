"""A compare and a subtract of the same operands become one subtract
(`passes/optimization/compare_subtract.py`, and
`tac_to_asm.Translator._fused_subtract`).

Each case runs in both pipelines, with values that make the compare
go both ways, including signed operands whose difference overflows
on the side of the branch that doesn't use it, and checks that the
optimized code has no separate compare.
"""
from __future__ import annotations

import shutil
import unittest

from compile import _run_stage
from preprocessor import preprocess
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None

_CASES = [
    # (function, [(call, expected)])
    ("""
     unsigned int f(unsigned int s, unsigned int k) {
         while (s >= k) s -= k;
         return s;
     }""", [("f(1000, 16)", 8), ("f(65535, 1000)", 535), ("f(3, 7)", 3),
            ("f(7, 7)", 0)]),
    ("""
     int f(int s, int k) {
         while (s >= k) s -= k;
         return s;
     }""", [("f(1000, 16)", 8), ("f(-5, 3)", -5), ("f(32767, 30000)", 2767),
            ("f(100, 1)", 0)]),
    ("""
     unsigned char f(unsigned char s, unsigned char k) {
         while (s >= k) s -= k;
         return s;
     }""", [("f(250, 7)", 5), ("f(255, 255)", 0), ("f(0, 1)", 0)]),
    ("""
     int f(int s, int k) {
         if (s >= k) return s - k;
         return 0;
     }""", [("f(-30000, 30000)", 0), ("f(100, -100)", 200), ("f(5, 3)", 2),
            ("f(-100, 100)", 0), ("f(3, 3)", 0)]),
    ("""
     signed char f(signed char s, signed char k) {
         if (s < k) return 0;
         return s - k;
     }""", [("f(-100, 100)", 0), ("f(100, -27)", 127), ("f(5, 3)", 2),
            ("f(-128, 127)", 0)]),
    ("""
     unsigned long f(unsigned long a, unsigned long b) {
         if (a > b) return b - a;
         return 7;
     }""", [("f(5, 3)", (3 - 5) & 0xFFFF), ("f(3, 5)", 7),
            ("f(70000, 69999)", (69999 - 70000) & 0xFFFF)]),
]


def _program(fn: str, call: str) -> str:
    return fn + f"\nint main(void) {{ return (int)({call}); }}\n"


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestCompareSubtract(unittest.TestCase):

    def test_results(self) -> None:
        for fn, calls in _CASES:
            for call, expected in calls:
                src = _program(fn, call)
                for optimize in (False, True):
                    with self.subTest(fn=fn.split("{")[0].strip(), call=call,
                                      optimize=optimize):
                        got = build_sim(src, optimize=optimize).run()
                        self.assertEqual(got.return_int(), expected & 0xFFFF)

    def test_no_separate_compare(self) -> None:
        for fn, calls in _CASES:
            if "signed char" in fn:
                # The compare is of the operands widened to `int`, and
                # the subtract is narrowed back to a byte, so the two
                # don't match.
                continue
            with self.subTest(fn=fn.split("{")[0].strip()):
                asm = _run_stage(
                    "codegen", preprocess(_program(fn, calls[0][0])),
                    optimize=True,
                )
                body = asm.split("\nf:\n")[1].split("\nmain:\n")[0]
                self.assertNotIn("CMP", body)
                self.assertEqual(body.count("SBC"), _width(fn), body)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestLoopInRegisters(unittest.TestCase):
    """`while (s >= k) s -= k;` on a 16-bit `s`, as Zork's
    `do_rwts_on_sector` does: `s` stays in A (low) and X (high), the
    exit adds `k` back, and a pass through the loop is `SBC; BCS` when
    nothing is borrowed from the high byte."""

    def test_shape(self) -> None:
        src = """
        unsigned char k;
        unsigned char n;
        unsigned char f(unsigned int s) {
            while (s >= k) { s -= k; n++; }
            return (unsigned char)s;
        }
        int main(void) { k = 16; return f(1000) + n; }
        """
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        body = asm.split("\nf:\n")[1].split("\nmain:\n")[0]
        loop = body.split(".loop@0_continue:")[1].split(".loop@0_break:")[0]
        lines = [" ".join(line.split()) for line in loop.splitlines()
                 if line.strip()]
        self.assertEqual(lines, [
            "SBC k", "BCS .thread@0", "CPX #$01", "DEX",
            "BCC .loop@0_break", ".thread@0:", "INC n",
            "JMP .loop@0_continue",
        ])
        self.assertIn("ADC   k", body.split(".loop@0_break:")[1])
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(
                    build_sim(src, optimize=optimize).run().return_int(),
                    1000 % 16 + 1000 // 16,
                )


def _width(fn: str) -> int:
    """How many bytes the function's operands have."""
    if "long" in fn:
        return 4
    return 1 if "char" in fn else 2


if __name__ == "__main__":
    unittest.main()
