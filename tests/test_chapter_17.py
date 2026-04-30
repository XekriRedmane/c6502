"""End-to-end harness for the chapter_17 corpus.

chapter_17 covers `void`, `void *`, and `sizeof`. c6502 implements
the void / void-pointer half of that work; the sizeof half (along
with anything that needs a real-memory `malloc` / `free` runtime)
isn't modelled, so a sizeof literal value of 4 / 8 / 1 baked into
the test source assumes the standard's "int is 4 bytes" / "double
is 8 bytes" sizing — c6502's storage model uses 1-byte `int` and
the like, so even if we wired up a `sizeof` operator the literal
comparisons in upstream's tests would fail.

Buckets:

  valid/                     — must compile through `--codegen`.
  invalid_parse/             — must fail at lex or parse.
  invalid_types/             — must be rejected somewhere in the
                               pipeline.

`_INCOMPATIBLE_VALID` lists files c6502 fundamentally can't compile
(uses `sizeof`, `malloc` / runtime memory management, or assumes a
larger integer model than c6502). Each entry is keyed against the
per-bucket relative path.

`_NOT_REJECTED_TODAY` per bucket lists files where c6502's current
acceptance/rejection differs from upstream expectation but the
behavior is intentional or low-priority to fix; pinning each entry
flags any drift.
"""

from __future__ import annotations

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from lark.exceptions import UnexpectedInput
from lexer import LexError, tokenize
from parser import ParserError, parse
from preprocessor import preprocess


_TESTS_DIR = Path(__file__).parent
_C17 = _TESTS_DIR / "chapter_17"

_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


# Permanently incompatible: assumes sizeof or runtime memory
# management we don't model. The sizeof tests bake the standard
# "int is 4 bytes" / "double is 8 bytes" into runtime comparisons
# that wouldn't match c6502's 1-byte-int model even if we
# implemented sizeof.
_INCOMPATIBLE_VALID: frozenset[str] = frozenset({
    # sizeof — operator not modelled; literal comparisons assume a
    # different integer model.
    "sizeof/simple.c",
    "sizeof/sizeof_array.c",
    "sizeof/sizeof_basic_types.c",
    "sizeof/sizeof_consts.c",
    "sizeof/sizeof_derived_types.c",
    "sizeof/sizeof_expressions.c",
    "sizeof/sizeof_not_evaluated.c",
    "sizeof/sizeof_result_is_ulong.c",
    "extra_credit/sizeof_bitwise.c",
    "extra_credit/sizeof_compound.c",
    "extra_credit/sizeof_compound_bitwise.c",
    "extra_credit/sizeof_incr.c",
    # void_pointer — uses malloc / free / calloc / realloc /
    # aligned_alloc, which need a heap c6502 doesn't model. Some of
    # these also use sizeof.
    "void_pointer/simple.c",
    "void_pointer/array_of_pointers_to_void.c",
    "void_pointer/common_pointer_type.c",
    "void_pointer/conversion_by_assignment.c",
    "void_pointer/explicit_cast.c",
    "void_pointer/memory_management_functions.c",
})


# Currently fail through `--codegen` despite being in the valid
# corpus; each represents a feature gap or known limitation that
# we'll fix later. Pinned so a regression / a fix flips the harness.
_EXPECTED_FAILURES_CODEGEN: frozenset[str] = frozenset()


# Per-bucket pinning for invalid tests c6502 doesn't reject today.
_INVALID_PARSE_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
_INVALID_TYPES_NOT_REJECTED_TODAY: frozenset[str] = frozenset()


# Multi-TU `libraries/` subdirs aren't applicable.
def _is_libraries(rel: str) -> bool:
    return rel.startswith("libraries/") or "/libraries/" in rel


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter17Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C17 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_17 valid files found")
        for path in files:
            rel = str(path.relative_to(_C17 / "valid"))
            if _is_libraries(rel):
                continue
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _EXPECTED_FAILURES_CODEGEN:
                    with self.assertRaises(
                        Exception,
                        msg=(
                            f"{rel} unexpectedly compiled — drop it "
                            f"from _EXPECTED_FAILURES_CODEGEN"
                        ),
                    ):
                        _run_stage("codegen", source)
                else:
                    _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter17InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C17 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C17 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter17InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C17 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C17 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
