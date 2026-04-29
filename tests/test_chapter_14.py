"""End-to-end harness for the chapter_14 corpus.

chapter_14 covers pointer types, the address-of `&` and dereference
`*` operators, and null pointer constants. Four buckets:

  valid/                — must compile through `--codegen`.
  invalid_parse/        — must fail at lex or parse.
  invalid_declarations/ — must be rejected somewhere in the pipeline.
  invalid_types/        — must be rejected somewhere in the pipeline.

The upstream `valid/libraries/` subdir is multi-TU and isn't applicable.

A handful of valid files use 32+ bit literals (chapter_14 mixes in
8-byte longs); they're listed in `_INCOMPATIBLE_VALID`. Two real bugs
are pinned in `_EXPECTED_FAILURES_CODEGEN`:
  - null_pointer_conversion.c — `_int_width` crashes on Pointer when
    the type checker tries to widen a null pointer constant.
  - dereference_expression_result.c — `_common_type(Pointer, Pointer)`
    crashes on a pointer-typed ternary.
A `switch` test rounds out the expected-failures.

Several invalid_parse and invalid_types files exercise behavior we
don't yet reject (mostly null-pointer / pointer-type mismatch
checks); they're pinned in `_NOT_REJECTED_TODAY` per bucket.
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
_C14 = _TESTS_DIR / "chapter_14"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


_INCOMPATIBLE_VALID = frozenset([
    "dereference/read_through_pointers.c",
    "dereference/static_var_indirection.c",
    "dereference/update_through_pointers.c",
    "extra_credit/bitshift_dereferenced_ptrs.c",
    "extra_credit/bitwise_ops_with_dereferenced_ptrs.c",
    "extra_credit/compound_assign_conversion.c",
    "extra_credit/compound_bitwise_dereferenced_ptrs.c",
    "extra_credit/incr_and_decr_through_pointer.c",
])


_EXPECTED_FAILURES_CODEGEN = frozenset([
    # `_int_width` crashes on Pointer when the type checker tries to
    # widen a null pointer constant assigned to a pointer.
    "casts/null_pointer_conversion.c",
    # `_common_type(Pointer, Pointer)` crashes on the ternary
    # branches inside this file.
    "dereference/dereference_expression_result.c",
    # `switch` keyword lexes but no grammar rule accepts it.
    "extra_credit/switch_dereferenced_pointer.c",
])


# invalid_parse files the parser currently accepts.
_INVALID_PARSE_NOT_REJECTED_TODAY = frozenset([
    "abstract_function_declarator.c",
    "malformed_function_declarator.c",
])

# invalid_types files we currently accept (no clean type-check
# rejection for the bad pointer assignments / null-pointer-constant
# misuse / pointer-as-int passing).
_INVALID_TYPES_NOT_REJECTED_TODAY = frozenset([
    "assign_int_to_pointer.c",
    "assign_int_var_to_pointer.c",
    "assign_wrong_pointer_type.c",
    "bad_null_pointer_constant.c",
    "invalid_static_initializer.c",
    "pass_pointer_as_int.c",
    "return_wrong_pointer_type.c",
])


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter14Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C14 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_14 valid files found")
        for path in files:
            rel = str(path.relative_to(_C14 / "valid"))
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
class TestChapter14InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C14 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C14 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter14InvalidDeclarations(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C14 / "invalid_declarations").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_declarations files found")
        for path in files:
            rel = str(path.relative_to(_C14 / "invalid_declarations"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(Exception):
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter14InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C14 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C14 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
