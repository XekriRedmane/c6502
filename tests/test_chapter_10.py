"""End-to-end harness for the chapter_10 corpus.

chapter_10 covers file-scope variables, the `static` and `extern`
storage classes, tentative definitions, and linkage. Five buckets:

  valid/                 — must compile through `--codegen`.
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


_INVALID_PARSE_NOT_REJECTED_TODAY = frozenset()

_INVALID_DECL_NOT_REJECTED_TODAY = frozenset()


def _run_codegen_subtest(test, files, root, not_rejected):
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
class TestChapter10Valid(unittest.TestCase):
    """Each chapter_10/valid file must compile through `--codegen`,
    except those listed in `_EXPECTED_FAILURES_CODEGEN`."""

    def test_codegen(self):
        files = sorted((_C10 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_10 valid files found")
        for path in files:
            rel = str(path.relative_to(_C10 / "valid"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _EXPECTED_FAILURES_CODEGEN:
                    with self.assertRaises(
                        Exception,
                        msg=(
                            f"{rel} unexpectedly compiled — drop it "
                            "from _EXPECTED_FAILURES_CODEGEN"
                        ),
                    ):
                        _run_stage("codegen", source)
                else:
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter10InvalidParse(unittest.TestCase):
    """Each chapter_10/invalid_parse file must fail at lex or parse,
    except those pinned in `_INVALID_PARSE_NOT_REJECTED_TODAY`."""

    def test_parse_fails(self):
        files = sorted((_C10 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C10 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter10InvalidDeclarations(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C10 / "invalid_declarations").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C10 / "invalid_declarations",
            _INVALID_DECL_NOT_REJECTED_TODAY,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter10InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C10 / "invalid_labels").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C10 / "invalid_labels", frozenset(),
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter10InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C10 / "invalid_types").rglob("*.c"))
        _run_codegen_subtest(
            self, files, _C10 / "invalid_types", frozenset(),
        )
