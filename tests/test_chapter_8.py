"""End-to-end harness for the chapter_8 corpus.

chapter_8 covers iteration statements (`while`, `do`/`while`, `for`),
`break` / `continue`, and `switch`. Three buckets:

  valid/             — must compile through `-S`.
  invalid_parse/     — must fail at lex or parse.
  invalid_semantics/ — must be rejected somewhere in the pipeline.

`empty_loop_body.c` and `for_absent_post.c` were modified locally
to use 1-byte int constants — same loop / switch semantics, just
scaled to fit.
"""

import unittest
from pathlib import Path

from compile import _run_source
from lark.exceptions import UnexpectedInput
from lexer import LexError
from parser import ParserError
from passes.identifier_resolution import IdentifierResolutionError
from passes.label_resolution import LabelResolutionError
from passes.loop_labeling import LoopLabelingError
from passes.type_checking import TypeCheckError
from tests.cpp_build import needs_c6502cpp


_TESTS_DIR = Path(__file__).parent
_C8 = _TESTS_DIR / "chapter_8"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)
_ANY_REJECTION = _PARSE_FAILURES + _SEMANTIC_FAILURES


_INCOMPATIBLE_VALID = frozenset()


_EXPECTED_FAILURES_CODEGEN = frozenset()


@needs_c6502cpp
class TestChapter8Valid(unittest.TestCase):
    """Each chapter_8/valid file must compile through `-S`,
    except those listed in `_EXPECTED_FAILURES_CODEGEN` (must fail) or
    `_INCOMPATIBLE_VALID` (skipped)."""

    def test_codegen(self):
        files = sorted((_C8 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_8 valid files found")
        for path in files:
            rel = path.relative_to(_C8 / "valid").as_posix()
            if rel in _INCOMPATIBLE_VALID:
                continue
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
class TestChapter8InvalidSemantics(unittest.TestCase):
    """Each chapter_8/invalid_semantics file must be rejected
    somewhere in the pipeline (parse or any semantic pass)."""

    def test_codegen_rejects(self):
        files = sorted((_C8 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = path.relative_to(_C8 / "invalid_semantics").as_posix()
            with self.subTest(file=rel):
                source = path.read_text()
                with self.assertRaises(_ANY_REJECTION):
                    _run_source("codegen", source)
