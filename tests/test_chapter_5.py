"""End-to-end harness for the chapter_5 corpus.

chapter_5 covers local variable declarations, assignment, compound
assignment (`+=` etc.), and pre/post increment/decrement. Three
buckets:

  valid/             — must compile through `-S`.
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

from compile import _run_stage
from preprocessor import preprocess
from passes.identifier_resolution import IdentifierResolutionError
from passes.label_resolution import LabelResolutionError
from passes.loop_labeling import LoopLabelingError
from passes.type_checking import TypeCheckError


_TESTS_DIR = Path(__file__).parent
_C5 = _TESTS_DIR / "chapter_5"


_INCOMPATIBLE_VALID = frozenset()


_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter5Valid(unittest.TestCase):
    """Each chapter_5/valid file must compile through `-S`."""

    def test_codegen(self):
        files = sorted((_C5 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_5 valid files found")
        for path in files:
            rel = path.relative_to(_C5 / "valid").as_posix()
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter5InvalidSemantics(unittest.TestCase):
    """Each chapter_5/invalid_semantics file must parse cleanly but
    fail in one of the semantic passes (identifier_resolution,
    label_resolution, loop_labeling, type_checking)."""

    def test_resolve_fails(self):
        files = sorted((_C5 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = path.relative_to(_C5 / "invalid_semantics").as_posix()
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                # Parse must succeed.
                # A semantic pass must reject.
                with self.assertRaises(_SEMANTIC_FAILURES):
                    _run_stage("codegen", source)
