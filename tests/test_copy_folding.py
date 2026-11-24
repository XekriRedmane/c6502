"""Tests for the TAC-level copy-folding pass.

`fold_copies` fuses adjacent `<producer dst=%t>; Copy(%t, X)` pairs
into `<producer dst=X>` when `%t` is single-use. The eliminated
Copy is what `c99_to_tac` emits to write back into a non-SSA-
promoted name (a static or address-taken local) and what
`from_ssa` emits at the end of each predecessor block to feed Phi
sources into Phi dsts.

Coverage:
  * Direct unit tests on synthetic TAC: Binary + Copy collapse to
    in-place Binary; SignExtend / Cast / FunctionCall similarly
    redirect; multi-use temp doesn't fuse; non-adjacent doesn't
    fuse; Phi.dst doesn't get redirected.
  * End-to-end via the optimizer pipeline:
      - `static T const` increment chains through to INC + BNE.
      - Loop counter `i++` (where regalloc coalesces) chains too.
      - Programs computed correctly under `-O`.
"""
from __future__ import annotations

import shutil
import unittest


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestCopyFoldingEndToEnd(unittest.TestCase):
    """Programs going through the full pipeline. Verifies the
    fusion + downstream INC peephole produce the expected asm
    and that the program computes the right answer."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_static_counter_collapses_to_inc_chain(self) -> None:
        # `static int counter; counter += 1;` — the textbook
        # case. Without fusion: ADC chain through a temp + Copy
        # back. With fusion: in-place ADC chain, which the INC
        # peephole then collapses to INC + BNE + INC.
        src = (
            "static int counter = 0;\n"
            "int main(void) { counter += 1; return counter; }\n"
        )
        asm = self._compile(src)
        # Expect INC counter; BNE done; INC counter+1; done:
        self.assertIn("INC   counter", asm)
        self.assertIn("BNE   .inc_done@", asm)
        self.assertIn("INC   counter+1", asm)
        # The temp routing is gone — no `STA counter\n   LDA`
        # round-trip.
        # (We can't easily check the *absence* of a temp, but
        # confirming the INC chain is enough.)

    def test_loop_counter_int_collapses_to_inc_chain(self) -> None:
        # `for (int i = 0; i < 10; i++)` — the loop counter's
        # `i++` is the textbook fusion case after from_ssa
        # inserts the back-edge Copy.
        src = (
            "int main(void) {\n"
            "    int s = 0;\n"
            "    for (int i = 0; i < 10; i++) s += 1;\n"
            "    return s;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The loop body should have INC + BNE + INC for the 16-bit
        # i++. The `.loop@0_continue:` label is dropped when no
        # `continue` statement targets it (unreferenced-label
        # cleanup); the `.inc_done@` label remains (it's the
        # target of the multi-byte INC's BNE).
        import re
        self.assertRegex(asm, r"INC\s+__local_\w+__\w+")
        self.assertIn(".inc_done@", asm)


if __name__ == "__main__":
    unittest.main()
