"""Tests for the asm-level `redundant_load_after_rmw` peephole.

The pass drops `LDA M` immediately following an in-place rmw on
the same M (DEC / INC / ASL / LSR / ROL / ROR), provided `Reg(A)`
is dead after the deletion point — every forward path kills A
before reading it. The motivating case is the rotated signed-
countdown loop tail `DEC m; LDA m; BPL .top`, which collapses to
the canonical `DEC m; BPL .top` 6502 idiom.

Coverage:
  * Direct unit tests on synthetic asm: drops the LDA when A is
    dead via fall-through kill, branch-target kill, or both;
    keeps the LDA when A is read on either path; refuses on
    non-rmw or address-mismatched cases; handles cycle (loop
    back-edge) without infinite recursion.
  * End-to-end via the optimizer pipeline: a signed-countdown
    `for` loop emits `DEC m; BPL .top` with no intervening LDA.

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6).
"""
from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRedundantLoadAfterRmwE2E(unittest.TestCase):
    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_signed_countdown_collapses_to_dec_bpl(self) -> None:
        # `int8_t` (1 byte signed): the post-decrement is a 1-byte
        # SBC chain that `dec_peephole` collapses to DEC, and the
        # cond becomes a 1-byte test against zero. This pass then
        # drops the redundant LDA between DEC and BPL.
        src = (
            "#include <stdint.h>\n"
            "int sum;\n"
            "int main(void) {\n"
            "    for (int8_t x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The loop tail must contain "DEC <op>" followed (possibly
        # via a label) by "BPL", with no intervening "LDA <op>"
        # for the same op.
        lines = asm.split("\n")
        dec_line = None
        for i, l in enumerate(lines):
            if l.strip().startswith("DEC   "):
                dec_line = i
                break
        self.assertIsNotNone(dec_line, msg=f"no DEC in:\n{asm}")
        # The instruction after DEC (skipping any blank lines)
        # should be BPL — no LDA between.
        nxt = dec_line + 1
        while nxt < len(lines) and not lines[nxt].strip():
            nxt += 1
        self.assertTrue(
            lines[nxt].strip().startswith("BPL "),
            msg=f"expected BPL after DEC, got {lines[nxt]!r}\n{asm}",
        )

    def test_simulator_returns_correct_value(self) -> None:
        # Verify semantics preserved end-to-end through the sim.
        src = (
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        from sim.harness import build_sim
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=200_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int_signed(), 120)


if __name__ == "__main__":
    unittest.main()
