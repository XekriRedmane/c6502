"""c6502's statement attributes (C23 §6.8): `[[c6502::unroll]]` and
`[[c6502::keep]]` before a `for` loop, and
`[[c6502::save_hardware_stack]]` before an expression statement, a
`return`, or a declaration in a block. Covers where the grammar takes
attributes before a statement, the errors for each attribute where it
doesn't apply, and the error for the `#pragma c6502` lines they
replaced. cpp/tests/test_unroll.cpp, cpp/tests/test_unroll_source.cpp,
and tests/test_recursion_saves.py test what `unroll` and
`save_hardware_stack` do.

The `--tac -O` text of a kept loop is checked in
cpp/tests/test_statement_attributes.cpp."""

from __future__ import annotations

import unittest


class TestLoopAttributes(unittest.TestCase):

    def test_break_and_continue_in_a_kept_loop(self) -> None:
        # A `break` or `continue` carries the loop's label without
        # KEEP_LOOP_MARK, and once jumped to a label the kept loop
        # never defined, which crashed the CFG build under -O.
        from sim.harness import build_sim
        src = (
            "#include <stdint.h>\n"
            "uint8_t even;\n"
            "uint8_t f(uint8_t x) {\n"
            "    uint8_t n = 0;\n"
            "    [[c6502::keep]] for (;;) {\n"
            "        if (--x == 0) break;\n"
            "        n++;\n"
            "        if (x & 1) continue;\n"
            "        even++;\n"
            "    }\n"
            "    return n;\n"
            "}\n"
            "int main(void) { return f(7) * 16 + even; }\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                out = build_sim(src, optimize=optimize).run()
                # x runs 6 down to 1, so n counts 6 passes, and even
                # counts the 3 with x even.
                self.assertEqual(out.return_int(), 6 * 16 + 3)


if __name__ == "__main__":
    unittest.main()
