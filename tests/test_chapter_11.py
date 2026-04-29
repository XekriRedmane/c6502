"""End-to-end harness for the chapter_11 corpus.

chapter_11 introduces `long` integer types and integer-type
conversions. Five buckets:

  valid/             — must compile through `--codegen`.
  invalid_lex/       — must fail at the lexer.
  invalid_parse/     — must fail at lex or parse.
  invalid_labels/    — must be rejected somewhere in the pipeline.
  invalid_types/     — must be rejected somewhere in the pipeline.

The upstream `valid/libraries/` subdir is multi-TU and isn't applicable.

Most chapter_11 tests use 32+ bit integer literals (the upstream
target's `long` is 8 bytes; c6502's is 2). Files where the only
fitting type would be `long long` go into `_INCOMPATIBLE_VALID`. Two
`switch` tests are listed in `_EXPECTED_FAILURES_CODEGEN` until c6502
grows a switch implementation.
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
_C11 = _TESTS_DIR / "chapter_11"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)


_INCOMPATIBLE_VALID = frozenset([
    # Literal exceeds c6502's 16-bit Long range.
    "explicit_casts/truncate.c",
    "extra_credit/bitshift.c",
    "extra_credit/bitwise_long_op.c",
    "extra_credit/compound_assign_to_int.c",
    "extra_credit/compound_assign_to_long.c",
    "extra_credit/compound_bitshift.c",
    "extra_credit/compound_bitwise.c",
    "extra_credit/increment_long.c",
    "implicit_casts/common_type.c",
    "implicit_casts/convert_by_assignment.c",
    "implicit_casts/convert_function_arguments.c",
    "implicit_casts/convert_static_initializer.c",
    "implicit_casts/long_constants.c",
    "long_expressions/arithmetic_ops.c",
    "long_expressions/assign.c",
    "long_expressions/comparisons.c",
    "long_expressions/large_constants.c",
    "long_expressions/logical.c",
    "long_expressions/long_and_int_locals.c",
    "long_expressions/long_args.c",
    "long_expressions/multi_op.c",
    "long_expressions/return_long.c",
    "long_expressions/rewrite_large_multiply_regression.c",
    "long_expressions/simple.c",
    "long_expressions/static_long.c",
    "long_expressions/type_specifiers.c",
])


_EXPECTED_FAILURES_CODEGEN = frozenset([
    # `switch` keyword lexes but no grammar rule accepts it.
    "extra_credit/switch_int.c",
    "extra_credit/switch_long.c",
])


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C11 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_11 valid files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "valid"))
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
class TestChapter11InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C11 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(LexError):
                    list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C11 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11InvalidLabels(unittest.TestCase):
    """Each chapter_11/invalid_labels file must be rejected somewhere
    in the pipeline. We accept any Exception as rejection so a noisy
    crash-on-bad-input still counts (we don't want bugs in the
    type checker to silently accept invalid programs)."""

    def test_codegen_rejects(self):
        files = sorted((_C11 / "invalid_labels").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_labels files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_labels"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter11InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C11 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C11 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)
