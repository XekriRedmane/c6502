"""End-to-end harness for the chapter_13 corpus.

chapter_13 covers floating-point types (`float` / `double`) and
conversions to/from integer types. Four buckets:

  valid/             — must compile through `--codegen`.
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
number concept); that file is pinned in
`_INVALID_LEX_NOT_REJECTED_TODAY`.
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError, tokenize
from parser import ParserError, parse
from preprocessor import preprocess
from passes.identifier_resolution import IdentifierResolutionError
from passes.label_resolution import LabelResolutionError
from passes.loop_labeling import LoopLabelingError
from passes.type_checking import TypeCheckError


_TESTS_DIR = Path(__file__).parent
_C13 = _TESTS_DIR / "chapter_13"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


_INCOMPATIBLE_VALID = frozenset()


# Lexer tokenises `1.0e10.0` as two CONSTANTs (`1.0e10` and `.0`)
# rather than rejecting it as a malformed preprocessing number; we
# don't model preprocessing numbers.
_INVALID_LEX_NOT_REJECTED_TODAY = frozenset([
    "malformed_exponent.c",
])

_INVALID_TYPES_NOT_REJECTED_TODAY = frozenset()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter13Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C13 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_13 valid files found")
        for path in files:
            rel = str(path.relative_to(_C13 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter13InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C13 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C13 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_LEX_NOT_REJECTED_TODAY:
                    list(tokenize(source))
                else:
                    with self.assertRaises(LexError):
                        list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter13InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C13 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C13 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter13InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C13 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C13 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
