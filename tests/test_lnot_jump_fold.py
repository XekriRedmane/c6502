"""Tests for cpp/src/passes/optimization/lnot_jump_fold.h.

The pass rewrites `Unary(LogicalNot, src, %t); JumpIf{True,False}(%t, t)`
— where `%t` is single-use and the JumpIf is immediately adjacent —
as a sense-flipped direct `JumpIf{False,True}(src, t)`. The Unary
itself is left in place; the now-dead Unary is reaped by standard
DSE in the same fixed-point loop.

Coverage:
  * Both (JumpIfTrue / JumpIfFalse) sense-flip cases.
  * Multi-use `%t` blocks the rewrite (correctness).
  * Non-adjacent rejection.
  * No JumpIf follower → no fold.
  * Non-LogicalNot Unary (Negate / Complement) → no fold.
  * End-to-end sim: `if (!x)` correctness across uint8_t / uint16_t
    sources.
  * Asm-level smoke: the `JSR f; LDA/BEQ/LDA #0/JMP/LDA #1/ORA/BEQ`
    materialize-then-test sequence collapses to a single inverted
    branch.

The `-S` text is checked in cpp/tests/test_lnot_jump_fold.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim, run_c_program


class TestEndToEnd(unittest.TestCase):
    """Differential opt-vs-unopt for `if (!x)` shapes."""

    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_uchar_negated(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) { return f(0) + 10 * f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (21, 21))

    def test_uint16_negated(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint16_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) { return f(0) + 10 * f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (21, 21))

    def test_uint32_negated(self):
        # Multi-byte src: the JumpIf*'s lowering walks every byte
        # (ORA chain), so the fold must still yield the right answer.
        src = (
            "#include <stdint.h>\n"
            "int f(uint32_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) {\n"
            "    return f(0) + 10 * f(0x00010000UL);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (21, 21))

    def test_negated_in_ternary(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { return !a ? 10 : 20; }\n"
            "int main(void) { return f(0) + f(5); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (30, 30))


if __name__ == "__main__":
    unittest.main()
