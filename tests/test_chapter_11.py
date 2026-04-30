"""End-to-end harness for the chapter_11 corpus.

chapter_11 introduces `long` integer types and integer-type
conversions. Five buckets:

  valid/             — must compile through `--codegen`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must fail at lex or parse.
  invalid_labels/    — must be rejected somewhere in the pipeline.
  invalid_types/     — must be rejected somewhere in the pipeline.

The upstream `valid/libraries/` subdir is multi-TU and isn't applicable.

chapter_11's `valid/` files were locally rewritten to substitute
c6502's 4-byte `long long` for upstream's 8-byte `long`, with
literal magnitudes scaled from the 8-byte to the 4-byte range
(see the per-file comments). Test semantics survive: multi-byte
arithmetic, common-type promotion, sign-/zero-extension,
truncation, switch-on-wide-int, and rewrite rules for
constants beyond the immediate-byte range.
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
_C11 = _TESTS_DIR / "chapter_11"


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
class TestChapter11Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C11 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_11 valid files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "valid"))
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
class TestChapter11InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C11 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(LexError):
                    list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C11 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11InvalidLabels(unittest.TestCase):
    """Each chapter_11/invalid_labels file must be rejected somewhere
    in the pipeline. We accept any Exception as rejection so a noisy
    crash-on-bad-input still counts (we don't want bugs in the
    type checker to silently accept invalid programs)."""

    def test_codegen_rejects(self):
        files = sorted((_C11 / "invalid_labels").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_labels files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_labels"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C11 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)
