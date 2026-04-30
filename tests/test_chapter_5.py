"""End-to-end harness for the chapter_5 corpus.

chapter_5 covers local variable declarations, assignment, compound
assignment (`+=` etc.), and pre/post increment/decrement. Three
buckets:

  valid/             — must compile through `--codegen`.
  invalid_parse/     — must fail at lex or parse.
  invalid_semantics/ — must parse cleanly but fail at one of the
                       semantic passes (identifier_resolution,
                       label_resolution, loop_labeling,
                       type_checking).

Two `valid/` files (`allocate_temps_and_vars.c` and
`extra_credit/compound_bitwise_shiftr.c`) were modified locally to
fit c6502's 1-byte int range — same operator semantics, scaled
constants.
"""

import shutil
import unittest
from pathlib import Path

from compile import _resolved, _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError
from parser import ParserError, parse
from preprocessor import preprocess
from passes.identifier_resolution import IdentifierResolutionError
from passes.label_resolution import LabelResolutionError
from passes.loop_labeling import LoopLabelingError
from passes.type_checking import TypeCheckError


_TESTS_DIR = Path(__file__).parent
_C5 = _TESTS_DIR / "chapter_5"


_INCOMPATIBLE_VALID = frozenset()


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter5Valid(unittest.TestCase):
    """Each chapter_5/valid file must compile through `--codegen`."""

    def test_codegen(self):
        files = sorted((_C5 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_5 valid files found")
        for path in files:
            rel = str(path.relative_to(_C5 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter5InvalidParse(unittest.TestCase):
    """Each chapter_5/invalid_parse file must fail at lex or parse."""

    def test_parse_fails(self):
        files = sorted((_C5 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C5 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter5InvalidSemantics(unittest.TestCase):
    """Each chapter_5/invalid_semantics file must parse cleanly but
    fail in one of the semantic passes (identifier_resolution,
    label_resolution, loop_labeling, type_checking)."""

    def test_resolve_fails(self):
        files = sorted((_C5 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = str(path.relative_to(_C5 / "invalid_semantics"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                # Parse must succeed.
                parse(source)
                # A semantic pass must reject.
                with self.assertRaises(_SEMANTIC_FAILURES):
                    _run_stage("codegen", source)
