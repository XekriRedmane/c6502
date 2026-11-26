"""End-to-end harness for the chapter_9 corpus.

chapter_9 covers function declarations, definitions, calls, and
arguments. Five buckets:

  valid/                 — must compile through `-S`.
  invalid_parse/         — must fail at lex or parse.
  invalid_declarations/  — must be rejected in the pipeline.
  invalid_labels/        — must be rejected in the pipeline.
  invalid_types/         — must be rejected in the pipeline.

The upstream `valid/libraries/` subdir is multi-TU linking and isn't
applicable; we don't import it.

A few invalid_* files aren't rejected by the current compiler; they're
listed in `_NOT_REJECTED_TODAY` per bucket. The harness pins those at
their current accept-it behavior so a regression OR a fix both flag
the test as failing — at which point you drop the entry.
"""

import unittest
from pathlib import Path

from compile import _run_source
from errors import (
    IdentifierResolutionError, LabelResolutionError, LexError,
    LoopLabelingError, ParserError, TypeCheckError, UnexpectedInput,
)
from tests.cpp_build import needs_c6502cpp


_TESTS_DIR = Path(__file__).parent
_C9 = _TESTS_DIR / "chapter_9"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)
_ANY_REJECTION = _PARSE_FAILURES + _SEMANTIC_FAILURES


_INCOMPATIBLE_VALID = frozenset()


# invalid_parse files the parser currently accepts. Drop entries as
# the parser learns to reject them.

_INVALID_DECL_NOT_REJECTED_TODAY = frozenset()


def _run_codegen_subtest(test, files, root, not_rejected):
    """Each file in `files` is expected to be rejected somewhere in
    the pipeline. Files in `not_rejected` are pinned at their current
    accept-it behavior — when the compiler grows the rejection, the
    pinned assertion fails and prompts a drop."""
    test.assertGreater(len(files), 0, f"no files in {root}")
    for path in files:
        rel = path.relative_to(root).as_posix()
        with test.subTest(file=rel):
            source = path.read_text()
            if rel in not_rejected:
                _run_source("codegen", source)
            else:
                with test.assertRaises(_ANY_REJECTION):
                    _run_source("codegen", source)


@needs_c6502cpp
class TestChapter9Valid(unittest.TestCase):
    """Each chapter_9/valid file must compile through `-S`."""

    def test_codegen(self):
        files = sorted((_C9 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_9 valid files found")
        for path in files:
            rel = path.relative_to(_C9 / "valid").as_posix()
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = path.read_text()
                _run_source("codegen", source)


@needs_c6502cpp
class TestChapter9InvalidDeclarations(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C9 / "invalid_declarations").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C9 / "invalid_declarations",
            _INVALID_DECL_NOT_REJECTED_TODAY,
        )


@needs_c6502cpp
class TestChapter9InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C9 / "invalid_labels").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C9 / "invalid_labels", frozenset(),
        )


@needs_c6502cpp
class TestChapter9InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C9 / "invalid_types").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C9 / "invalid_types", frozenset(),
        )
