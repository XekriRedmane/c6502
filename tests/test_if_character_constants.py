"""Tests for character constants in an `#if` (preprocessor.py's
`_without_named_escapes`).

pcpp's evaluator expands a character constant's escapes with Python's
`unicode-escape` codec, which also reads Python's `\\N{name}`. C has no
such escape (C23 §6.4.4.4), and c6502's lexer rejects
`'\\N{LATIN SMALL LETTER A}'` outside an `#if`, so in an `#if` it gets
pcpp's error for a constant that isn't one character, as `'\\q'` does,
and the `#if` takes its false branch.

Coverage:

  * The C escapes keep their values in an `#if`.
  * `'\\q'` and `'\\N{LATIN SMALL LETTER A}'` each get pcpp's report of
    the TypeError of `ord()`, with the constant's length, and the
    group after the `#if` is skipped.
  * Escapes next to a `\\N` expand as they would without it, and a
    `\\u` escape that is too short to take the `\\N` still fails with
    the same UnicodeDecodeError.
  * Outside an `#if`, both constants are lex errors.
"""
from __future__ import annotations

import contextlib
import io
import unittest

from lexer import LexError, tokenize
from preprocessor import _without_named_escapes, preprocess


def _if(condition: str) -> tuple[str, str]:
    """Returns the text and the stderr of preprocessing an `#if` on
    `condition` with a line in each branch."""
    err = io.StringIO()
    with contextlib.redirect_stderr(err):
        out = preprocess(
            f"#if {condition}\nint yes;\n#else\nint no;\n#endif\n", [],
            "t.c",
        )
    return out, err.getvalue()


class TestCEscapes(unittest.TestCase):
    def test_values(self) -> None:
        cases = {
            r"'\n'": 10, r"'\t'": 9, r"'\\'": 92, r"'\''": 39,
            r"'\x41'": 65, r"'\101'": 65, r"'\0'": 0, r"'a'": 97,
            r"L'b'": 98,
        }
        for constant, value in cases.items():
            with self.subTest(constant=constant):
                out, err = _if(f"{constant} == {value}")
                self.assertIn("int yes;", out)
                self.assertEqual(err, "")


class TestInvalidEscapes(unittest.TestCase):
    def test_unknown_escape(self) -> None:
        out, err = _if(r"'\q' == 113")
        self.assertNotIn("int yes;", out)
        self.assertEqual(
            err,
            "t.c:1 error: Could not evaluate expression due to "
            "TypeError('ord() expected a character, but string of length "
            "2 found') (passed to evaluator: ''\\q' == 113')\n",
        )

    def test_named_escape_is_not_an_escape(self) -> None:
        # pcpp used to give this the value 97.
        out, err = _if(r"'\N{LATIN SMALL LETTER A}' == 97")
        self.assertNotIn("int yes;", out)
        self.assertIn("int no;", out)
        self.assertEqual(
            err,
            "t.c:1 error: Could not evaluate expression due to "
            "TypeError('ord() expected a character, but string of length "
            "24 found') (passed to evaluator: "
            "''\\N{LATIN SMALL LETTER A}' == 97')\n",
        )

    def test_named_escape_in_a_wide_constant(self) -> None:
        out, err = _if(r"L'\N{DIGIT ONE}' == 49")
        self.assertNotIn("int yes;", out)
        self.assertIn("string of length 13 found", err)


class TestRewriting(unittest.TestCase):
    def test_spellings(self) -> None:
        # Each backslash the expansion leaves is doubled, and every other
        # escape is expanded.
        cases = {
            r"'\N{A}'": r"'\\N{A}'",
            r"'\N{\x41}'": r"'\\N{A}'",
            r"'\N{\N{A}}'": r"'\\N{\\N{A}}'",
            r"L'\N'": r"L'\\N'",
            r"'\\N{A}'": r"'\\N{A}'",
            r"'\n\N'": "'\n\\\\N'",
        }
        for spelling, rewritten in cases.items():
            with self.subTest(spelling=spelling):
                self.assertEqual(_without_named_escapes(spelling), rewritten)

    def test_failing_escape_is_kept(self) -> None:
        # `\u` has only three characters after it here, so it isn't an
        # escape, and the backslash of `\N` is written out.
        self.assertEqual(_without_named_escapes(r"'\u0\N'"), r"'\\u0\\N'")
        # `\x` takes `zz`, which the codec can't decode, so the text from
        # there on is left for pcpp, which reports the same error.
        self.assertEqual(
            _without_named_escapes(r"'\N\xzz\N'"), r"'\\N\xzz\N'",
        )
        out, err = _if(r"'\N\xzz\N' == 0")
        self.assertIn(
            "UnicodeDecodeError('unicodeescape', b'\\\\xzz', 0, 2, "
            "'truncated \\\\xXX escape')",
            err,
        )


class TestOutsideIf(unittest.TestCase):
    def test_lex_errors(self) -> None:
        for constant in (r"'\q'", r"'\N{LATIN SMALL LETTER A}'"):
            with self.subTest(constant=constant):
                with self.assertRaises(LexError):
                    list(tokenize(f"int x = {constant};"))


if __name__ == "__main__":
    unittest.main()
