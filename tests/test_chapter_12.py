"""End-to-end harness for the chapter_12 corpus.

chapter_12 introduces unsigned integer types and signed/unsigned
conversions. Five buckets:

  valid/             — must compile through `--codegen`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must fail at lex or parse.
  invalid_labels/    — must be rejected somewhere in the pipeline.
  invalid_types/     — must be rejected somewhere in the pipeline.

Most chapter_12 tests use 32+ bit unsigned literals (the upstream
target's `unsigned int` is 4 bytes; c6502's is 1). Files that don't
fit go into `_INCOMPATIBLE_VALID`, plus rewrite_movz_regression.c
which initializes a 1-byte unsigned with 5000.
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
_C12 = _TESTS_DIR / "chapter_12"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


_INCOMPATIBLE_VALID = frozenset([
    "explicit_casts/chained_casts.c",
    "explicit_casts/extension.c",
    "explicit_casts/rewrite_movz_regression.c",
    "explicit_casts/round_trip_casts.c",
    "explicit_casts/same_size_conversion.c",
    "explicit_casts/truncate.c",
    "extra_credit/bitwise_unsigned_ops.c",
    "extra_credit/bitwise_unsigned_shift.c",
    "extra_credit/compound_assign_uint.c",
    "extra_credit/compound_bitshift.c",
    "extra_credit/compound_bitwise.c",
    "extra_credit/postfix_precedence.c",
    "extra_credit/unsigned_incr_decr.c",
    "implicit_casts/common_type.c",
    "implicit_casts/convert_by_assignment.c",
    "implicit_casts/promote_constants.c",
    "implicit_casts/static_initializers.c",
    "type_specifiers/unsigned_type_specifiers.c",
    "unsigned_expressions/arithmetic_ops.c",
    "unsigned_expressions/arithmetic_wraparound.c",
    "unsigned_expressions/comparisons.c",
    "unsigned_expressions/locals.c",
    "unsigned_expressions/logical.c",
    "unsigned_expressions/simple.c",
    "unsigned_expressions/static_variables.c",
    # Switch test with case constants beyond c6502's 16-bit ULong.
    "extra_credit/switch_uint.c",
])


_EXPECTED_FAILURES_CODEGEN = frozenset()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C12 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_12 valid files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _EXPECTED_FAILURES_CODEGEN:
                    with self.assertRaises(
                        Exception,
                        msg=(f"{rel} unexpectedly compiled — drop "
                             "from _EXPECTED_FAILURES_CODEGEN"),
                    ):
                        _run_stage("codegen", source)
                else:
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C12 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(LexError):
                    list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C12 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C12 / "invalid_labels").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_labels files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_labels"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter12InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C12 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C12 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)
