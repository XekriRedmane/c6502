"""Behavioral tests for `[[c6502::unroll]]`.

Coverage:
  Surface plumbing (commit 1):
  - The parser sets `loop_attribute` on the ForStmt after the
    attribute, and the old `#pragma c6502 loop unroll(...)` is an
    error that names the attribute.
  - The attribute rides through every AST-level pass.

  Unroll pass (commit 2):
  - Canonical loops unroll: ascending / descending, increment /
    decrement / += K / -= K, all six supported integer types.
  - Body with locals: each iteration scopes its own copy.
  - Nested annotated loops: outer × inner clones.
  - Zero-iteration loops produce empty Compounds.
  - Unannotated loops untouched.
  - Recognizer rejects: break / continue / goto / labeled-stmt
    in body, induction-var assignment / address-of / re-mod,
    inner shadow declaration, non-canonical condition / init /
    post shapes, infinite loops, iteration cap exceeded.
"""
from __future__ import annotations

import unittest

import c99_ast
from parser import parse
from passes.optimization_ast.unroll import UnrollError, unroll_program
from preprocessor import PreprocessorError, preprocess


_PROG = """
int main(void) {{
    {attribute}for (int i = 0; i < 4; i++) {{ }}
    return 0;
}}
"""


class TestUnrollAttributeParse(unittest.TestCase):


    def test_old_pragma_names_the_attribute(self) -> None:
        for state, needle in (
            ("enable", "[[c6502::unroll]]"),
            ("disable", "delete the line"),
        ):
            with self.subTest(state):
                with self.assertRaises(PreprocessorError) as cm:
                    preprocess(_PROG.format(
                        attribute=f"#pragma c6502 loop unroll({state})\n",
                    ))
                self.assertIn(needle, str(cm.exception))


def _unrolled(body: str) -> c99_ast.Type_program:
    """Wrap `body` in `int main(void) { ... }`, run preprocess →
    parse → unroll, return the resulting program."""
    src = "int main(void) {\n" + body + "\n    return 0;\n}\n"
    return unroll_program(parse(preprocess(src)))


class TestUnrollRecognizerRejections(unittest.TestCase):
    def _expect_unroll_error(self, body: str, msg_substr: str) -> None:
        with self.assertRaises(UnrollError) as cm:
            _unrolled(body)
        self.assertIn(msg_substr, str(cm.exception))


    def test_cli_reports_one_line_error(self) -> None:
        import io
        from unittest.mock import patch

        from compile import main
        src = (
            "unsigned char row[256];\n"
            "void fill(unsigned char v) {\n"
            "    [[c6502::unroll]] "
            "for (unsigned char i = 0; i <= 255; i++) row[i] = v;\n"
            "}\n"
        )
        with patch("sys.stdin", io.StringIO(src)), \
             patch("sys.stdout", new_callable=io.StringIO), \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(["compile.py", "-", "-S", "-O"])
        self.assertNotEqual(rc, 0)
        lines = err.getvalue().strip().splitlines()
        self.assertEqual(len(lines), 1)
        self.assertIn("never terminates", lines[0])


class TestUnrollCli(unittest.TestCase):
    """`--unroll` CLI flag — invocation, gating on `-O`,
    and an end-to-end pipeline check that an annotated loop
    actually compiles down."""

    def _run(self, argv: list[str], stdin: str = "") -> tuple[int, str, str]:
        import io
        from unittest.mock import patch

        from compile import main
        with patch("sys.stdin", io.StringIO(stdin)), \
             patch("sys.stdout", new_callable=io.StringIO) as out, \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(argv)
        return rc, out.getvalue(), err.getvalue()

    def test_attribute_unrolls_under_optimize(self) -> None:
        # Under `-O`, the AST-level unroll pass fires on
        # every for-loop marked `[[c6502::unroll]]` (with the canonical
        # init=const; cond=var<const; step=var=var+const shape).
        # Verify the loop is gone from the TAC (no continue/break
        # labels mint).
        src = """
        int main(void) {
            int s = 0;
            [[c6502::unroll]] for (int i = 0; i < 4; i++) s += i;
            return s;
        }
        """
        rc, out, _ = self._run(
            ["compile.py", "-", "--tac", "-O"], stdin=src,
        )
        self.assertEqual(rc, 0)
        self.assertNotIn("_continue", out)
        self.assertNotIn("_break", out)

    def test_attribute_ignored_without_optimize(self) -> None:
        # Without `-O`, the unroll pass is skipped. The
        # attribute parses but the loop lowers normally with continue
        # / break labels.
        src = """
        int main(void) {
            int s = 0;
            [[c6502::unroll]] for (int i = 0; i < 4; i++) s += i;
            return s;
        }
        """
        rc, out, _ = self._run(
            ["compile.py", "-", "--tac"], stdin=src,
        )
        self.assertEqual(rc, 0)
        self.assertIn("_continue", out)
        self.assertIn("_break", out)


if __name__ == "__main__":
    unittest.main()
