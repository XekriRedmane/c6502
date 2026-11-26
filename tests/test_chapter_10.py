"""End-to-end harness for the chapter_10 corpus.

chapter_10 covers file-scope variables, the `static` and `extern`
storage classes, tentative definitions, and linkage. Five buckets:

  valid/                 — must compile through `-S`.
  invalid_parse/         — must fail at lex or parse.
  invalid_declarations/  — must be rejected in the pipeline.
  invalid_labels/        — must be rejected in the pipeline.
  invalid_types/         — must be rejected in the pipeline.

The upstream `valid/libraries/` subdir is multi-TU and isn't applicable;
the `data_on_page_boundary_*.s` files are platform-specific x86 ASM.
We don't import either.

A handful of files aren't yet rejected (or compiled) by c6502 — they're
listed per-bucket and pinned at their current behavior, so a regression
OR a fix both flag the test as failing and prompt a drop.
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
_C10 = _TESTS_DIR / "chapter_10"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)
_ANY_REJECTION = _PARSE_FAILURES + _SEMANTIC_FAILURES


_EXPECTED_FAILURES_CODEGEN = frozenset()


_INVALID_DECL_NOT_REJECTED_TODAY = frozenset()


def _run_codegen_subtest(test, files, root, not_rejected):
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
class TestChapter10Valid(unittest.TestCase):
    """Each chapter_10/valid file must compile through `-S`,
    except those listed in `_EXPECTED_FAILURES_CODEGEN`."""

    def test_codegen(self):
        files = sorted((_C10 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_10 valid files found")
        for path in files:
            rel = path.relative_to(_C10 / "valid").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                if rel in _EXPECTED_FAILURES_CODEGEN:
                    with self.assertRaises(
                        Exception,
                        msg=(
                            f"{rel} unexpectedly compiled — drop it "
                            "from _EXPECTED_FAILURES_CODEGEN"
                        ),
                    ):
                        _run_source("codegen", source)
                else:
                    _run_source("codegen", source)


@needs_c6502cpp
class TestChapter10InvalidDeclarations(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C10 / "invalid_declarations").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C10 / "invalid_declarations",
            _INVALID_DECL_NOT_REJECTED_TODAY,
        )


@needs_c6502cpp
class TestChapter10InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C10 / "invalid_labels").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C10 / "invalid_labels", frozenset(),
        )


@needs_c6502cpp
class TestChapter10InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C10 / "invalid_types").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C10 / "invalid_types", frozenset(),
        )
