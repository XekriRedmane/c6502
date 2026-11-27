"""Tests for cpp/src/passes/optimization/cmp_zero_jump_fold.h.

The pass rewrites `Binary(==/!=, x, 0, cond); JumpIf(cond, t)` —
where `cond` is single-use — as a direct `JumpIf(x, t)` with the
appropriate sense flip. Optionally narrows by tracing through
ZeroExtend defs.

Coverage:
  * Each of the 4 (Equal/NotEqual) × (JumpIfTrue/JumpIfFalse)
    combinations rewrites correctly.
  * Zero on either side of the comparison.
  * ZeroExtend tracing produces the original narrow value.
  * Multi-use `cond` blocks the rewrite (correctness).
  * Non-zero comparison (e.g., `x == 5`) doesn't fire.
  * End-to-end sim: each pattern produces correct results.
  * Asm-level smoke check: the resulting body uses bare `LDA / BEQ`
    or `LDA / BNE` for the uint8_t case.

The `-S` text is checked in cpp/tests/test_cmp_zero_jump_fold.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim, run_c_program
from tests.no_inlining import no_inlining


# ---------------------------------------------------------------------------
# TAC-level rewrite tests.
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# End-to-end sim: each pattern produces correct results.
# ---------------------------------------------------------------------------


class TestEndToEnd(unittest.TestCase):
    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_static_cond_kept(self):
        # The compare's result goes to a static that another function
        # reads. Its one use in `check` is the jump, but the store
        # must stay. The fold had dropped it, so `apply` never
        # negated and returned -7.
        src = (
            "#include <stdint.h>\n"
            "#include <stdbool.h>\n"
            "static bool neg;\n"
            "uint16_t r;\n"
            "static uint16_t check(uint16_t a, uint16_t b) {\n"
            "    neg = (int16_t)(a ^ b) < 0;\n"
            "    if (neg) a = -a;\n"
            "    return a;\n"
            "}\n"
            "static void apply(uint16_t n) {\n"
            "    if (neg) n = -n;\n"
            "    r = n;\n"
            "}\n"
            "int main(void) { apply(check(7, 0xFFFE)); return (int)r; }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (7, 7))

    def test_uint8_eq_zero(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
            "int main(void) { return f(0); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (1, 1))

    def test_uint8_eq_zero_false(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
            "int main(void) { return f(5); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (2, 2))

    def test_uint8_ne_zero(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a != 0) return 1; return 2; }\n"
            "int main(void) { return f(5); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (1, 1))

    def test_uint16_eq_zero_ternary(self):
        # Ternary lowers to the same comparison-then-jump pattern;
        # verify the rewrite is correct under it too.
        src = (
            "#include <stdint.h>\n"
            "int f(uint16_t a) { return a == 0 ? 10 : 20; }\n"
            "int main(void) { return f(0) + f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (30, 30))


# ---------------------------------------------------------------------------
# JumpIfCmp rewrites: the generalized fold for ordering ops + non-zero
# constants. The pass produces `JumpIfCmp(op', src1, src2, target)`
# where op' is the original op (for JumpIfTrue) or the inverted op
# (for JumpIfFalse).
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Asm-level shape: narrow ordering compare-and-branch produces the
# 3-instruction LDA/CMP/BCS sequence.
# ---------------------------------------------------------------------------


class TestNarrowOrderingAsmShape(unittest.TestCase):

    def test_uchar_loop_returns_correct_count(self):
        # End-to-end: the loop must terminate with sum == 105.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t i;\n"
            "    int sum = 0;\n"
            "    for (i = 0; i < 105; i = i + 1) sum = sum + 1;\n"
            "    return sum;\n"
            "}\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int(), 105)


# ---------------------------------------------------------------------------
# SignExtend narrowing for `>= 0` / `< 0` against zero.
# ---------------------------------------------------------------------------


class TestStaleExtendSource(unittest.TestCase):
    """A ZeroExtend or SignExtend of a static that's stored to before
    the compare. The narrowing read the static at the jump, after the
    store, and compiled `int t = g; g = 5; if (t < 3)` to `LDA #$05;
    STA g; CMP #$03; BCS`."""

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t g;\n"
        "int8_t sg;\n"
        "uint8_t hits;\n"
        "void lt(void) { int t = g; g = 5; if (t < 3) hits |= 1; }\n"
        "void eq0(void) { int t = g; g = 5; if (t == 0) hits |= 2; }\n"
        "void lt_c(void) { int t = g; g = 5; int c = t < 3;"
        " if (c) hits |= 4; }\n"
        "void eq0_c(void) { int t = g; g = 5; int c = t == 0;"
        " if (c) hits |= 8; }\n"
        "void ge0(void) { int t = sg; sg = -5; if (t >= 0) hits |= 16; }\n"
        "int main(void) {\n"
        "    g = 1; lt(); g = 0; eq0(); g = 1; lt_c(); g = 0; eq0_c();\n"
        "    sg = 1; ge0();\n"
        "    return hits;\n"
        "}\n"
    )

    def test_store_between_extend_and_compare(self):
        for optimize in (False, True):
            r = build_sim(self._SRC, optimize=optimize).run()
            self.assertEqual(r.return_int(), 31, f"optimize={optimize}")


class TestMultiUseSsaExtend(unittest.TestCase):
    """A switch on a byte compares the one extended value with every
    case. When that value is SSA-renamed, each compare narrows to the
    byte, and the extend stays for any other use."""


    _SWITCH = (
        "#include <stdint.h>\n"
        "uint8_t pick(uint8_t n) {\n"
        "    switch (n) {\n"
        "    case 0: return 10;\n"
        "    case 1: return 11;\n"
        "    case 2: return 12;\n"
        "    case 7: return 17;\n"
        "    case 200: return 99;\n"
        "    case 300: return 1;\n"
        "    default: return 50;\n"
        "    }\n"
        "}\n"
        "int main(void) {\n"
        "    return pick(0) + pick(1) + pick(2) + pick(7) + pick(200)\n"
        "        + pick(44);\n"
        "}\n"
    )

    def test_switch_on_byte_runs(self):
        for optimize in (False, True):
            r = build_sim(self._SWITCH, optimize=optimize).run()
            self.assertEqual(
                r.return_int(), 10 + 11 + 12 + 17 + 99 + 50,
                f"optimize={optimize}",
            )


@no_inlining()
class TestValueCompareNarrowing(unittest.TestCase):
    """A comparison kept as a value narrows its operands through their
    ZeroExtends (`NARROW_VALUE_COMPARE_RULE`), so `(int)x < 10` on a
    `uint8_t x` subtracts one byte."""

    _VALUES = (0, 9, 10, 11, 127, 128, 200, 255)

    def _src(self, expr: str) -> str:
        table = ", ".join(str(v) for v in self._VALUES)
        return (
            "#include <stdint.h>\n"
            f"uint8_t g[{len(self._VALUES)}] = {{{table}}};\n"
            "static uint8_t test(uint8_t x) { return " + expr + "; }\n"
            "int main(void) {\n"
            "    uint16_t n = 0;\n"
            f"    for (uint8_t i = 0; i < {len(self._VALUES)}; i++)\n"
            "        n = (uint16_t)(n << 1) | test(g[i]);\n"
            "    return n;\n"
            "}\n"
        )

    def _expected(self, pred) -> int:
        n = 0
        for v in self._VALUES:
            n = ((n << 1) | int(pred(v))) & 0xFFFF
        return n

    _CASES = (
        ("x < 10", lambda x: x < 10),
        ("x <= 10", lambda x: x <= 10),
        ("x > 127", lambda x: x > 127),
        ("x >= 200", lambda x: x >= 200),
        ("10 < x", lambda x: 10 < x),
        ("255 > x", lambda x: 255 > x),
        ("x < 256", lambda x: True),
        ("x > -1", lambda x: True),
    )

    def test_runs(self):
        for expr, pred in self._CASES:
            for optimize in (False, True):
                with self.subTest(expr=expr, optimize=optimize):
                    r = build_sim(self._src(expr), optimize=optimize).run()
                    self.assertEqual(r.return_int(), self._expected(pred))

if __name__ == "__main__":
    unittest.main()
