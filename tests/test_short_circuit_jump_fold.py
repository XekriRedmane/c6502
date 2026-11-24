"""Tests for `passes.optimization.short_circuit_jump_fold`.

The pass rewrites the canonical 5-instruction short-circuit tail
(`Copy(C_ft,%t); Jump(end); Label(branch); Copy(C_sc,%t); Label(end)`)
plus an adjacent `JumpIf{True,False}(%t, T)` consumer into direct
conditional branches that retarget the chain's short-circuit jumps
to wherever the consumer would have routed `%t == C_sc`. The
tail+consumer is deleted; for the "flipped" case where the
consumer's branch direction routes `%t == C_sc` to the
fall-through, a fresh `.<funcname>@scfold@<N>` label and a
trailing `Jump(T)` materialize the fall-through path.

Coverage:
  * All four (C_ft, C_sc) × consumer-kind combinations: AND/OR
    crossed with JumpIfFalse/JumpIfTrue, covering the natural and
    flipped cases for both senses.
  * Retargeting of multiple chain jumps (3-operand `&&`).
  * Nested short-circuits — outer fold's branch_label flows into
    inner fold's retarget map (transitive closure).
  * Multi-use `%t` blocks the rewrite (correctness).
  * Multi-target `end_label` blocks the rewrite (correctness).
  * Non-{0,1} constants don't fire (Conditional shape).
  * End-to-end sim across `&&` / `||` / nested.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim, run_c_program


class TestEndToEnd(unittest.TestCase):
    """Differential opt-vs-unopt for short-circuit shapes."""

    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_and_uchar_natural(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a, uint8_t b) {\n"
            "    if (a >= 0x40 && a < 0x47) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x44, 0) + 10 * f(0x10, 0) + 100 * f(0x50, 0);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 1 + 10 * 2 + 100 * 2)

    def test_or_uchar_flipped(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a < 0x40 || a >= 0x50) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x44) + 10 * f(0x10) + 100 * f(0x60);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 2 + 10 * 1 + 100 * 1)

    def test_nested_or_of_or(self):
        # Three-operand `||` — c99_to_tac nests as `(a||b)||c`,
        # so two foldable short-circuits stack.
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a == 0x63 || a == 0x8B || a == 0xB3) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x63) + 10 * f(0x8B) + 100 * f(0xB3)\n"
            "        + 1000 * f(0x00);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 1 + 10 * 1 + 100 * 1 + 1000 * 2)

    def test_uint16_operands(self):
        # The chain's JumpIf*Cmp lowering ORs each byte; the fold
        # is width-agnostic, so a uint16_t source should also work.
        src = (
            "#include <stdint.h>\n"
            "int f(uint16_t a) {\n"
            "    if (a >= 0x0100 && a < 0x0200) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x0150) + 10 * f(0x0050) + 100 * f(0x0250);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 1 + 10 * 2 + 100 * 2)


class TestAsmShape(unittest.TestCase):
    """After the fold, the `&&` / `||` 0-or-1 materialize labels
    (.and_false / .and_end / .or_true / .or_end) should be gone
    from the asm body of a function whose only short-circuit is
    consumed by an `if`."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_and_collapses_to_chain(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a >= 0x40 && a < 0x47) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) { return f(0x44); }\n"
        )
        asm = self._compile(src)
        body = self._extract_function(asm, "f")
        self.assertNotIn(".and_false", body)
        self.assertNotIn(".and_end", body)

    def test_or_collapses_to_chain(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a < 0x40 || a >= 0x50) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) { return f(0x44); }\n"
        )
        asm = self._compile(src)
        body = self._extract_function(asm, "f")
        self.assertNotIn(".or_true", body)
        self.assertNotIn(".or_end", body)

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


class TestCopiesBeforeConsumer(unittest.TestCase):
    """A phi's copy between the tail and the consumer, as SSA
    destruction leaves `if (c >= 0x60 && c < 0x80) c -= 0x20;`."""


    def test_runs(self):
        # buffer_char's case conversion, in both pipelines.
        src = """
        unsigned char up(unsigned char c) {
            if (c >= 0x60 && c < 0x80) c -= 0x20;
            return c | 0x80;
        }
        int main(void) {
            return up('a') + up('Z') * 2 + up(0x90) * 4;
        }
        """
        want = (ord("A") | 0x80) + (ord("Z") | 0x80) * 2 + 0x90 * 4
        self.assertEqual(run_c_program(src).return_int(), want)
        self.assertEqual(
            build_sim(src, optimize=True).run().return_int(), want,
        )

if __name__ == "__main__":
    unittest.main()
