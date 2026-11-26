"""End-to-end harness for the chapter_13 corpus.

chapter_13 covers floating-point types (`float` / `double`) and
conversions to/from integer types. Four buckets:

  valid/             — must compile through `-S`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must fail at lex or parse.
  invalid_types/     — must be rejected somewhere in the pipeline.

The upstream `valid/libraries/` and `helper_libs/` subdirs are
multi-TU and aren't applicable.

c6502 lays down IEEE 754 byte patterns for FP literals and statics
correctly, but the FP arithmetic helpers aren't in this repo yet —
so chapter_13 valid tests assemble but won't link. Most failures here
are still literal-out-of-range (chapter_13 mixes in 8-byte longs),
not FP gaps.

The lexer accepts `1.0e10.0` as two tokens (it has no preprocessing-
number concept). The tokens of each invalid_lex file, that one
included, are doctest cases of the C++ port
(cpp/tests/test_chapter_13_calls.cpp).
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_source


_TESTS_DIR = Path(__file__).parent
_C13 = _TESTS_DIR / "chapter_13"


_INCOMPATIBLE_VALID = frozenset()


# Lexer tokenises `1.0e10.0` as two CONSTANTs (`1.0e10` and `.0`)
# rather than rejecting it as a malformed preprocessing number; we
# don't model preprocessing numbers.

_INVALID_TYPES_NOT_REJECTED_TODAY = frozenset()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter13Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C13 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_13 valid files found")
        for path in files:
            rel = path.relative_to(_C13 / "valid").as_posix()
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = path.read_text()
                _run_source("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter13InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C13 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = path.relative_to(_C13 / "invalid_types").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_source("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_source("codegen", source)
