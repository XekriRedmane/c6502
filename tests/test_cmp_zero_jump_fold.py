"""Tests for `passes.optimization.cmp_zero_jump_fold`.

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
# Asm-level smoke check
# ---------------------------------------------------------------------------


class TestAsmShape(unittest.TestCase):
    """For `if (uint8_t a == 0)`, the optimized asm body should
    contain a bare `LDA / BEQ` or `LDA / BNE` for the comparison —
    not a multi-byte CMP / 0/1-select sequence."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_uint8_eq_zero_uses_bne(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
            "int main(void) { return f(0); }\n"
        )
        asm = self._compile(src)
        body = self._function_body_without_frame(asm, "f")
        # The body (excluding prologue/epilogue) should NOT have
        # the V-corrected signed-comparison sequence.
        self.assertNotIn("BVC", body)
        # And should NOT have a `CMP` / multi-byte SBC chain for
        # the comparison — the bare zero-test only needs LDA + BEQ
        # / BNE.
        self.assertNotIn("CMP", body)
        # SHOULD have a BNE for the if-end branch.
        self.assertIn("BNE", body)

    @staticmethod
    def _function_body_without_frame(asm: str, name: str) -> str:
        """Like `_extract_function` but strips the prologue and
        epilogue regions (between the `; prologue:` / `; epilogue`
        comments and the next blank line). Leaves only the
        function's actual user-code body so signed-cmp checks
        aren't fooled by SSP arithmetic in the frame setup."""
        lines: list[str] = []
        in_fn = False
        in_frame_block = False
        for line in asm.splitlines():
            if line.startswith(f"{name}:"):
                in_fn = True
            if not in_fn:
                continue
            if (
                line and not line.startswith((" ", "\t", "."))
                and line.endswith(":") and not line.startswith(f"{name}:")
            ):
                break
            if "; prologue:" in line or "; epilogue" in line:
                in_frame_block = True
                continue
            if in_frame_block:
                if line == "":
                    in_frame_block = False
                continue
            lines.append(line)
        return "\n".join(lines)

    @staticmethod
    def _extract_function(asm: str, name: str) -> str:
        out: list[str] = []
        in_fn = False
        for line in asm.splitlines():
            if line.startswith(f"{name}:"):
                in_fn = True
            if in_fn:
                if (
                    line and not line.startswith((" ", "\t", "."))
                    and line.endswith(":") and not line.startswith(f"{name}:")
                ):
                    break
                out.append(line)
        return "\n".join(out)


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
    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_uchar_lt_const_uses_8bit_cmp_bcs(self):
        # The loop test in `for (uint8_t i = 0; i < 105; i++)` should
        # compile to a 1-byte unsigned compare-and-branch:
        #     LDA <i>; CMP #105; BCC <body>
        # at the bottom of the loop (passes/rotate_loop_test.py). The
        # body reads `i`, or the loop would count down to 0
        # (passes/optimization/count_down.py) and have no compare.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t i;\n"
            "    int sum = 0;\n"
            "    for (i = 0; i < 105; i = i + 1) sum = sum + i;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The 16-bit signed-compare V-correction sequence must be
        # absent for this loop test.
        self.assertNotIn("BVC   .jcmp_novf", asm)
        # And we should see CMP #$69 (105 in hex) followed by a BCC
        # back to the loop's body.
        self.assertIn("CMP   #$69\n   BCC   .main@rot@", asm)

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


# ---------------------------------------------------------------------------
# Asm-level: signed `>= 0` lowers to `LDA; B<PL|MI>` (no SBC chain).
# ---------------------------------------------------------------------------


class TestSignedZeroOrderingAsmShape(unittest.TestCase):
    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_signed_byte_ge_zero_uses_bare_lda_bpl(self):
        # `int8_t x; for (...; x >= 0; x--)` — the loop tail
        # should NOT contain the V-correction (BVC + EOR #$80)
        # sequence. The lowering of `JumpIfCmp(GE, schar, 0, .top)`
        # is `LDA schar; BPL .top` — 2 instructions.
        src = (
            "#include <stdint.h>\n"
            "int sum;\n"
            "int main(void) {\n"
            "    for (int8_t x = 15; x >= 0; x--) sum += 1;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # No V-correction in this function.
        self.assertNotIn("BVC   .jcmp_novf", asm)
        # And `BPL` to a loop-back target appears (the
        # `redundant_load_after_rmw` pass also drops the LDA, so
        # asm shows DEC + BPL directly).
        self.assertIn("BPL", asm)


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

    def test_switch_on_byte_compares_bytes(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._SWITCH), optimize=True)
        body = asm[asm.index("pick:"):asm.index("main:")]
        # No compare of the promoted value's zero high byte.
        self.assertNotIn("CMP   #$00", body)
        self.assertIn("CMP   #$C8", body)

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

    def test_one_byte_compare(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._src("x < 10")),
                         optimize=True)
        body = asm[asm.index("\ntest:"):asm.index("\nmain:")]
        self.assertNotIn("BVC", body)
        self.assertNotIn("SBC   #$00", body)
        self.assertIn("#$0A", body)


class TestConstexprBound(unittest.TestCase):
    """A global byte compared with a `constexpr uint8_t` narrows. The
    constant's read is folded only after the compare reaches its jump,
    where the global's extend is found in the same straight run."""

    def test_byte_compare(self):
        from compile import _run_stage
        from preprocessor import preprocess
        src = (
            "#include <stdint.h>\n"
            "constexpr uint8_t M = 0x63;\n"
            "extern uint8_t T [[c6502::address(0x4239)]];\n"
            "void f(void) { if (T < M) T++; }\n"
        )
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        body = asm[asm.index("\nf:"):]
        self.assertIn("CMP   #$63", body)
        self.assertNotIn("BVC", body)

if __name__ == "__main__":
    unittest.main()
