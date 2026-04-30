"""End-to-end harness for the chapter_16 corpus.

chapter_16 covers `char` types and string literals (along with
adjacent-string concatenation, char arrays initialised from string
literals, char pointers initialised from string literals, &"..."
lvalue addressing, `char *` parameters, etc.) and is a near-exact
match for the feature set c6502 added in this round of work.

Buckets:

  valid/                   — must compile through `--codegen`.
  invalid_lex/             — must fail at the lexer.
  invalid_parse/           — must fail at lex or parse.
  invalid_types/           — must be rejected somewhere in the
                             pipeline (parse, identifier resolution,
                             label resolution, type checking, or
                             c99_to_tac).
  invalid_labels/          — `case` / `default` / `goto` / labelled-
                             statement rule violations; rejected by
                             label_resolution or loop_labeling.

Multi-TU `libraries/` files exist under `valid/` too; we skip them
at import time the same way every prior chapter does (they need a
linker / second TU we don't model).

`_INCOMPATIBLE_VALID` lists files c6502 fundamentally can't compile
(literals beyond ULong / large-memory model assumptions). Each entry
is keyed against the per-bucket relative path and skipped at harness
time.

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
_C16 = _TESTS_DIR / "chapter_16"

_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


# Permanently incompatible: literals out of c6502's range,
# frames too large, or features that need an `int` larger than
# c6502's 1 byte / a preprocessor that preserves control
# characters in source / a runtime c6502 doesn't model.
_INCOMPATIBLE_VALID: frozenset[str] = frozenset()


# Currently fail through `--codegen` despite being in the valid
# corpus; each represents a feature gap or known limitation that
# we'll fix later. Pinned so a regression / a fix flips the harness.
_EXPECTED_FAILURES_CODEGEN: frozenset[str] = frozenset()


# Per-bucket pinning for invalid tests c6502 doesn't reject today.
_INVALID_LEX_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
_INVALID_PARSE_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
_INVALID_TYPES_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
_INVALID_LABELS_NOT_REJECTED_TODAY: frozenset[str] = frozenset()


# Multi-TU `libraries/` subdirs aren't applicable.
def _is_libraries(rel: str) -> bool:
    return rel.startswith("libraries/") or "/libraries/" in rel


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter16Valid(unittest.TestCase):
    def test_codegen(self):
        files = sorted((_C16 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_16 valid files found")
        for path in files:
            rel = str(path.relative_to(_C16 / "valid"))
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
class TestChapter16InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C16 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C16 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_LEX_NOT_REJECTED_TODAY:
                    list(tokenize(source))
                else:
                    with self.assertRaises(LexError):
                        list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter16InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C16 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C16 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter16InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C16 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C16 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter16InvalidLabels(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C16 / "invalid_labels").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_labels files found")
        for path in files:
            rel = str(path.relative_to(_C16 / "invalid_labels"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_LABELS_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
