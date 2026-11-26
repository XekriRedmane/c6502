"""End-to-end harness for the chapter_7 corpus.

chapter_7 covers compound statements (block scopes) — `{ ... }` as a
statement, nested scopes, variable shadowing, and goto across
scopes. Three buckets:

  valid/             — must compile through `-S`.
  invalid_parse/     — must fail at lex or parse.
  invalid_semantics/ — must parse cleanly but fail at one of the
                       semantic passes.
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
_C7 = _TESTS_DIR / "chapter_7"


_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


@needs_c6502cpp
class TestChapter7Valid(unittest.TestCase):
    """Each chapter_7/valid file must compile through `-S`."""

    def test_codegen(self):
        files = sorted((_C7 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_7 valid files found")
        for path in files:
            rel = path.relative_to(_C7 / "valid").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                _run_source("codegen", source)


@needs_c6502cpp
class TestChapter7InvalidSemantics(unittest.TestCase):
    """Each chapter_7/invalid_semantics file must parse cleanly but
    fail in one of the semantic passes."""

    def test_resolve_fails(self):
        files = sorted((_C7 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = path.relative_to(_C7 / "invalid_semantics").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                with self.assertRaises(_SEMANTIC_FAILURES):
                    _run_source("codegen", source)
