"""End-to-end harness for the chapter_9 corpus.

chapter_9 covers function declarations, definitions, calls, and
arguments. Five buckets:

  valid/                 — must compile through `--codegen`.
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

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError
from parser import ParserError, parse
from preprocessor import preprocess
from passes.identifier_resolution import IdentifierResolutionError
from passes.label_resolution import LabelResolutionError
from passes.loop_labeling import LoopLabelingError
from passes.type_checking import TypeCheckError


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


_INCOMPATIBLE_VALID = frozenset([
    # Literal '10000000' beyond 16-bit Long range.
    "stack_arguments/test_for_memory_leaks.c",
])


# invalid_parse files the parser currently accepts. Drop entries as
# the parser learns to reject them.
_INVALID_PARSE_NOT_REJECTED_TODAY = frozenset()

_INVALID_DECL_NOT_REJECTED_TODAY = frozenset()


def _run_codegen_subtest(test, files, root, not_rejected):
    """Each file in `files` is expected to be rejected somewhere in
    the pipeline. Files in `not_rejected` are pinned at their current
    accept-it behavior — when the compiler grows the rejection, the
    pinned assertion fails and prompts a drop."""
    test.assertGreater(len(files), 0, f"no files in {root}")
    for path in files:
        rel = str(path.relative_to(root))
        with test.subTest(file=rel):
            source = preprocess(path.read_text(), [])
            if rel in not_rejected:
                _run_stage("codegen", source)
            else:
                with test.assertRaises(_ANY_REJECTION):
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter9Valid(unittest.TestCase):
    """Each chapter_9/valid file must compile through `--codegen`."""

    def test_codegen(self):
        files = sorted((_C9 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_9 valid files found")
        for path in files:
            rel = str(path.relative_to(_C9 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter9InvalidParse(unittest.TestCase):
    """Each chapter_9/invalid_parse file must fail at lex or parse,
    except those pinned at their current accept-it behavior in
    `_INVALID_PARSE_NOT_REJECTED_TODAY`."""

    def test_parse_fails(self):
        files = sorted((_C9 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C9 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter9InvalidDeclarations(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C9 / "invalid_declarations").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C9 / "invalid_declarations",
            _INVALID_DECL_NOT_REJECTED_TODAY,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter9InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C9 / "invalid_labels").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C9 / "invalid_labels", frozenset(),
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter9InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C9 / "invalid_types").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C9 / "invalid_types", frozenset(),
        )
