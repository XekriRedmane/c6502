"""End-to-end harness for the chapter_6 corpus.

chapter_6 covers `if` / `else`, the ternary `?:`, and `goto` /
labeled statements. Four buckets:

  valid/             — must compile through `-S`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must lex cleanly but fail at parse.
  invalid_semantics/ — must parse cleanly but fail at one of the
                       semantic passes (identifier_resolution,
                       label_resolution, loop_labeling,
                       type_checking).
"""

import unittest
from pathlib import Path

from compile import _run_source
from errors import (
    IdentifierResolutionError, LabelResolutionError, LoopLabelingError,
    TypeCheckError,
)
from tests.cpp_build import needs_c6502cpp


_TESTS_DIR = Path(__file__).parent
_C6 = _TESTS_DIR / "chapter_6"


_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


@needs_c6502cpp
class TestChapter6Valid(unittest.TestCase):
    """Each chapter_6/valid file must compile through `-S`."""

    def test_codegen(self):
        files = sorted((_C6 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_6 valid files found")
        for path in files:
            rel = path.relative_to(_C6 / "valid").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                _run_source("codegen", source)


@needs_c6502cpp
class TestChapter6InvalidSemantics(unittest.TestCase):
    """Each chapter_6/invalid_semantics file must parse cleanly but
    fail in one of the semantic passes."""

    def test_resolve_fails(self):
        files = sorted((_C6 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = path.relative_to(_C6 / "invalid_semantics").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                with self.assertRaises(_SEMANTIC_FAILURES):
                    _run_source("codegen", source)
