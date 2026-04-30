"""End-to-end harness for the chapter_18 corpus.

chapter_18 covers `struct` and `union`. c6502 implements neither
yet, so the entire valid/ bucket is pinned as
`_EXPECTED_FAILURES_CODEGEN` — all 108 valid programs use struct
syntax (declarations, compound initializers, member access via
`.` / `->`, struct return types, pointer-to-struct, ...) that
the parser doesn't accept. The pins flip individually as struct
support lands.

The invalid_* buckets work the way they do for the other chapters:
each file must be rejected somewhere in the pipeline. Most of these
contain `struct` keywords which our parser already rejects (with an
UnexpectedInput error), so they pass the "rejected somewhere"
check trivially even though the failure is "the struct keyword
itself isn't accepted" rather than the test's intended cause.

invalid_lex contains two pp-number tests (`.1l`, `.0foo`) — c6502's
lexer doesn't have the C preprocessing-number concept, so these
DON'T fail at lex time the way they do for upstream's compiler.
They DO fail at parse (because of struct), but that's at the wrong
stage for the invalid_lex bucket. They're pinned via
`_INVALID_LEX_NOT_REJECTED_TODAY`.
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
_C18 = _TESTS_DIR / "chapter_18"

_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)


# Permanently incompatible: features c6502 fundamentally can't
# compile. The chapter 18 corpus is all about struct / union, and
# c6502 has neither yet, so this set is empty here — files that
# don't compile because of struct are pinned in
# `_EXPECTED_FAILURES_CODEGEN` instead (so they auto-flip when
# struct support lands).
_INCOMPATIBLE_VALID: frozenset[str] = frozenset()


# A handful of `valid/` files don't actually need struct support to
# compile — they're testing edge cases (e.g. lexer disambiguation
# of `-->` between postfix `--` and `>`) that happen to land in
# this chapter's directory. List them here so the harness checks
# them as ordinary "must compile" tests; every OTHER valid file
# is treated as an expected failure.
_VALID_PASSES_TODAY: frozenset[str] = frozenset({
    # `ptr-->arr` is `ptr-- > arr` — only exercises lexer max-munch
    # for `--` vs. `-->`, doesn't actually declare a struct.
    "extra_credit/other_features/decr_arrow_lexing.c",
})


# Per-bucket pinning for invalid tests c6502 doesn't reject today.
# c6502's lexer has no preprocessing-number concept, so `.1l` (a
# DOT followed by a valid LONG_INTEGER) lexes cleanly even though
# the standard would reject the whole sequence as one ill-formed
# pp-number. The companion case `.0foo` (DOT followed by `0foo`)
# is caught — c6502's INVALID_NUMBER regex flags numeric / digit
# / non-letter abutting an identifier — so only one of the pair
# pins here. The file DOES fail at parse time because of the
# surrounding struct keyword, but that's at the wrong stage for
# this bucket.
_INVALID_LEX_NOT_REJECTED_TODAY: frozenset[str] = frozenset({
    "dot_bad_token.c",
})
_INVALID_PARSE_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
_INVALID_TYPES_NOT_REJECTED_TODAY: frozenset[str] = frozenset()
_INVALID_STRUCT_TAGS_NOT_REJECTED_TODAY: frozenset[str] = frozenset()


# Multi-TU `libraries/` subdirs aren't applicable.
def _is_libraries(rel: str) -> bool:
    return rel.startswith("libraries/") or "/libraries/" in rel


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18Valid(unittest.TestCase):
    def test_codegen(self):
        # Every valid file is expected to fail today (no struct
        # support). The harness iterates and asserts each fails;
        # when struct support lands, individual files start passing
        # and need to be removed from `_EXPECTED_FAILURES_CODEGEN`.
        files = sorted((_C18 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_18 valid files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "valid"))
            if _is_libraries(rel):
                continue
            if rel in _INCOMPATIBLE_VALID:
                continue
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _VALID_PASSES_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(
                        Exception,
                        msg=(
                            f"{rel} unexpectedly compiled — add it "
                            f"to _VALID_PASSES_TODAY"
                        ),
                    ):
                        _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidLex(unittest.TestCase):
    def test_lex_fails(self):
        files = sorted((_C18 / "invalid_lex").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_lex files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_lex"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_LEX_NOT_REJECTED_TODAY:
                    list(tokenize(source))
                else:
                    with self.assertRaises(LexError):
                        list(tokenize(source))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidParse(unittest.TestCase):
    def test_parse_fails(self):
        files = sorted((_C18 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_PARSE_NOT_REJECTED_TODAY:
                    parse(source)
                else:
                    with self.assertRaises(_PARSE_FAILURES):
                        parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidTypes(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C18 / "invalid_types").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_types files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_types"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_TYPES_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter18InvalidStructTags(unittest.TestCase):
    def test_codegen_rejects(self):
        files = sorted((_C18 / "invalid_struct_tags").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_struct_tags files found")
        for path in files:
            rel = str(path.relative_to(_C18 / "invalid_struct_tags"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                if rel in _INVALID_STRUCT_TAGS_NOT_REJECTED_TODAY:
                    _run_stage("codegen", source)
                else:
                    with self.assertRaises(Exception):
                        _run_stage("codegen", source)
