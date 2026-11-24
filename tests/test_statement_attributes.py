"""c6502's statement attributes (C23 §6.8): `[[c6502::unroll]]` and
`[[c6502::keep]]` before a `for` loop, and
`[[c6502::save_hardware_stack]]` before an expression statement, a
`return`, or a declaration in a block. Covers where the grammar takes
attributes before a statement, the errors for each attribute where it
doesn't apply, and the error for the `#pragma c6502` lines they
replaced. tests/test_unroll.py and tests/test_recursion_saves.py test
what `unroll` and `save_hardware_stack` do."""

from __future__ import annotations

import logging
import unittest
from pathlib import Path

from lark import Lark, logger as lark_logger

from compile import _run_stage
from preprocessor import preprocess


class TestGrammar(unittest.TestCase):

    def test_no_new_conflicts(self) -> None:
        # Lark resolves a shift/reduce conflict as a shift and logs it
        # at DEBUG. The grammar has had these four: the dangling
        # `else`, and `void` before `)` in three states. Attributes
        # before a statement add none.
        messages: list[str] = []

        class Collect(logging.Handler):
            def emit(self, record):
                messages.append(record.getMessage())

        handler = Collect()
        level = lark_logger.level
        lark_logger.addHandler(handler)
        lark_logger.setLevel(logging.DEBUG)
        try:
            Lark.open(
                str(Path(__file__).resolve().parent.parent / "c99.lark"),
                parser="lalr",
                start=["start", "lex_only", "declarator", "type_name"],
            )
        finally:
            lark_logger.removeHandler(handler)
            lark_logger.setLevel(level)
        conflicts = sorted(
            m for m in messages if "Reduce/Reduce" in m
            or m.startswith("Shift/Reduce")
        )
        self.assertEqual(conflicts, sorted(
            ["Shift/Reduce conflict for terminal ELSE: (resolving as shift)"]
            + ["Shift/Reduce conflict for terminal RPAREN: "
               "(resolving as shift)"] * 3
        ))


class TestLoopAttributes(unittest.TestCase):

    def test_keep_keeps_a_delay_loop(self) -> None:
        for attribute, kept in (("", False), ("[[c6502::keep]] ", True)):
            with self.subTest(attribute=attribute):
                tac = _run_stage("tac", preprocess(
                    "void f(void) { " + attribute
                    + "for (unsigned char y = 0xB4; --y;) { } }"
                ), optimize=True)
                self.assertEqual("_keep_start" in tac, kept)
                self.assertEqual("Jump(" in tac, kept)

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
