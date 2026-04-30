"""End-to-end harness for the chapter_12 corpus.

chapter_12 introduces unsigned integer types and signed/unsigned
conversions. Five buckets:

  valid/             — must compile through `--codegen`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must fail at lex or parse.
  invalid_labels/    — must be rejected somewhere in the pipeline.
  invalid_types/     — must be rejected somewhere in the pipeline.

chapter_12's `valid/` files were locally rewritten to substitute
c6502's wider unsigned types for upstream's: `unsigned long`
(2 bytes here vs 8 upstream) replaces upstream's `unsigned int`
in ~half the call sites, and `unsigned long long` (4 bytes)
replaces `unsigned long`. Constants are scaled accordingly. Test
semantics survive: zero-/sign-extension boundaries, unsigned
arithmetic wraparound, common-type promotion, static initializers
across width boundaries.
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
_C12 = _TESTS_DIR / "chapter_12"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


_INCOMPATIBLE_VALID = frozenset()


_EXPECTED_FAILURES_CODEGEN = frozenset()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C12 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_12 valid files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _EXPECTED_FAILURES_CODEGEN:
                    with self.assertRaises(
                        Exception,
                        msg=(f"{rel} unexpectedly compiled — drop "
                             "from _EXPECTED_FAILURES_CODEGEN"),
                    ):
                        _run_stage("codegen", source)
                else:
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C12 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(LexError):
                    list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C12 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C12 / "invalid_labels").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_labels files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_labels"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C12 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)
