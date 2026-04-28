"""End-to-end harness for the chapter_6 corpus.

chapter_6 covers `if` / `else`, the ternary `?:`, and `goto` /
labeled statements. Four buckets:

  valid/             — must compile through `--codegen`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must lex cleanly but fail at parse.
  invalid_semantics/ — must parse cleanly but fail at one of the
                       semantic passes (identifier_resolution,
                       label_resolution, loop_labeling,
                       type_checking).
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
_C6 = _TESTS_DIR / "chapter_6"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter6Valid(unittest.TestCase):
    """Each chapter_6/valid file must compile through `--codegen`."""

    def test_codegen(self):
        files = sorted((_C6 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_6 valid files found")
        for path in files:
            rel = str(path.relative_to(_C6 / "valid"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter6InvalidLex(unittest.TestCase):
    """Each chapter_6/invalid_lex file must fail at the lexer."""

    def test_lex_fails(self):
        files = sorted((_C6 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C6 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(LexError):
                    list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter6InvalidParse(unittest.TestCase):
    """Each chapter_6/invalid_parse file must fail at lex or parse."""

    def test_parse_fails(self):
        files = sorted((_C6 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C6 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter6InvalidSemantics(unittest.TestCase):
    """Each chapter_6/invalid_semantics file must parse cleanly but
    fail in one of the semantic passes."""

    def test_resolve_fails(self):
        files = sorted((_C6 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = str(path.relative_to(_C6 / "invalid_semantics"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                parse(source)
                with self.assertRaises(_SEMANTIC_FAILURES):
                    _run_stage("codegen", source)
