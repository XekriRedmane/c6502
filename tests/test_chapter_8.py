"""End-to-end harness for the chapter_8 corpus.

chapter_8 covers iteration statements (`while`, `do`/`while`, `for`),
`break` / `continue`, and `switch`. Three buckets:

  valid/             — must compile through `--codegen`.
  invalid_parse/     — must fail at lex or parse.
  invalid_semantics/ — must be rejected somewhere in the pipeline.
                       Today, files exercising switch fail at parse
                       (not in our grammar) instead of at semantic;
                       once switch lands they'll fail at the intended
                       semantic stage. The harness accepts either
                       so it doesn't have to track that boundary.

c6502 doesn't yet support `switch` / `case` / `default` (the keywords
lex but no grammar production accepts them). All chapter_8/valid files
that use switch are listed in `_EXPECTED_FAILURES_CODEGEN`. Two
non-switch files use literals beyond c6502's 16-bit Long range and
land in `_INCOMPATIBLE`.
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
_C8 = _TESTS_DIR / "chapter_8"


_PARSE_FAILURES = (LexError, ParserError, UnexpectedInput)
_SEMANTIC_FAILURES = (
    IdentifierResolutionError,
    LabelResolutionError,
    LoopLabelingError,
    TypeCheckError,
)
_ANY_REJECTION = _PARSE_FAILURES + _SEMANTIC_FAILURES


_INCOMPATIBLE_VALID = frozenset([
    # Literals beyond ULong's 0..65535 range.
    "empty_loop_body.c",
    "for_absent_post.c",
])


# valid/ files that exercise `switch` — the keyword lexes but no
# grammar rule accepts it, so they fail at parse today. Drop entries
# as a switch implementation lands.
_EXPECTED_FAILURES_CODEGEN = frozenset([
    "extra_credit/case_block.c",
    "extra_credit/duffs_device.c",
    "extra_credit/loop_in_switch.c",
    "extra_credit/switch.c",
    "extra_credit/switch_assign_in_condition.c",
    "extra_credit/switch_break.c",
    "extra_credit/switch_decl.c",
    "extra_credit/switch_default.c",
    "extra_credit/switch_default_fallthrough.c",
    "extra_credit/switch_default_not_last.c",
    "extra_credit/switch_default_only.c",
    "extra_credit/switch_empty.c",
    "extra_credit/switch_fallthrough.c",
    "extra_credit/switch_goto_mid_case.c",
    "extra_credit/switch_in_loop.c",
    "extra_credit/switch_nested_cases.c",
    "extra_credit/switch_nested_not_taken.c",
    "extra_credit/switch_nested_switch.c",
    "extra_credit/switch_no_case.c",
    "extra_credit/switch_not_taken.c",
    "extra_credit/switch_single_case.c",
    "extra_credit/switch_with_continue.c",
    "extra_credit/switch_with_continue_2.c",
])


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter8Valid(unittest.TestCase):
    """Each chapter_8/valid file must compile through `--codegen`,
    except those listed in `_EXPECTED_FAILURES_CODEGEN` (must fail) or
    `_INCOMPATIBLE_VALID` (skipped)."""

    def test_codegen(self):
        files = sorted((_C8 / "valid").rglob("*.c"))
        self.assertGreater(len(files), 0, "no chapter_8 valid files found")
        for path in files:
            rel = str(path.relative_to(_C8 / "valid"))
            if rel in _INCOMPATIBLE_VALID:
                continue
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
class TestChapter8InvalidParse(unittest.TestCase):
    """Each chapter_8/invalid_parse file must fail at lex or parse."""

    def test_parse_fails(self):
        files = sorted((_C8 / "invalid_parse").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_parse files found")
        for path in files:
            rel = str(path.relative_to(_C8 / "invalid_parse"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_PARSE_FAILURES):
                    parse(source)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestChapter8InvalidSemantics(unittest.TestCase):
    """Each chapter_8/invalid_semantics file must be rejected
    somewhere in the pipeline. Today some files (those using switch)
    fail at parse rather than at the intended semantic stage; the
    harness accepts either."""

    def test_codegen_rejects(self):
        files = sorted((_C8 / "invalid_semantics").rglob("*.c"))
        self.assertGreater(len(files), 0, "no invalid_semantics files found")
        for path in files:
            rel = str(path.relative_to(_C8 / "invalid_semantics"))
            with self.subTest(file=rel):
                source = preprocess(path.read_text(), [])
                with self.assertRaises(_ANY_REJECTION):
                    _run_stage("codegen", source)
