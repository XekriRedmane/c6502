"""End-to-end harness for the chapter_14 corpus.

chapter_14 covers pointer types, the address-of `&` and dereference
`*` operators, and null pointer constants. Four buckets:

  valid/                — must compile through `-S`.
  invalid_parse/        — must fail at lex or parse.
  invalid_declarations/ — must be rejected somewhere in the pipeline.
  invalid_types/        — must be rejected somewhere in the pipeline.

The upstream `valid/libraries/` subdir is multi-TU and isn't applicable.

All `valid/` files now compile through `-S`. Files that
originally used 8-byte-long literals were locally rewritten to
substitute c6502's `long long` (4B) and `unsigned long long`
(4B), with constants scaled accordingly.

Two invalid_parse files (abstract function declarators) are
currently accepted by the parser; they're pinned in
`_INVALID_PARSE_NOT_REJECTED_TODAY`. The invalid_types pin set is
empty — all cross-type pointer assignment shapes are now rejected
at the type-check boundary.
"""

import shutil
import unittest
from pathlib import Path

from compile import _run_stage
from preprocessor import preprocess


_TESTS_DIR = Path(__file__).parent
_C14 = _TESTS_DIR / "chapter_14"


_INCOMPATIBLE_VALID = frozenset()


_EXPECTED_FAILURES_CODEGEN = frozenset()


_INVALID_TYPES_NOT_REJECTED_TODAY = frozenset()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter14Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C14 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_14 valid files found")
        for path in files:
            rel = path.relative_to(_C14 / "valid").as_posix()
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
class TestChapter14InvalidDeclarations(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C14 / "invalid_declarations").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_declarations files found")
        for path in files:
            rel = path.relative_to(_C14 / "invalid_declarations").as_posix()
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
            rel = path.relative_to(_C14 / "invalid_types").as_posix()
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
